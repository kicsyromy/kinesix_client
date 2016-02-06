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

#ifndef ACTIVE_DEVICE_SELECTION_DIALOG_H
#define ACTIVE_DEVICE_SELECTION_DIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QListView>
#include <QAbstractListModel>
#include <QAbstractItemDelegate>

namespace kinesix
{
    struct ActiveDeviceSelectionDialog
    {
        QDialog mainDialog;
        QHBoxLayout mainLayout;
        QLabel deviceIcon;
        QListView view;

        ActiveDeviceSelectionDialog(QWidget *parent,
                                    QAbstractListModel &deviceListModel,
                                    QAbstractItemDelegate &deviceListDelegate) :
            mainDialog(parent),
            mainLayout(),
            deviceIcon(),
            view()
        {
            deviceIcon.setPixmap(QPixmap(":/touchpad").scaledToWidth(200, Qt::SmoothTransformation));

            view.setModel(&deviceListModel);
            view.setItemDelegate(&deviceListDelegate);

            mainLayout.addWidget(&deviceIcon);
            mainLayout.addWidget(&view);
            mainDialog.setLayout(&mainLayout);
            mainDialog.setModal(true);
        }

        operator QWidget *()
        {
            return &mainDialog;
        }

        operator const QDialog *()
        {
            return &mainDialog;
        }
    };
}

#endif // ACTIVE_DEVICE_SELECTION_DIALOG_H
