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

#include "storage.h"
#include "tag.h"
#include "tagmodel.h"
#include "task.h"
#include "kernel.h"
#include "extendedtagsmodel.h"
#include "sortedtagsmodel.h"
#include "taskfilterproxymodel.h"
#include "webdavsyncer.h"
#include "runtimeconfiguration.h"
#include "nonemptytagfilterproxy.h"
#include "flowjsonbackendinstance.h"

#if defined(BUILT_FOR_TESTING)
# include "assertingproxymodel.h"
  int Storage::storageCount = 0;
  int Storage::saveCallCount = 0;
#endif

Storage::Storage(Kernel *kernel, QObject *parent)
    : QObject(parent)
    , m_kernel(kernel)
    , m_taskFilterModel(new TaskFilterProxyModel(this))
    , m_untaggedTasksModel(new TaskFilterProxyModel(this))
    , m_dueDateTasksModel(new TaskFilterProxyModel(this))
    , m_stagedTasksModel(new TaskFilterProxyModel(this))
    , m_archivedTasksModel(new TaskFilterProxyModel(this))
    , m_nonEmptyTagsModel(new NonEmptyTagFilterProxy(this))
    , m_extendedTagsModel(new ExtendedTagsModel(this))
    , m_backendInstance(FlowJsonPlugin::backendInstance(kernel))
{
    m_sortedTagModel = new SortedTagsModel(m_backendInstance->tagModel(), this);
    m_extendedTagsModel->setSourceModel(m_sortedTagModel);
    m_nonEmptyTagsModel->setSourceModel(m_extendedTagsModel);

    connect(this, &Storage::tagAboutToBeRemoved,
            this, &Storage::onTagAboutToBeRemoved);

    m_taskFilterModel->setSourceModel(m_backendInstance->taskModel());
    m_untaggedTasksModel->setSourceModel(m_archivedTasksModel);
    m_dueDateTasksModel->setSourceModel(m_archivedTasksModel);

    m_stagedTasksModel->setSourceModel(m_backendInstance->taskModel());
    m_stagedTasksModel->setFilterStaged(true);

    m_archivedTasksModel->setSourceModel(m_backendInstance->taskModel());
    m_archivedTasksModel->setFilterArchived(true);
    m_untaggedTasksModel->setFilterUntagged(true);
    m_untaggedTasksModel->setObjectName("Untagged and archived tasks model");
    m_dueDateTasksModel->setFilterDueDated(true);
    m_dueDateTasksModel->setObjectName("Archived tasks with due date");
    m_dueDateTasksModel->sort(0, Qt::AscendingOrder);

#if defined(BUILT_FOR_TESTING)
    AssertingProxyModel *assert = new AssertingProxyModel(this);
    assert->setSourceModel(tasksModel);
    assert = new AssertingProxyModel(this);
    assert->setSourceModel(tagsModel);
    assert = new AssertingProxyModel(this);
    assert->setSourceModel(m_stagedTasksModel);
    assert = new AssertingProxyModel(this);
    assert->setSourceModel(m_archivedTasksModel);
    assert = new AssertingProxyModel(this);
    assert->setSourceModel(m_untaggedTasksModel);
    assert = new AssertingProxyModel(this);
    assert->setSourceModel(m_taskFilterModel);
    assert = new AssertingProxyModel(this);
    assert->setSourceModel(m_sortedTagModel);
    assert = new AssertingProxyModel(this);
    assert->setSourceModel(m_extendedTagsModel);
    assert = new AssertingProxyModel(this);
    assert->setSourceModel(m_dueDateTasksModel);
    storageCount++;
    qDebug() << "Created storage" << this << "; count is now" << storageCount;
#endif

}

Storage::~Storage()
{
#if defined(BUILT_FOR_TESTING)
    storageCount--;
    qDebug() << "Deleted storage" << this << "; count is now" << storageCount;
#endif
}

void Storage::load()
{
    m_backendInstance->load();
}

void Storage::save()
{
    m_backendInstance->save();
}

void Storage::scheduleSave()
{
    m_backendInstance->scheduleSave();
}

bool Storage::removeTag(const QString &tagName)
{
    emit tagAboutToBeRemoved(tagName);
    return m_backendInstance->removeTag(tagName);
}

Tag::Ptr Storage::tag(const QString &name, bool create)
{
   return m_backendInstance->tag(name, create);
}

Tag::Ptr Storage::createTag(const QString &tagName, const QString &uid)
{
    return m_backendInstance->createTag(tagName, uid);
}

