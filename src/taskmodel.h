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

#ifndef TASK_MODEL_H
#define TASK_MODEL_H

#include "task.h"
#include "tagmodel.h"

#include <QAbstractListModel>
#include <QList>

class TaskModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:    
    enum Role {
        TaskRole = TagModel::LastRole + 1,
        TaskPtrRole,
        DueDateSectionRole
    };

    explicit TaskModel(QObject *parent = nullptr);
    ~TaskModel();
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    int count() const;
    Task::List tasks() const;
    void remove(const Task::Ptr &task);

Q_SIGNALS:
    void countChanged();
    void modelChanged();

private:
    Task::List m_tasks;
};

#endif
