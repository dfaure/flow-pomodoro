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

#include "taskmodel.h"
#include "tagmodel.h"
#include "tagrefmodel.h"

TagRefModel::TagRefModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(this, &TagRefModel::rowsInserted, this, &TagRefModel::countChanged);
    connect(this, &TagRefModel::rowsRemoved, this, &TagRefModel::countChanged);
    connect(this, &TagRefModel::modelReset, this, &TagRefModel::countChanged);
    connect(this, &TagRefModel::layoutChanged, this, &TagRefModel::countChanged);

    connect(this, &TagRefModel::countChanged, this, &TagRefModel::modelChanged);
    connect(this, &QAbstractListModel::dataChanged, this, &TagRefModel::modelChanged);
}

TagRefModel::~TagRefModel()
{
}

int TagRefModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_tagRefs.size();
}

QVariant TagRefModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= static_cast<int>(m_tagRefs.size()))
        return QVariant();

    auto tagRef = m_tagRefs.at(index.row());

    switch (role) {
    case TagModel::TagRole:
        return QVariant::fromValue<Tag*>(tagRef.tag().data());
    case TaskModel::TaskRole:
        return QVariant::fromValue<Task*>(tagRef.m_task.data());
    default:
        return QVariant();
    }

    return QVariant();
}

QHash<int, QByteArray> TagRefModel::roleNames() const
{
    auto roles = QAbstractListModel::roleNames();
    roles.insert(TagModel::TagRole, "tag");
    roles.insert(TaskModel::TaskRole, "task");

    return roles;
}

int TagRefModel::count() const
{
    return m_tagRefs.size();
}

bool TagRefModel::contains(const QString &tagName) const
{
    return indexOfTag(tagName) != -1;
}

void TagRefModel::removeAt(int index)
{
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        m_tagRefs.erase(m_tagRefs.begin() + index);
        endRemoveRows();
    }
}

TagRef TagRefModel::at(int index) const
{
    return m_tagRefs.at(index);
}

void TagRefModel::append(const TagRef &tagRef)
{
    const int count = m_tagRefs.size();
    beginInsertRows(QModelIndex(), count, count);
    m_tagRefs.push_back(tagRef);
    endInsertRows();
}

bool TagRefModel::isEmpty() const
{
    return m_tagRefs.empty();
}

void TagRefModel::clear()
{
    if (!m_tagRefs.empty()) {
        beginResetModel();
        m_tagRefs.clear();
        endResetModel();
    }
}

bool TagRefModel::equalTags(TagRefModel *other) const
{
    const size_t count = m_tagRefs.size();
    const TagRef::List &otherTagRefs = other->m_tagRefs;
    if (count != otherTagRefs.size())
        return false;

    for (size_t i = 0; i < count; ++i) {
        if (!m_tagRefs.at(i).tag()->equals(otherTagRefs.at(i).tag().data()))
            return false;
    }

    return true;
}

int TagRefModel::indexOfTag(const QString &name) const
{
    QString trimmedName = name.toLower().trimmed();
    const int count = m_tagRefs.size();
    for (int i = 0; i < count; ++i) {
        Q_ASSERT(m_tagRefs.at(i).storage());
        if (m_tagRefs.at(i).tagName().toLower() == trimmedName)
            return i;
    }

    return -1;
}

/*
void TagRefModel::remove(const Tag::Ptr &tag)
{
    const int index = m_tagRefs.indexOf(tag);
    if (index != -1) {
        beginRemoveRows(QModelIndex(), index, index);
        m_tagRefs.removeAt(index);
        endRemoveRows();
    }
}



Tag::Ptr TagRefModel::tag(const QString &name) const
{
    const int index = indexOfTag(name);
    return index == -1 ? Tag::Ptr() : m_tagRefs.at(index);
}
*/
