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

#include "flowjsonbackendinstance.h"

#include <QVariant>
#include <QFileInfo>
#include <QDebug>

FlowJsonBackendInstance::FlowJsonBackendInstance(QObject *parent)
    : StorageBackendInstance(parent)
{
}

bool FlowJsonBackendInstance::isConfigured() const
{
    return true;
}

QVariant FlowJsonBackendInstance::configuration_impl() const
{
    // No config to save
    return QVariant();
}

void FlowJsonBackendInstance::setConfiguration_impl(const QVariant &)
{
    // No config to save
}

QAbstractItemModel *FlowJsonBackendInstance::taskModel() const
{
    // TODO
    return nullptr;
}
