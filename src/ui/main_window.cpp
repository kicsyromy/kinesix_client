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

#include "main_window.h"

#include <QEvent>
#include <QKeyEvent>
#include <QWidget>

#include "kinesixd_proxy.h"
#include "device.h"
#include "gesture_list_model.h"
#include "gesture_list_delegate.h"
#include "key_combination.h"
#include "uinput_delegate.h"
#include "x11delegate.h"
#include "active_device_selection_dialog.h"
#include "valid_device_list_model.h"
#include "device_list_delegate.h"

#include "logger.h"

using namespace kinesix;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_dbusProxy(QDBusConnection::sessionBus()),
    m_config(),
    m_inputHandler(),
    m_actions(),
    m_activeConfig(GestureType::Unknown, 0),
    ui(this,
       new GestureListModel(this),
       new GestureListDelegate(this)),
    m_activeDeviceSelectionDlg(this,
                               *(new ValidDeviceListModel(m_dbusProxy, this)),
                               *(new DeviceListDelegate(this))),
    m_snl(),
    m_closeRequested(false)
{
    connect(&m_dbusProxy, &KinesixdProxy::Swiped, this, &MainWindow::onSwiped);
    connect(&ui.gestureListView, &QListView::clicked, this, &MainWindow::onGestureClicked);
    connect(&m_inputHandler, &InputHandler::keybindFinished, this, &MainWindow::onKeybindFinished);
    connect(&m_inputHandler, &InputHandler::keyPressed, this, &MainWindow::onKeyPressed);
    connect(ui.toolBarLayout, &QToolBar::actionTriggered, m_activeDeviceSelectionDlg, &QDialog::show);
    connect(&m_activeDeviceSelectionDlg.view, &QListView::doubleClicked, this, &MainWindow::onDeviceSelected);
    connect(m_snl, &QSystemTrayIcon::activated, this, &MainWindow::onSNIActivated);
    connect(&m_snl.showAction, &QAction::triggered, this, &MainWindow::show);
    connect(&m_snl.quitAction, &QAction::triggered, this, &MainWindow::onCloseRequested);
}

MainWindow::~MainWindow()
{
    for (auto it = m_actions.begin(); it != m_actions.end(); ++it)
    {
        delete it->second;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    m_closeRequested ? event->accept() : event->ignore();
    m_closeRequested = false;
}

void MainWindow::onCloseRequested()
{
    m_closeRequested = true;
    close();
}

void MainWindow::onSNIActivated(QSystemTrayIcon::ActivationReason reason)
{
    if ((!isVisible()) && (reason == QSystemTrayIcon::Trigger))
        show();
}

void MainWindow::onDeviceSelected(const QModelIndex &index)
{
    const Device &device = index.data(DEVICE_ROLE).value<Device>();
    m_dbusProxy.SetActiveDevice(device);
    ui.statusBarLayout.statusBar.showMessage(device.name);
    m_activeDeviceSelectionDlg.mainDialog.setVisible(false);
    m_snl.setMessage(QString().sprintf("Listening for input from %s", device.name.toLatin1().constData()));
}

void MainWindow::onGestureClicked(const QModelIndex &index)
{
    GestureConfigPanel *configurationPanel = static_cast<GestureConfigPanel *>(index.data(ConfigurationPanel).value<void *>());
    m_activeConfig.first = gestureValue(index.row());

    connect(&configurationPanel->swipe3Fingers.keybindButton, &QPushButton::clicked,
            this, &MainWindow::assign3FingerKeyCombination, Qt::UniqueConnection);
    connect(&configurationPanel->swipe3Fingers.commandButton, &QPushButton::clicked,
            this, &MainWindow::assign3FingerCommand, Qt::UniqueConnection);
    connect(&configurationPanel->swipe4Fingers.keybindButton, &QPushButton::clicked,
            this, &MainWindow::assign4FingerKeyCombination, Qt::UniqueConnection);
    connect(&configurationPanel->swipe4Fingers.commandButton, &QPushButton::clicked,
            this, &MainWindow::assign4FingerCommand, Qt::UniqueConnection);
    connect(&configurationPanel->swipe5Fingers.keybindButton, &QPushButton::clicked,
            this, &MainWindow::assign5FingerKeyCombination, Qt::UniqueConnection);
    connect(&configurationPanel->swipe5Fingers.commandButton, &QPushButton::clicked,
            this, &MainWindow::assign5FingerCommand, Qt::UniqueConnection);

    ui.setGestureConfigPanel(*configurationPanel);
}

void MainWindow::onSwiped(int direction, int fingerCount)
{
    LOG_DEBUG("Caught signal Swiped with args: %s and %d", gestureToString(gestureValue(direction)), fingerCount);
    auto it = m_actions.find(std::make_pair(gestureValue(direction), fingerCount));
    if (it != m_actions.end())
        it->second->execute();
}

void MainWindow::assign3FingerKeyCombination()
{
    m_activeConfig.second = 3;
    ui.gestureConfigPanel()->swipe3Fingers.keybindEdit.setText("Waiting for input...");
    m_inputHandler.startListening(*QObject::sender());
}

void MainWindow::assign3FingerCommand()
{
}

void MainWindow::assign4FingerKeyCombination()
{
    m_activeConfig.second = 4;
    ui.gestureConfigPanel()->swipe4Fingers.keybindEdit.setText("Waiting for input...");
    m_inputHandler.startListening(*QObject::sender());
}

void MainWindow::assign4FingerCommand()
{
}

void MainWindow::assign5FingerKeyCombination()
{
    m_activeConfig.second = 5;
    ui.gestureConfigPanel()->swipe5Fingers.keybindEdit.setText("Waiting for input...");
    m_inputHandler.startListening(*QObject::sender());
}

void MainWindow::assign5FingerCommand()
{
}

void MainWindow::onKeybindFinished(const std::vector<int64_t> &keys, bool canceled)
{
    QLineEdit *keybindEdit = activeKeybindEdit();
    if (canceled)
    {
        keybindEdit->setText("None");
        return;
    }

    m_config.setKeyCombination(m_activeConfig.first, m_activeConfig.second, keybindEdit->text());

    m_actions[m_activeConfig] = new KeyCombination<UInputDelegate>(keys);
    m_activeConfig = std::make_pair(GestureType::Unknown, 0);
}

void MainWindow::onKeyPressed(const QString &keyString, bool first)
{
    QLineEdit *keybindEdit = activeKeybindEdit();
    QString keyText;
    if (first)
    {
        keybindEdit->clear();
        keyText = keyString;
    }
    else
        keyText = QString(" + ") + keyString;

    keybindEdit->setText(keybindEdit->text() + keyText);
}

QLineEdit *MainWindow::activeKeybindEdit()
{
    const int fingerCount = m_activeConfig.second;
    QLineEdit *keybindEdit = Q_NULLPTR;

    switch (fingerCount)
    {
    case 3:
        keybindEdit = &ui.gestureConfigPanel()->swipe3Fingers.keybindEdit;
        break;
    case 4:
        keybindEdit = &ui.gestureConfigPanel()->swipe4Fingers.keybindEdit;
        break;
    case 5:
        keybindEdit = &ui.gestureConfigPanel()->swipe5Fingers.keybindEdit;
        break;
    default:
        break;
    }
    return keybindEdit;
}
