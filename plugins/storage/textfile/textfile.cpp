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

#include "textfile.h"
#include "textfilebackendinstance.h"

TextFilePlugin::TextFilePlugin() : StoragePlugin()
{
}

QString TextFilePlugin::text() const
{
    return QStringLiteral("Text file");
}

QString TextFilePlugin::helpText() const
{
    return tr("Reads tasks from a plain text file. Each line is a task.");
}

bool TextFilePlugin::enabledByDefault() const
{
    return false;
}

StorageBackendInstance* TextFilePlugin::createBackend_impl()
{
    return new TextFileBackendInstance(this);
}

StorageBackendInstance *TextFilePlugin::fromConfiguration(const QVariant &conf)
{
    auto backendInstance = new TextFileBackendInstance(this);
    backendInstance->setConfiguration(conf);
    return backendInstance;
}
