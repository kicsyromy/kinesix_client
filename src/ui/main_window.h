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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <utility>

#include <QWidget>
#include <QSystemTrayIcon>

#include "main_window_layout.h"
#include "active_device_selection_dialog.h"
#include "status_notifier_layout.h"
#include "configuration_client.h"
#include "kinesixd_proxy.h"
#include "gesture.h"
#include "input_handler.h"

class QPushButton;

namespace kinesix
{
    class ActionBase;

    class MainWindow : public QWidget
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = Q_NULLPTR);
        ~MainWindow();

    protected:
        virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    private slots:
        void onCloseRequested();
        void onSNIActivated(QSystemTrayIcon::ActivationReason reason);
        void onDeviceSelected(const QModelIndex &index);
        void onGestureClicked(const QModelIndex &index);
        void onSwiped(int direction, int fingerCount);
        void assign3FingerKeyCombination();
        void assign3FingerCommand();
        void assign4FingerKeyCombination();
        void assign4FingerCommand();
        void assign5FingerKeyCombination();
        void assign5FingerCommand();
        void onKeybindFinished(const std::vector<int64_t> &keys, bool canceled);
        void onKeyPressed(const QString &keyString, bool first);

    private:
        QLineEdit *activeKeybindEdit();

    private:
        KinesixdProxy m_dbusProxy;
        ConfigurationClient m_config;
        InputHandler m_inputHandler;
        std::map<std::pair<GestureType, int>, ActionBase *> m_actions;
        std::pair<GestureType, int> m_activeConfig;
        MainWindowLayout ui;
        ActiveDeviceSelectionDialog m_activeDeviceSelectionDlg;
        StatusNotifierLayout m_snl;
        bool m_closeRequested;
    };
}

#endif // MAINWINDOW_H
