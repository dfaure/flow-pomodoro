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

#ifndef TAG_MODEL_H
#define TAG_MODEL_H

#include "tag.h"

#include <QAbstractListModel>
#include <QList>

class TagModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Role {
        TagRole = Qt::UserRole,
        TagPtrRole,
        LastRole
    };
    Q_ENUMS(Role)

    explicit TagModel(QObject *parent = nullptr);
    ~TagModel();
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    int count() const;
    Tag::List tags() const;
    void remove(const Tag::Ptr &);
    int indexOfTag(const QString &name) const;
    Tag::Ptr tag(const QString &name) const;

Q_SIGNALS:
    void countChanged();
    void modelChanged();

private:
    Tag::List m_tags;
};

#endif
