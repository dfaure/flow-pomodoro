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

#ifndef TAGRED_MODEL_H
#define TAGRED_MODEL_H

#include "tagref.h"

#include <QAbstractListModel>
#include <QList>

class TagRefModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit TagRefModel(QObject *parent = nullptr);
    ~TagRefModel();
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    int count() const;
    bool contains(const QString &tagName) const;
    void removeAt(int index);
    TagRef at(int index) const;
    int indexOfTag(const QString &name) const;
    void append(const TagRef &);
    bool isEmpty() const;
    void clear();
    bool equalTags(TagRefModel *other) const;

Q_SIGNALS:
    void countChanged();
    void modelChanged();

private:
    TagRef::List m_tagRefs;
};

#endif
