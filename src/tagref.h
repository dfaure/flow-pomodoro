/*
  This file is part of Flow.

  Copyright (C) 2014 Sérgio Martins <iamsergio@gmail.com>

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

#ifndef TAGREF_H
#define TAGREF_H

#include "tag.h"

#include <QPointer>
#include <vector>

class Task;
class Storage;

class TagRef
{
public:
    typedef std::vector<TagRef> List; // std::vector calls less CTORs than QVector when appending

    TagRef(const TagRef &other);
    TagRef operator=(const TagRef &);
    TagRef(const QPointer<Task> &task,
           const QString &tagName, Storage *storage,
           bool temporary = false);
    ~TagRef();

    Tag::Ptr tag() const;
    Storage *storage() const;
    void setStorage(Storage *);
    QString tagName() const;

    QPointer<Task> m_task;
private:
    TagRef();
    void incrementCount();
    void decrementCount();
    QString m_tagName;
    bool m_temporary;
    Storage *m_storage;
    Tag::Ptr m_tag;
};

#endif
