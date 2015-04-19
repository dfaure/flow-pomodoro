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

TaskModel::TaskModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(this, &TaskModel::rowsInserted, this, &TaskModel::countChanged);
    connect(this, &TaskModel::rowsRemoved, this, &TaskModel::countChanged);
    connect(this, &TaskModel::modelReset, this, &TaskModel::countChanged);
    connect(this, &TaskModel::layoutChanged, this, &TaskModel::countChanged);

    connect(this, &TaskModel::countChanged, this, &TaskModel::modelChanged);
    connect(this, &QAbstractListModel::dataChanged, this, &TaskModel::modelChanged);
}

TaskModel::~TaskModel()
{
}

int TaskModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_tasks.count();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_tasks.count())
        return QVariant();

    auto task = m_tasks.at(index.row());
    Q_ASSERT(task != nullptr);

    switch (role) {
    case TaskRole:
        return QVariant::fromValue<Task*>(task.data());
    case TaskPtrRole:
        return QVariant::fromValue<Task::Ptr>(task);
    case DueDateSectionRole:
        if (task && task->dueDate().isValid()) {
            const QDate today = QDate::currentDate();
            const QDate date = task->dueDate();
            if (today.year() == date.year()) {
                if (date.weekNumber() == today.weekNumber()) {
                    return QObject::tr("This week");
                } else if (date.month() == today.month()) {
                    return QObject::tr("This month");
                } else {
                    return QDate::longMonthName(date.month());
                }
            } else {
                return date.year();
            }
        }
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> TaskModel::roleNames() const
{
    auto roles = QAbstractListModel::roleNames();
    roles.insert(TaskRole, "task");
    roles.insert(TaskPtrRole, "taskPtr");
    roles.insert(DueDateSectionRole, "dueDateSection");

    return roles;
}

int TaskModel::count() const
{
    return m_tasks.count();
}

Task::List TaskModel::tasks() const
{
    return m_tasks;
}

void TaskModel::remove(const Task::Ptr &task)
{
    const int index = m_tasks.indexOf(task);
    beginRemoveRows(QModelIndex(), index, index);
    m_tasks.removeAt(index);
    endRemoveRows();
}
