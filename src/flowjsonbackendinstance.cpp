/*
  This file is part of Flow.

  Copyright (C) 2014-2015 Sérgio Martins <iamsergio@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "flowjsonbackendinstance.h"
#include "storage.h"
#include "tagmodel.h"
#include "taskmodel.h"
#include "kernel.h"

#include <QVariant>
#include <QFileInfo>
#include <QDebug>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QTemporaryFile>

FlowJsonBackendInstance::FlowJsonBackendInstance(Kernel *kernel, QObject *parent)
    : StorageBackendInstance(parent)
    , m_savingInProgress(false)
    , m_loadingInProgress(false)
    , m_savingDisabled(0)
    , m_kernel(kernel)
    , m_tagModel(new TagModel(this))
    , m_taskModel(new TaskModel(this))
{
    m_scheduleTimer.setSingleShot(true);
    m_scheduleTimer.setInterval(0);

    connect(&m_scheduleTimer, &QTimer::timeout, this, &FlowJsonBackendInstance::save);
    connect(m_tagModel, &TagModel::modelChanged, this, &FlowJsonBackendInstance::scheduleSave);
    connect(m_taskModel, &TaskModel::modelChanged, this, &FlowJsonBackendInstance::scheduleSave);
}

FlowJsonBackendInstance::~FlowJsonBackendInstance()
{
    if (!m_savingDisabled)
        save();
}

bool FlowJsonBackendInstance::isConfigured() const
{
    return true;
}

QVariant FlowJsonBackendInstance::configuration_impl() const
{
    // No config to save
    return QVariant();
}

void FlowJsonBackendInstance::setConfiguration_impl(const QVariant &)
{
    // No config to save
}

QAbstractItemModel *FlowJsonBackendInstance::taskModel() const
{
    return m_taskModel;
}

TagModel *FlowJsonBackendInstance::tagModel() const
{
    return m_tagModel;
}

void FlowJsonBackendInstance::clearTags()
{
    // Don't use clear here
    auto tags = m_tagModel->tags();
    foreach (const Tag::Ptr &tag, tags) {
        removeTag(tag->name());
    }
}

void FlowJsonBackendInstance::scheduleSave()
{
    if (m_savingDisabled == 0) {
        m_scheduleTimer.start();
    }
}

bool FlowJsonBackendInstance::saveScheduled() const
{
    return m_scheduleTimer.isActive();
}

bool FlowJsonBackendInstance::removeTag(const QString &tagName)
{
    int index = m_tagModel->indexOfTag(tagName);
    if (index == -1) {
        qWarning() << Q_FUNC_INFO << "Non existant tag" << tagName;
        return false;
    }

    if (webDAVSyncSupported())
        m_data.deletedItemUids << m_data.tags.at(index)->uuid(); // TODO: Make this persistent
    m_data.tags.removeAt(index);
    m_deletedTagName = tagName;
    return true;
}

void FlowJsonBackendInstance::removeTask(const Task::Ptr &task)
{
    m_taskModel->remove(task);
    task->setTagList(TagRef::List()); // So Tag::taskCount() decreases in case Task::Ptr is left hanging somewhere
    // if (webDAVSyncSupported()) m_data.deletedItemUids << task->uuid(); // TODO: Make this persistent
}

Tag::Ptr FlowJsonBackendInstance::tag(const QString &name, bool create)
{
    Tag::Ptr tag = m_tagModel->tag(name);
    return (tag || !create) ? tag : createTag(name);
}

bool FlowJsonBackendInstance::containsTag(const QString &name) const
{
    return m_tagModel->indexOfTag(name) != -1;
}

Tag::Ptr FlowJsonBackendInstance::createTag(const QString &tagName, const QString &uid)
{
    QString trimmedName = tagName.trimmed();
    if (trimmedName.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Will not add empty tag";
        return Tag::Ptr();
    }

    const int index = m_tagModel->indexOfTag(trimmedName);
    if (index != -1) {
        qDebug() << Q_FUNC_INFO << "Refusing to add duplicate tag " << tagName;
        return m_data.tags.at(index);
    }

    Tag::Ptr tag = Tag::Ptr(new Tag(m_kernel, trimmedName));
    if (!uid.isEmpty())
        tag->setUuid(uid);

    m_data.tags << tag;
    return tag;
}

bool FlowJsonBackendInstance::renameTag(const QString &oldName, const QString &newName)
{
    // For renaming we remove and create instead of simply renaming.
    // Webdav sync isn't prepared for a simple rename, because the task json
    // references the tag name, not the tag uid

    QString trimmedNewName = newName.trimmed();
    if (oldName == newName || trimmedNewName.isEmpty())
        return true;

    if (m_tagModel->indexOfTag(trimmedNewName) != -1)
        return false; // New name already exists

    Tag::Ptr oldTag = tag(oldName, /*create=*/ false);
    if (!oldTag) {
        qWarning() << "Could not find tag with name" << oldName;
        return false;
    }

    if (!createTag(trimmedNewName, oldTag->uuid()))
        return false;

    foreach (const Task::Ptr &task, m_data.tasks) {
        if (task->containsTag(oldName))
            task->addTag(trimmedNewName);
    }

    if (!removeTag(oldName))
        return false;

    scheduleSave();
    return true;
}

