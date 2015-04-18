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

#include "flowjsonplugin.h"
#include "flowjsonbackendinstance.h"

FlowJsonPlugin::FlowJsonPlugin(QObject *parent) : StoragePlugin(parent)
{
}

QString FlowJsonPlugin::text() const
{
    return QStringLiteral("Flow JSON format");
}

QString FlowJsonPlugin::helpText() const
{
    return tr("Reads tasks from a JSON file. This is flow's default format.");
}

bool FlowJsonPlugin::enabledByDefault() const
{
    return false;
}

StorageBackendInstance* FlowJsonPlugin::createBackend_impl()
{
    return new FlowJsonBackendInstance(this);
}

StorageBackendInstance *FlowJsonPlugin::fromConfiguration(const QVariant &)
{
    static FlowJsonBackendInstance *instance = new FlowJsonBackendInstance();
    return instance;
}

bool FlowJsonPlugin::isSingleInstanced() const
{
    return true;
}