QString Storage::dataFile() const
{
    return m_kernel->runtimeConfiguration().dataFileName();
}

QAbstractItemModel *Storage::tagsModel() const
{
    return m_sortedTagModel;
}

bool Storage::containsTag(const QString &name) const
{
    return m_backendInstance->containsTag(name);
}

bool Storage::renameTag(const QString &oldName, const QString &newName)
{
    return m_backendInstance->renameTag(oldName, newName);
}

void Storage::onTagAboutToBeRemoved(const QString &tagName)
{
    return m_backendInstance->onTagAboutToBeRemoved(tagName);
}

TaskFilterProxyModel *Storage::taskFilterModel() const
{
    return m_taskFilterModel;
}

TaskFilterProxyModel *Storage::untaggedTasksModel() const
{
    return m_untaggedTasksModel;
}

TaskFilterProxyModel *Storage::dueDateTasksModel() const
{
    return m_dueDateTasksModel;
}

TaskFilterProxyModel *Storage::stagedTasksModel() const
{
    return m_stagedTasksModel;
}

TaskFilterProxyModel *Storage::archivedTasksModel() const
{
    return m_archivedTasksModel;
}

QAbstractItemModel *Storage::taskModel() const
{
    return m_backendInstance->taskModel();
}

void Storage::dumpDebugInfo()
{
    m_backendInstance->dumpDebugInfo();
}

int Storage::proxyRowToSource(int proxyRow) const
{
    QModelIndex proxyIndex = m_taskFilterModel->index(proxyRow, 0);
    QModelIndex index = m_taskFilterModel->mapToSource(proxyIndex);

    return index.isValid() ? index.row() : -1;
}

void Storage::clearTasks()
{
    m_backendInstance->clearTasks();
}

void Storage::setDisableSaving(bool disable)
{
    m_backendInstance->setDisableSaving(disable);
}

bool Storage::savingInProgress() const
{
    return m_backendInstance->savingInProgress();
}

bool Storage::loadingInProgress() const
{
    return m_backendInstance->loadingInProgress();
}

bool Storage::webDAVSyncSupported() const
{
    return m_backendInstance->webDAVSyncSupported();
}

bool Storage::hasTasks() const
{
    return m_backendInstance->taskModel()->rowCount() > 0;
}

Task::Ptr Storage::taskAt(int index) const
{
    return m_backendInstance->taskAt(index);
}

Task::Ptr Storage::addTask(const QString &taskText, const QString &uid)
{
    return m_backendInstance->addTask(taskText, uid);
}

Task::Ptr Storage::prependTask(const QString &taskText)
{
    auto task = m_backendInstance->prependTask(taskText);
    connectTask(task);
    return task;
}

Task::Ptr Storage::addTask(const Task::Ptr &task)
{
    auto addedTask = m_backendInstance->addTask(task);
    connectTask(addedTask);
    return addedTask;
}

void Storage::connectTask(const Task::Ptr &task)
{
    connect(task.data(), &Task::changed, this,
            &Storage::scheduleSave, Qt::UniqueConnection);
    connect(task.data(), &Task::stagedChanged, m_stagedTasksModel,
            &TaskFilterProxyModel::invalidateFilter, Qt::UniqueConnection);
    connect(task.data(), &Task::stagedChanged, m_archivedTasksModel,
            &TaskFilterProxyModel::invalidateFilter, Qt::UniqueConnection);
    connect(task.data(), &Task::tagsChanged, m_untaggedTasksModel,
            &TaskFilterProxyModel::invalidateFilter, Qt::UniqueConnection);
    connect(task.data(), &Task::dueDateChanged, m_dueDateTasksModel,
            &TaskFilterProxyModel::invalidate, Qt::UniqueConnection); // invalidate sorting too
    connect(task.data(), &Task::statusChanged, m_stagedTasksModel,
            &TaskFilterProxyModel::invalidateFilter, Qt::UniqueConnection);

    connect(task.data(), &Task::priorityChanged, m_stagedTasksModel,
            &TaskFilterProxyModel::invalidate, Qt::UniqueConnection);
    connect(task.data(), &Task::priorityChanged, m_archivedTasksModel,
            &TaskFilterProxyModel::invalidate, Qt::UniqueConnection);
}

void Storage::removeTask(const Task::Ptr &task)
{
    m_backendInstance->removeTask(task);
}

QAbstractItemModel* Storage::nonEmptyTagsModel() const
{
    return m_nonEmptyTagsModel;
}

ExtendedTagsModel* Storage::extendedTagsModel() const
{
    return m_extendedTagsModel;
}