void FlowJsonBackendInstance::onTagAboutToBeRemoved(const QString &tagName)
{
    foreach (const Task::Ptr &task, m_data.tasks)
        task->removeTag(tagName);
}

void FlowJsonBackendInstance::dumpDebugInfo()
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << "task count:" << m_data.tasks.count();
    for (int i = 0; i < m_data.tasks.count(); ++i)
        qDebug() << i << m_data.tasks.at(i)->summary();

    if (!m_data.deletedItemUids.isEmpty()) {
        qDebug() << "Items pending deletion on webdav:";
        foreach (const QString &uid, m_data.deletedItemUids)
            qDebug() << uid;
    }
}

void FlowJsonBackendInstance::clearTasks()
{
    if (!m_data.tasks.isEmpty()) {
        // Don't use clear() here
        foreach (const Task::Ptr &task, m_data.tasks) {
            removeTask(task);
        }
        // emit taskCountChanged();
    }
}

Task::Ptr FlowJsonBackendInstance::taskAt(int index) const
{
    return m_data.tasks.value(index);
}

Task::Ptr FlowJsonBackendInstance::addTask(const QString &taskText, const QString &uid)
{
    Task::Ptr task = Task::createTask(m_kernel, taskText, uid);
    return addTask(task);
}

Task::Ptr FlowJsonBackendInstance::prependTask(const QString &taskText)
{
    Task::Ptr task = Task::createTask(m_kernel, taskText);
    m_data.tasks.prepend(task);
    return task;
}

Task::Ptr FlowJsonBackendInstance::addTask(const Task::Ptr &task)
{
    m_data.tasks << task;
    return task;
}

QByteArray FlowJsonBackendInstance::instanceId()
{
    if (m_data.instanceId.isEmpty())
        m_data.instanceId = QUuid::createUuid().toByteArray();

    return m_data.instanceId;
}

void FlowJsonBackendInstance::setDisableSaving(bool disable)
{
    m_savingDisabled += (disable ? 1 : -1);
    if (m_savingDisabled < 0) {
        qWarning() << "invalid value for m_savingDisabled" << m_savingDisabled;
        Q_ASSERT(false);
    }
}

bool FlowJsonBackendInstance::savingInProgress() const
{
    return m_savingInProgress;
}

bool FlowJsonBackendInstance::loadingInProgress() const
{
    return m_loadingInProgress;
}

