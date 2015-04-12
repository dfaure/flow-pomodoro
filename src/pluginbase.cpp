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

#include "pluginbase.h"

#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>

class PluginBase::Private
{
public:
    Private()
        : m_enabled(false)
        , m_settings(nullptr)
        , m_qmlEngine(nullptr)
    {
    }

    bool m_enabled;
    QSettings *m_settings;
    QQuickItem *m_configItem;
    QQmlEngine *m_qmlEngine;
    QString m_lastError;
};


PluginBase::PluginBase() : d(new Private())
{
}

PluginBase::~PluginBase()
{
    delete d;
}

void PluginBase::setEnabled(bool enabled)
{
    if (enabled != d->m_enabled) {
        d->m_enabled = enabled;
        emit enabledChanged(d->m_enabled);
    }
}

void PluginBase::setSettings(QSettings *settings)
{
    Q_ASSERT(!d->m_settings && settings);
    d->m_settings = settings;
}

QSettings* PluginBase::settings() const
{
    return d->m_settings;
}

bool PluginBase::enabled() const
{
    return d->m_enabled;
}

QQuickItem *PluginBase::configureItem() const
{
    return d->m_configItem;
}

QString PluginBase::lastError() const
{
    return d->m_lastError;
}

void PluginBase::setLastError(const QString &lastError)
{
    if (!lastError.isEmpty())
        qWarning() << text() << lastError;
    if (lastError != d->m_lastError) {
        d->m_lastError = lastError;
        emit lastErrorChanged();
    }
}

QString PluginBase::settingsGroup() const
{
    return metaObject()->className();
}

QQmlComponent *PluginBase::configComponent() const
{
    return nullptr;
}

void PluginBase::setQmlEngine(QQmlEngine *engine)
{
    if (d->m_qmlEngine || !engine) {
        qWarning() << "Invalid setQmlEngine call" << engine << d->m_qmlEngine;
        Q_ASSERT(false);
        return;
    }

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

QQmlEngine *PluginBase::qmlEngine() const
{
    return d->m_qmlEngine;
}
