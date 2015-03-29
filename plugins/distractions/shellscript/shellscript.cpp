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

#include "shellscript.h"

#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QRunnable>
#include <QThreadPool>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlContext>
#include <QQmlEngine>
#include <QStandardPaths>

#ifdef Q_OS_LINUX
static QString linuxTextEditor()
{
    // We can't use xdg-open because it will execute the .sh file.
    // A better approach might be
    static QString editor;
    if (editor.isNull()) {
        QStringList candidates;
        candidates << "kwrite" << "kate" << "gedit" << "mousepad" << "leafpad" << "emacs";
        foreach (const QString &candidate, candidates) {
            if (!QStandardPaths::findExecutable(candidate).isEmpty()) {
                editor = candidate;
                break;
            }
        }
    }

    return editor;
}
#endif

class RunScriptTask : public QRunnable
{
public:
    RunScriptTask(const QString &filename, const QStringList &args, QMutex *mutex)
        : m_filename(filename)
        , m_args(args)
        , m_mutex(mutex)
    {
    }

private:
    void run()
    {
        QMutexLocker locker(m_mutex);
        QProcess *process = new QProcess();
        process->start(m_filename, m_args);
        process->waitForFinished();
        delete process;
    }

    QString m_filename;
    QStringList m_args;
    QMutex *m_mutex;
};

ShellScriptPlugin::ShellScriptPlugin() : DistractionsPlugin()
  , m_allowingDistractions(true)
{
    m_scriptName = "shell_script_plugin";
    const QString suffix =
#ifdef Q_OS_WIN
    ".bat";
#else
    ".sh";
#endif

    // Fixes crash in static mode, because qqmlimport calls QPluginLoader::staticPlugins() before us.
    moveToThread(qApp->thread());

    QString dataDirectory = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    m_scriptName = dataDirectory + "/" + m_scriptName + suffix;

    // Create one from template if it doesn't exist
    if (!QFile::exists(m_scriptName)) {
        if (QFile::copy(":/plugins/shellscript/script_template" + suffix, m_scriptName)) {
            QFlags<QFile::Permission> permissions = QFile::permissions(m_scriptName) | QFileDevice::WriteOwner | QFileDevice::ExeOwner;
            if (!QFile::setPermissions(m_scriptName, permissions))
                setLastError(tr("Failed to set permissions on %1").arg(m_scriptName));
        } else {
            setLastError(tr("Failed to create script %1").arg(m_scriptName));
        }
    }

    checkSanity();
}

void ShellScriptPlugin::update(bool allowDistractions)
{
    if (allowDistractions == m_allowingDistractions)
        return;

    m_allowingDistractions = allowDistractions;

    setLastError("");
    if (!checkSanity())
        return;

    QStringList args;
    args << (allowDistractions ? "allow" : "disallow");

    RunScriptTask *task = new RunScriptTask(m_scriptName, args, &m_mutex);
    QThreadPool::globalInstance()->start(task);
}

void ShellScriptPlugin::setTaskStatus(TaskStatus status)
{
    if (enabled()) {
        update(status != TaskStarted);
    }
}

QString ShellScriptPlugin::text() const
{
    return QStringLiteral("Shell script");
}

QString ShellScriptPlugin::helpText() const
{
    return tr("Executes a shell script to enable/disable distractions.\nYou must create or edit <b>%1</b>. The first argument passed to the script will be <b>allow</b> or <b>disallow</b>.").arg(m_scriptName);
}

QQmlComponent *ShellScriptPlugin::configComponent() const
{
#if defined(Q_OS_LINUX)
    if (linuxTextEditor().isEmpty())
        return 0;
#endif

    return new QQmlComponent(qmlEngine(), QUrl("qrc:/plugins/shellscript/Config.qml"),
                             QQmlComponent::PreferSynchronous, const_cast<QObject*>((QObject*)this));
}

bool ShellScriptPlugin::enabledByDefault() const
{
    return true;
}

void ShellScriptPlugin::editScript()
{
    QString command;
#if defined(Q_OS_WIN)
    command = "notepad";
#elif defined(Q_OS_OSX)
    command = "open -a TextEdit";
#elif defined(Q_OS_LINUX)
    command = linuxTextEditor();
#endif

    if (!command.isEmpty()) {
        command += " \"" + m_scriptName + "\"";
        if (!QProcess::startDetached(command)) {
            setLastError(tr("Error running command %1").arg(command));
        }
    }
}

bool ShellScriptPlugin::checkSanity()
{
    QFile file(m_scriptName);
    if (!file.exists()) {
        setLastError(tr("File doesn't exist: %1").arg(m_scriptName));
        return false;
    }

    return true;
}
