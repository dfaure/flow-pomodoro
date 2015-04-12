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

#ifndef STORAGE_BACKEND_H
#define STORAGE_BACKEND_H

#include "flow_export.h"

#include <QObject>

class QVariant;
class QAbstractItemModel;

class FLOW_EXPORT StorageBackendInstance : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConfigured READ isConfigured NOTIFY configurationChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
public:
    typedef QList<StorageBackendInstance*> List;
    explicit StorageBackendInstance(QObject *parent = nullptr);
    ~StorageBackendInstance();

    bool enabled() const;
    void setEnabled(bool);
    virtual bool isConfigured() const = 0;

    QVariant configuration() const;
    void setConfiguration(const QVariant &);

    /**
     * Returns a model with the following roles:
     *   TaskRole = Qt::UserRole + 1,
     *   TaskPtrRole,
     *   DueDateSectionRole
     *
     * and the following role names:
     * "task", "taskPtr" and "dueDateSection"
     */
    virtual QAbstractItemModel *taskModel() const = 0;

Q_SIGNALS:
    void configurationChanged();
    void enabledChanged();

protected:
    virtual QVariant configuration_impl() const = 0;
    virtual void setConfiguration_impl(const QVariant &) = 0;

private:
    bool m_enabled;
};

#endif
