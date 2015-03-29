/*
  This file is part of Flow.

  Copyright (C) 2013 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Sérgio Martins <sergio.martins@kdab.com>

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

#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include "flow_interface_export.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QtPlugin>
#include <QSettings>

class Controller;
class QQmlEngine;
class QQuickItem;
class QSettings;
class QQmlComponent;

class FLOWINTERFACE_EXPORT PluginInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
public:
    typedef QList<PluginInterface*> List;
    PluginInterface();
    virtual ~PluginInterface();

    bool enabled() const;
    void setEnabled(bool enabled);

    virtual void setSettings(QSettings *);
    QSettings *settings() const;

    virtual void setQmlEngine(QQmlEngine *);
    QQmlEngine *qmlEngine() const;
    QQuickItem* configureItem() const;
    QString lastError() const;
    void setLastError(const QString &);

    virtual QQmlComponent *configComponent() const;

    virtual QString text() const = 0;
    virtual QString helpText() const = 0;
    virtual bool enabledByDefault() const = 0;

Q_SIGNALS:
    void lastErrorChanged();
    void enabledChanged(bool enabled);

private:
    class Private;
    Private *const d;
};

Q_DECLARE_INTERFACE(PluginInterface, "com.kdab.flow.PluginInterface/v0.9.3")

#endif
