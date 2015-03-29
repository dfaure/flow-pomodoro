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

#include "plugininterface.h"

#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>

class PluginInterface::Private
{
public:
    Private()
        : m_enabled(false)
        , m_settings(0)
    {
    }

    bool m_enabled;
    QSettings *m_settings;
    QQuickItem *m_configItem;
    QQmlEngine *m_qmlEngine;
    QString m_lastError;
};


PluginInterface::PluginInterface() : d(new Private())
{
}

PluginInterface::~PluginInterface()
{
    delete d;
}

void PluginInterface::setEnabled(bool enabled)
{
    if (enabled != d->m_enabled) {
        d->m_enabled = enabled;
        emit enabledChanged(d->m_enabled);
    }
}

void PluginInterface::setSettings(QSettings *settings)
{
    Q_ASSERT(!d->m_settings && settings);
    d->m_settings = settings;
}

QSettings* PluginInterface::settings() const
{
    return d->m_settings;
}

bool PluginInterface::enabled() const
{
    return d->m_enabled;
}

QQuickItem *PluginInterface::configureItem() const
{
    return d->m_configItem;
}

QString PluginInterface::lastError() const
{
    return d->m_lastError;
}

void PluginInterface::setLastError(const QString &lastError)
{
    if (!lastError.isEmpty())
        qWarning() << text() << lastError;
    if (lastError != d->m_lastError) {
        d->m_lastError = lastError;
        emit lastErrorChanged();
    }
}

QQmlComponent *PluginInterface::configComponent() const
{
    return 0;
}

void PluginInterface::setQmlEngine(QQmlEngine *engine)
{
    Q_ASSERT(!d->m_qmlEngine && engine);
    d->m_qmlEngine = engine;

    QQmlComponent *component = configComponent();
    if (!component)
        return;

    if (component->isError()) {
        setLastError("Error creating component: " + component->errorString());
        return;
    }

    QQmlContext *subContext = new QQmlContext(engine->rootContext());
    d->m_configItem = qobject_cast<QQuickItem*>(component->create(subContext));
    subContext->setContextProperty("_plugin", this);

    if (!d->m_qmlEngine) {
        setLastError("Error creating item");
        return;
    }
}

QQmlEngine *PluginInterface::qmlEngine() const
{
    return d->m_qmlEngine;
}
