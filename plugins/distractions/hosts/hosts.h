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

#ifndef HOSTS_PLUGIN_H
#define HOSTS_PLUGIN_H

#include "plugininterface.h"
#include "task.h"

class HostsPlugin : public PluginInterface
{
    Q_OBJECT
    Q_PROPERTY(QString hosts READ hosts WRITE setHosts NOTIFY hostsChanged)
    Q_PLUGIN_METADATA(IID "com.kdab.flow.PluginInterface/v0.9.3")
    Q_INTERFACES(PluginInterface)

public:
    HostsPlugin();

    void setTaskStatus(TaskStatus status) Q_DECL_OVERRIDE;
    QString text() const Q_DECL_OVERRIDE;
    QString helpText() const Q_DECL_OVERRIDE;
    QQmlComponent* configComponent() const Q_DECL_OVERRIDE;
    void setSettings(QSettings *) Q_DECL_OVERRIDE;
    bool enabledByDefault() const Q_DECL_OVERRIDE;

    void setHosts(const QString &);
    QString hosts() const;

Q_SIGNALS:
    void hostsChanged();

private:
    bool checkSanity();
    void updateHosts(bool allow);
    void update(bool blockDistractions) Q_DECL_OVERRIDE;
    void startProcess(const QString &filename, const QStringList &arguments);
    QString m_hosts;
};

#endif