void FlowJsonBackendInstance::load()
{
    m_loadingInProgress = true;
    m_savingDisabled += 1;
    load_impl();
    m_savingDisabled += -1;

    if (m_data.tags.isEmpty()) {
        // Create default tags. We always use the same uuids for these so we don't get
        // duplicates when synching with other devices
        createTag(tr("work"), "{bb2ab284-8bb7-4aec-a452-084d64e85697}");
        createTag(tr("personal"), "{73533168-9a57-4fc0-ba9a-9120bbadcb6c}");
        createTag(tr("family"), "{4e81dd75-84c4-4359-912c-f3ead717f694}");
        createTag(tr("bills"), "{4b4ae5fb-f35d-4389-9417-96b7ddcb3b8f}");
        createTag(tr("books"), "{b2697470-f457-461c-9310-7d4b56aea395}");
        createTag(tr("movies"), "{387be44a-1eb7-4895-954a-cf5bc82d8f03}");
    }
    m_loadingInProgress = false;
}

void FlowJsonBackendInstance::save()
{
#if defined(BUILT_FOR_TESTING)
    Storage::saveCallCount++;
#endif

    if (!m_kernel->runtimeConfiguration().saveEnabled()) // Unit-tests don't save
        return;

    m_savingInProgress = true;
    m_savingDisabled++;
    save_impl();
    m_savingDisabled--;
    m_savingInProgress = false;
}

void FlowJsonBackendInstance::save_impl()
{
    RuntimeConfiguration runtimeConfiguration = m_kernel->runtimeConfiguration();
    QByteArray serializedData = serializeToJsonData(m_data);
    QString tmpDataFileName = runtimeConfiguration.dataFileName() + "~";;

    QFile temporaryFile(tmpDataFileName); // not using QTemporaryFile so the backup stays next to the main one
    if (!temporaryFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not open" << tmpDataFileName << "for writing"
                   << temporaryFile.errorString() << temporaryFile.error();
        return;
    }

    const QString dataFileName = runtimeConfiguration.dataFileName();
    temporaryFile.write(serializedData, serializedData.count());
    if (QFile::exists(dataFileName) && !QFile::remove(dataFileName)) {
        qWarning() << "Could not update (remove error)" << dataFileName
                   << "backup file is at" << tmpDataFileName;
        return;
    }

    if (!temporaryFile.copy(dataFileName)) {
        qWarning() << "Could not update" << dataFileName
                   << "backup file is at" << tmpDataFileName;
        return;
    }
}

void FlowJsonBackendInstance::load_impl()
{
    const QString dataFileName = m_kernel->runtimeConfiguration().dataFileName();
    qDebug() << "JsonStorage::load_impl Loading from" << dataFileName;
    if (!QFile::exists(dataFileName)) { // Nothing to load
        qDebug() << "JsonStorage::load_impl():" << dataFileName << "does not exist yet.";
        return;
    }

    QFile file(dataFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open data file" << dataFileName
                   << "; error=" << file.errorString() << file.error();
        qFatal("Bailing out");
        return;
    }

    QByteArray serializedData = file.readAll();
    file.close();

    QString errorMsg;
    FlowJsonPlugin::Data data = deserializeJsonData(serializedData, errorMsg, m_kernel);

    if (!errorMsg.isEmpty()) {
        qWarning() << "Error parsing json file" << dataFileName;
        qWarning() << "Error was" << errorMsg;
        qFatal("Bailing out");
        return;
    }

    m_data.tags = data.tags;
    m_data.instanceId = data.instanceId;

    m_data.tasks.clear();
    for (int i = 0; i < data.tasks.count(); ++i) {
        addTask(data.tasks.at(i)); // don't add to m_tasks directly. addTask() does some connects
    }
}

QVariantMap FlowJsonBackendInstance::toJsonVariantMap(const FlowJsonPlugin::Data &data)
{
    QVariantMap map;
    QVariantList tasksVariant;
    QVariantList tagsVariant;
    for (int i = 0; i < data.tags.count(); ++i) {
        tagsVariant << data.tags.at(i)->toJson();
    }

    for (int i = 0; i < data.tasks.count(); ++i) {
        tasksVariant << data.tasks.at(i)->toJson();
    }

    map.insert("instanceId", data.instanceId);
    map.insert("tags", tagsVariant);
    map.insert("tasks", tasksVariant);
    map.insert("JsonSerializerVersion", data.serializerVersion);

    return map;
}

