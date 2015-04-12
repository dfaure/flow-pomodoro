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

#include "distractionsplugin.h"
#include "task.h"

class HostsPlugin : public DistractionsPlugin
{
    Q_OBJECT
    Q_PROPERTY(QString hosts READ hosts WRITE setHosts NOTIFY hostsChanged)
    Q_PLUGIN_METADATA(IID "com.kdab.flow.PluginBase/v1.2")
    Q_INTERFACES(PluginBase)

public:
    HostsPlugin();

    void setTaskStatus(TaskStatus status) override;
    QString text() const override;
    QString helpText() const override;
    QQmlComponent* configComponent() const override;
    void setSettings(QSettings *) override;
    bool enabledByDefault() const override;

    void setHosts(const QString &);
    QString hosts() const;

Q_SIGNALS:
    void hostsChanged();

private:
    bool checkSanity();
    void updateHosts(bool allow);
    void update(bool blockDistractions) override;
    void startProcess(const QString &filename, const QStringList &arguments);
    QString m_hosts;
};

#endif
