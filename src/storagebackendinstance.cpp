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

#include "storagebackendinstance.h"

#include <QVariant>
#include <QDebug>

StorageBackendInstance::StorageBackendInstance(QObject *parent)
    : QObject(parent)
    , m_enabled(false)
{
    connect(this, &StorageBackendInstance::enabledChanged, this, &StorageBackendInstance::configurationChanged);
}

StorageBackendInstance::~StorageBackendInstance()
{
}

bool StorageBackendInstance::enabled() const
{
    return m_enabled;
}

void StorageBackendInstance::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        emit enabledChanged();
    }
}

QVariant StorageBackendInstance::configuration() const
{
    auto baseClassConf = QVariant(m_enabled);
    auto derivedClassConf = configuration_impl();

    return QVariantList() << baseClassConf << derivedClassConf;
}

void StorageBackendInstance::setConfiguration(const QVariant &conf)
{
    auto list = conf.toList();
    if (list.count() == 2) {
        m_enabled = list.first().toBool();
        setConfiguration_impl(list.last());
    } else if (list.count() != 0) {
        qWarning() << "Ignoring storage backend conf with more than 2 elements" << list.count();
    }
}
