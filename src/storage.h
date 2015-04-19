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

#ifndef FLOW_STORAGE_H
#define FLOW_STORAGE_H

#include "task.h"
#include "tag.h"
#include "flowjsonplugin.h"

#include <QTimer>
#include <QObject>
#include <QUuid>

class Kernel;
class SortedTagsModel;
class TaskFilterProxyModel;
class NonEmptyTagFilterProxy;
class ExtendedTagsModel;

class Storage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ExtendedTagsModel* extendedTagsModel READ extendedTagsModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel* nonEmptyTagsModel READ nonEmptyTagsModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel* tagsModel READ tagsModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel* taskModel READ taskModel CONSTANT)
    Q_PROPERTY(TaskFilterProxyModel* stagedTasksModel READ stagedTasksModel CONSTANT)
    Q_PROPERTY(TaskFilterProxyModel* archivedTasksModel READ archivedTasksModel CONSTANT)
    Q_PROPERTY(TaskFilterProxyModel* taskFilterModel READ taskFilterModel CONSTANT)
    Q_PROPERTY(TaskFilterProxyModel* untaggedTasksModel READ untaggedTasksModel CONSTANT)
    Q_PROPERTY(TaskFilterProxyModel* dueDateTasksModel READ dueDateTasksModel CONSTANT)
    Q_PROPERTY(bool webDAVSyncSupported READ webDAVSyncSupported CONSTANT)

public:
    enum TaskModelRole {
        TaskRole = Qt::UserRole + 1,
        TaskPtrRole,
        DueDateSectionRole
    };

    explicit Storage(Kernel *kernel, QObject *parent = nullptr);
    ~Storage();

    // Temporary disable saving. For performance purposes
    void setDisableSaving(bool);

    bool savingInProgress() const;
    bool loadingInProgress() const;

    bool webDAVSyncSupported() const;

    bool hasTasks() const;

#ifdef DEVELOPER_MODE
    Q_INVOKABLE void removeDuplicateData();
#endif
//------------------------------------------------------------------------------
// Stuff for tasks
    TaskFilterProxyModel* taskFilterModel() const;
    TaskFilterProxyModel* untaggedTasksModel() const;
    TaskFilterProxyModel* dueDateTasksModel() const;
    TaskFilterProxyModel* stagedTasksModel() const;
    TaskFilterProxyModel* archivedTasksModel() const;
    QAbstractItemModel *taskModel() const;
    Task::Ptr taskAt(int index) const;
    Task::Ptr addTask(const QString &taskText, const QString &uid = QString());
    Task::Ptr prependTask(const QString &taskText);
    void removeTask(const Task::Ptr &task);
    void clearTasks();
//------------------------------------------------------------------------------
// Stuff for tags
    Q_INVOKABLE bool removeTag(const QString &tagName);
    Q_INVOKABLE Tag::Ptr createTag(const QString &tagName, const QString &uid = QString());
    Tag::Ptr tag(const QString &name, bool create = true);
    QAbstractItemModel *tagsModel() const;
    ExtendedTagsModel *extendedTagsModel() const;
    bool containsTag(const QString &name) const;
//------------------------------------------------------------------------------

    Q_INVOKABLE QString dataFile() const;

#if defined(BUILT_FOR_TESTING)
    static int storageCount;
    static int saveCallCount;
#endif

    QAbstractItemModel* nonEmptyTagsModel() const;
public Q_SLOTS:
    bool renameTag(const QString &oldName, const QString &newName);
    void dumpDebugInfo();
    void load();
    void save();
    void scheduleSave();

Q_SIGNALS:
    void tagAboutToBeRemoved(const QString &name);

private Q_SLOTS:
    void onTagAboutToBeRemoved(const QString &tagName);

protected:
    Task::Ptr addTask(const Task::Ptr &task);
    Kernel *const m_kernel;

private:
    void connectTask(const Task::Ptr &);
    int proxyRowToSource(int proxyIndex) const;
    SortedTagsModel *m_sortedTagModel;
    TaskFilterProxyModel *const m_taskFilterModel;
    TaskFilterProxyModel *const m_untaggedTasksModel;
    TaskFilterProxyModel *const m_dueDateTasksModel;
    TaskFilterProxyModel *const m_stagedTasksModel;
    TaskFilterProxyModel *const m_archivedTasksModel;
    NonEmptyTagFilterProxy  *const m_nonEmptyTagsModel;
    ExtendedTagsModel *const m_extendedTagsModel;
    FlowJsonBackendInstance *const m_backendInstance;
};

#endif
