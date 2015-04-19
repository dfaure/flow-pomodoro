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

#include "tagmodel.h"

TagModel::TagModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(this, &TagModel::rowsInserted, this, &TagModel::countChanged);
    connect(this, &TagModel::rowsRemoved, this, &TagModel::countChanged);
    connect(this, &TagModel::modelReset, this, &TagModel::countChanged);
    connect(this, &TagModel::layoutChanged, this, &TagModel::countChanged);

    connect(this, &TagModel::countChanged, this, &TagModel::modelChanged);
    connect(this, &QAbstractListModel::dataChanged, this, &TagModel::modelChanged);
}

TagModel::~TagModel()
{
}

int TagModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_tags.count();
}

QVariant TagModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_tags.count())
        return QVariant();

    auto tag = m_tags.at(index.row());
    Q_ASSERT(tag != nullptr);

    switch (role) {
    case TagRole:
        return QVariant::fromValue<Tag*>(tag.data());
    case TagPtrRole:
        return QVariant::fromValue<Tag::Ptr>(tag);
    }

    return QVariant();
}

QHash<int, QByteArray> TagModel::roleNames() const
{
    auto roles = QAbstractListModel::roleNames();
    roles.insert(TagRole, "tag");
    roles.insert(TagPtrRole, "tagPtr");

    return roles;
}

int TagModel::count() const
{
    return m_tags.count();
}

Tag::List TagModel::tags() const
{
    return m_tags;
}

void TagModel::remove(const Tag::Ptr &tag)
{
    const int index = m_tags.indexOf(tag);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        m_tags.removeAt(index);
        endRemoveRows();
    }
}

int TagModel::indexOfTag(const QString &name) const
{
    QString normalizedName = name.toLower().trimmed();
    for (int i = 0; i < m_tags.count(); ++i) {
        if (m_tags.at(i)->name().toLower() == normalizedName)
            return i;
    }

    return -1;
}

Tag::Ptr TagModel::tag(const QString &name) const
{
    const int index = indexOfTag(name);
    return index == -1 ? Tag::Ptr() : m_tags.at(index);
}