QByteArray FlowJsonBackendInstance::serializeToJsonData(const FlowJsonPlugin::Data &data)
{
    QJsonDocument document = QJsonDocument::fromVariant(toJsonVariantMap(data));
    return document.toJson();
}

FlowJsonPlugin::Data FlowJsonBackendInstance::deserializeJsonData(const QByteArray &serializedData,
                                                                  QString &errorMsg, Kernel *kernel)
{
    FlowJsonPlugin::Data result;
    errorMsg.clear();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(serializedData, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        errorMsg = jsonError.errorString();
        return result;
    }

    QVariantMap rootMap = document.toVariant().toMap();
    QVariantList tagList = rootMap.value("tags").toList();
    QVariantList taskList = rootMap.value("tasks").toList();
    result.instanceId = rootMap.value("instanceId").toByteArray();
    if (result.instanceId.isEmpty())
        result.instanceId = QUuid::createUuid().toByteArray();

    int serializerVersion = rootMap.value("JsonSerializerVersion", FlowJsonPlugin::JsonSerializerVersion1).toInt();
    if (serializerVersion > result.serializerVersion) {
        errorMsg = QString("Found serializer version %1 which is bigger than %2. Update your application").arg(serializerVersion).arg(result.serializerVersion);
        return result;
    }

    foreach (const QVariant &t, tagList) {
        Tag::Ptr tag = Tag::Ptr(new Tag(kernel, QString()));
        tag->fromJson(t.toMap());
        if (!tag->name().isEmpty() && !itemListContains<Tag::Ptr>(result.tags, tag)) {
            if (kernel) // Reuse tags from given storage
                tag = kernel->storage()->tag(tag->name());
            result.tags << tag;
        }
    }

    foreach (const QVariant &t, taskList) {
        Task::Ptr task = Task::createTask(kernel);
        Q_ASSERT(task);
        task->fromJson(t.toMap());
        if (task)
            result.tasks << task;
    }

    return result;
}

bool FlowJsonBackendInstance::webDAVSyncSupported() const
{
#ifndef NO_WEBDAV
    return true;
#endif
    return false;
}

/*
void Storage::setData(FlowJsonPlugin::Data &data)
{
    Tag::List newTags;
    foreach (const Tag::Ptr &tag, data.tags) {
        if (tag->kernel())
            newTags << tag;
        else
            newTags << createTag(tag->name(), tag->uuid());
    }

    data.tags = newTags;

    foreach (const Task::Ptr &task, data.tasks) {
        if (!task->kernel())
            task->setKernel(m_kernel);
    }

    QByteArray oldInstanceId = m_data.instanceId;
    m_data = data;
    if (m_data.instanceId.isEmpty())
        m_data.instanceId = oldInstanceId;

    emit taskCountChanged();
}
*/

#ifdef DEVELOPER_MODE
void Storage::removeDuplicateData()
{
    Data newData;
    foreach (const Task::Ptr &task, m_data.tasks) {
        bool found = false;
        foreach (const Task::Ptr &task2, newData.tasks) {
            if (task->uuid() != task2->uuid())
                continue;

            found = true;
            break;
        }

        if (found) {
            qDebug() << "Task " << task->summary() << task->uuid() << "is a duplicate";
        } else {
            newData.tasks << task;
        }
    }

    foreach (const Tag::Ptr &tag, m_data.tags) {
        bool found = false;
        foreach (const Tag::Ptr &tag2, newData.tags) {
            if (tag->name() != tag2->name())
                continue;

            found = true;
            break;
        }

        if (found) {
            qDebug() << "Tag " << tag->name() << "is a duplicate";
        } else {
            newData.tags << tag;
        }
    }

    setData(newData);
    qDebug() << Q_FUNC_INFO << "done";
}

#endif

