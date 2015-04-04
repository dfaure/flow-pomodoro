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

#ifndef SHELLSCRIPT_PLUGIN_H
#define SHELLSCRIPT_PLUGIN_H

#include "distractionsplugin.h"
#include "task.h"

#include <QProcess>
#include <QMutex>

class ShellScriptPlugin : public DistractionsPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.kdab.flow.PluginBase/v1.2")
    Q_INTERFACES(PluginBase)

public:
    ShellScriptPlugin();

    void setTaskStatus(TaskStatus status) Q_DECL_OVERRIDE;
    QString text() const Q_DECL_OVERRIDE;
    QString helpText() const Q_DECL_OVERRIDE;
    QQmlComponent* configComponent() const Q_DECL_OVERRIDE;
    bool enabledByDefault() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void editScript();

private:
    bool checkSanity();
    void update(bool blockDistractions) Q_DECL_OVERRIDE;
    void startProcess(const QString &filename, const QStringList &arguments);
    QMutex m_mutex;
    bool m_allowingDistractions;
    QString m_scriptName;
    QQmlEngine *m_qmlEngine;
    QQuickItem *m_configItem;
};

#endif
