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

#ifndef FLOW_JSON_BACKEND_H
#define FLOW_JSON_BACKEND_H

#include "storagebackendinstance.h"
#include "flowjsonplugin.h"
#include "runtimeconfiguration.h"

#include <QUrl>
#include <QTimer>

class Kernel;
class TagModel;
class TaskModel;

class FlowJsonBackendInstance : public StorageBackendInstance
{
    Q_OBJECT
public:
    explicit FlowJsonBackendInstance(Kernel *, QObject *parent = nullptr);
    ~FlowJsonBackendInstance();

    bool isConfigured() const override;

    QAbstractItemModel *taskModel() const override;
    TagModel *tagModel() const;

    // Tags:
    void clearTags();
    bool removeTag(const QString &tagName);
    Tag::Ptr createTag(const QString &tagName, const QString &uid = QString());
    Tag::Ptr tag(const QString &name, bool create = true);
    bool containsTag(const QString &name) const;
    bool renameTag(const QString &oldName, const QString &newName);
    void onTagAboutToBeRemoved(const QString &tagName);

    // Tasks:
    void removeTask(const Task::Ptr &);
    Task::Ptr taskAt(int index) const;
    Task::Ptr addTask(const QString &taskText, const QString &uid = QString());
    Task::Ptr addTask(const Task::Ptr &);
    Task::Ptr prependTask(const QString &taskText);
    void clearTasks();

    bool saveScheduled() const;
    void scheduleSave();
    void dumpDebugInfo();
    QByteArray instanceId();
    void setDisableSaving(bool disable);
    bool savingInProgress() const;
    bool loadingInProgress() const;
    void load();
    void save();
    bool webDAVSyncSupported() const;

    static FlowJsonPlugin::Data deserializeJsonData(const QByteArray &serializedData, QString &error,
                                                    Kernel *kernel);
    static QByteArray serializeToJsonData(const FlowJsonPlugin::Data &);

    template <typename T>
    static inline bool itemListContains(const QVector<T> &list, const T &item)
    {
        return FlowJsonBackendInstance::indexOfItem(list, item) != -1;
    }

    template <typename T>
    static inline int indexOfItem(const QVector<T> &list, const T &item)
    {
        for (int i = 0; i < list.count(); i++)
            if (*list.at(i).data() == *item.data())
                return i;

        return -1;
    }


protected:
    QVariant configuration_impl() const override;
    void setConfiguration_impl(const QVariant &) override;

private:
    static QVariantMap toJsonVariantMap(const FlowJsonPlugin::Data &);
    void load_impl();
    void save_impl();

    FlowJsonPlugin::Data m_data;
    QTimer m_scheduleTimer;
    QString m_deletedTagName;
    bool m_savingInProgress;
    bool m_loadingInProgress;
    int m_savingDisabled;
    const RuntimeConfiguration m_runtimeConfiguration;
    Kernel *const m_kernel;

    TagModel *const m_tagModel;
    TaskModel *const m_taskModel;
};

#endif
