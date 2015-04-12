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

#ifndef TEXT_FILE_BACKEND_H
#define TEXT_FILE_BACKEND_H

#include "storagebackendinstance.h"

#include <QUrl>

class TextFileBackendInstance : public StorageBackendInstance
{
    Q_OBJECT
    Q_PROPERTY(QUrl filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(QString shortname READ shortname NOTIFY filenameChanged)
public:
    explicit TextFileBackendInstance(QObject *parent = nullptr);
    bool isConfigured() const override;

    QUrl filename() const;
    QString shortname() const;
    void setFilename(const QUrl &);

    QAbstractItemModel *taskModel() const override;

Q_SIGNALS:
    void filenameChanged();

protected:
    QVariant configuration_impl() const override;
    void setConfiguration_impl(const QVariant &) override;

private:
    QUrl m_filename;
};

#endif
