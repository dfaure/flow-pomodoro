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

#ifndef STORAGE_PLUGIN_H
#define STORAGE_PLUGIN_H

#include "pluginbase.h"
#include <QUrl>

class StorageBackendInstance;
class StorageBackendInstanceModel;
class QSettings;

class FLOW_EXPORT StoragePlugin : public PluginBase
{
    Q_OBJECT
    Q_PROPERTY(StorageBackendInstanceModel *instanceModel READ instanceModel CONSTANT)
    Q_PROPERTY(QUrl instanceConfigComponentUrl READ instanceConfigComponentUrl CONSTANT)
public:
    StoragePlugin();

    // The list of backend instances associated with this plugin
    StorageBackendInstanceModel* instanceModel() const;
    QUrl instanceConfigComponentUrl() const;

    Q_INVOKABLE void createBackendInstance();
    Q_INVOKABLE void removeBackendInstance(StorageBackendInstance *);

protected:
    virtual StorageBackendInstance* createBackend_impl() = 0;
    virtual StorageBackendInstance *fromConfiguration(const QVariant &conf) = 0;

private:
    void loadBackendInstances();
    void saveBackendInstances();
    void connectInstance(StorageBackendInstance *);
    QQmlComponent *configComponent() const override;

    StorageBackendInstanceModel *const m_instanceModel;
    QSettings *const m_settings;
};

#endif
