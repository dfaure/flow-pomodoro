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

#ifndef FLOW_UTILS_H
#define FLOW_UTILS_H

#include "flow_export.h"

#include <qglobal.h>

class QUrl;
class QString;

namespace Utils {
    void FLOW_EXPORT openUrl(const QUrl &);
    void FLOW_EXPORT printTimeInfo(const QString &location);
    void FLOW_EXPORT keepScreenOn(bool);
    bool FLOW_EXPORT isMobile();
    bool FLOW_EXPORT isOSX();
    qreal dpiFactor();
}

#endif
