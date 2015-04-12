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

#ifndef TEXT_FILE_PLUGIN_H
#define TEXT_FILE_PLUGIN_H

#include "storageplugin.h"

class StorageBackendInstance;

class TextFilePlugin : public StoragePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.kdab.flow.PluginBase/v1.2")
    Q_INTERFACES(PluginBase)
public:
    TextFilePlugin();

    QString text() const override;
    QString helpText() const override;
    bool enabledByDefault() const override;
    StorageBackendInstance *createBackend_impl() override;
    StorageBackendInstance *fromConfiguration(const QVariant &conf) override;
};

#endif
