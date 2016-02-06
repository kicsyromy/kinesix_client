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

#ifndef GESTURELISTMODEL_H
#define GESTURELISTMODEL_H

#include <QAbstractListModel>

#include "gesture.h"
#include "gesture_config_panel.h"

namespace kinesix
{
    class GestureListModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        explicit GestureListModel(QObject *parent = Q_NULLPTR);

    public:
        virtual int rowCount(const QModelIndex &) const Q_DECL_OVERRIDE;
        virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    private:
        mutable GestureConfigPanel m_configurationPanels[gestureCount];
    };
}

#endif // GESTTURELISTMODEL_H
