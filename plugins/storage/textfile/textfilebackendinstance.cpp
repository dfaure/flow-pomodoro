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

#include "textfilebackendinstance.h"

#include <QVariant>
#include <QFileInfo>
#include <QDebug>

TextFileBackendInstance::TextFileBackendInstance(QObject *parent)
    : StorageBackendInstance(parent)
{
    connect(this, &TextFileBackendInstance::filenameChanged, this, &StorageBackendInstance::configurationChanged);
}

bool TextFileBackendInstance::isConfigured() const
{
    return m_filename.isValid() && m_filename.isLocalFile();
}

QVariant TextFileBackendInstance::configuration_impl() const
{
    return m_filename;
}

void TextFileBackendInstance::setConfiguration_impl(const QVariant &data)
{
    setFilename(data.toUrl());
}

QUrl TextFileBackendInstance::filename() const
{
    return m_filename;
}

QString TextFileBackendInstance::shortname() const
{
    if (isConfigured()) {
        QFileInfo info(m_filename.toLocalFile());
        return info.fileName();
    }

    return QString();
}

void TextFileBackendInstance::setFilename(const QUrl &filename)
{
    if (filename != m_filename) {
        m_filename = filename;
        emit filenameChanged();
    }
}

QAbstractItemModel *TextFileBackendInstance::taskModel() const
{
    // TODO
    return nullptr;
}
