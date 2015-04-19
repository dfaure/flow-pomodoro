/*
  This file is part of Flow.

  Copyright (C) 2015 Sérgio Martins <iamsergio@gmail.com>

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

#ifndef FLOW_JSON_PLUGIN_H
#define FLOW_JSON_PLUGIN_H

#include "storageplugin.h"
#include "task.h"
#include "tag.h"

class Kernel;
class StorageBackendInstance;
class FlowJsonBackendInstance;

// Implementation of flow's native JSON storage format

class FlowJsonPlugin : public StoragePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.kdab.flow.PluginBase/v1.2")
    Q_INTERFACES(PluginBase)
public:
    enum {
        JsonSerializerVersion1 = 1
    };

    struct Data {
        Data() : serializerVersion(JsonSerializerVersion1) {}
        Task::List tasks;
        Tag::List tags;
        QStringList deletedItemUids; // so we can sync to server
        int serializerVersion;
        QByteArray instanceId;
    };

    explicit FlowJsonPlugin(QObject *parent = nullptr);

    QString text() const override;
    QString helpText() const override;
    bool enabledByDefault() const override;
    StorageBackendInstance *createBackend_impl() override;
    StorageBackendInstance *fromConfiguration(const QVariant &conf) override;

    static FlowJsonBackendInstance *backendInstance(Kernel * = nullptr);

protected:
    bool isSingleInstanced() const override;
    static FlowJsonBackendInstance *s_backendInstance;
};

#endif
