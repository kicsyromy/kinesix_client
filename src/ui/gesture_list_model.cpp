/*
 * Copyright © 2016 Romeo Calota
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Romeo Calota
 */

#include "gesture_list_model.h"

#include <QIcon>

#include "kinesixd_proxy.h"

#include "logger.h"

using namespace kinesix;

namespace
{
    const char GESTURE_ICON_PREFIX[] = ":/icons";

    const char *GESTURE_ICON_NAMES[] =
    {
        "swipe-up",
        "swipe-down",
        "swipe-left",
        "swipe-right",
        "pinch-in",
        "pinch-out",
        "fingers"
    };
}

GestureListModel::GestureListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_configurationPanels()
{
}

int GestureListModel::rowCount(const QModelIndex &) const
{
    return gestureCount;
}

QVariant GestureListModel::data(const QModelIndex &index, int role) const
{
    QVariant retVal;

    switch (role)
    {
    default:
        break;
    case Name:
        retVal = gestureToString(gestureValue(index.row()));
        break;
    case Description:
        retVal = QString("some awesome description of gesture");
        break;
    case Icon:
        retVal = QPixmap(QString().sprintf("%s/%s", GESTURE_ICON_PREFIX, GESTURE_ICON_NAMES[index.row()]));
        break;
    case ConfigurationPanel:
        retVal = QVariant::fromValue(static_cast<void *>(&m_configurationPanels[index.row()]));
        break;
    }

    return retVal;
}
