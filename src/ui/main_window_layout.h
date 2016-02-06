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

#ifndef MAINWINDOWLAYOUT_H
#define MAINWINDOWLAYOUT_H

#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QListView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStatusBar>

#include "gesture_config_panel.h"

namespace kinesix
{
    struct MenuLayout
    {
        QMenuBar menuBar;
        QMenu *file;
        QMenu *edit;
        QMenu *help;

        MenuLayout() :
            menuBar(),
            file(Q_NULLPTR),
            edit(Q_NULLPTR),
            help(Q_NULLPTR)
        {
//            file = menuBar.addMenu("&File");
//            edit = menuBar.addMenu("&Edit");
//            help = menuBar.addMenu("&Help");
//            file->addAction("Exit");
        }

        operator QWidget *()
        {
            return &menuBar;
        }

        operator QMenuBar *()
        {
            return &menuBar;
        }
    };

    struct ToolBarLayout
    {
        QToolBar toolBar;

        ToolBarLayout() :
            toolBar()
        {
            toolBar.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

            toolBar.addAction(QIcon::fromTheme("input-tablet"), "Set Active Device");
        }

        operator QWidget *()
        {
            return &toolBar;
        }

        operator const QToolBar *()
        {
            return &toolBar;
        }
    };

    struct StatusBarLayout
    {
        QStatusBar statusBar;

        StatusBarLayout() :
            statusBar()
        {
            statusBar.setSizeGripEnabled(true);
            statusBar.showMessage("No device active");
        }

        operator QWidget *()
        {
            return &statusBar;
        }

        operator const QStatusBar *()
        {
            return &statusBar;
        }
    };

    struct MainWindowLayout
    {
        QVBoxLayout mainLayout;
        QHBoxLayout contentLayout;
        MenuLayout menuLayout;
        ToolBarLayout toolBarLayout;
        StatusBarLayout statusBarLayout;
        QListView gestureListView;
        QLabel selectGestureLabel;

        MainWindowLayout(QWidget *centralWidget,
                         QAbstractListModel *gestureListModel,
                         QAbstractItemDelegate *gestureViewDelegate)
            :
              mainLayout(),
              menuLayout(),
              toolBarLayout(),
              statusBarLayout(),
              gestureListView(),
              selectGestureLabel("Select a gesture to configure"),
              m_gestureConfigPanel(Q_NULLPTR)
        {
            mainLayout.setMargin(0);
            mainLayout.setSpacing(0);
            contentLayout.setMargin(0);
            contentLayout.setSpacing(0);

            gestureListView.setItemDelegate(gestureViewDelegate);
            gestureListView.setModel(gestureListModel);
            gestureListView.setMinimumWidth(300);

            contentLayout.addWidget(&gestureListView);
            selectGestureLabel.setStyleSheet(LABEL_STYLE);
            selectGestureLabel.setAlignment(Qt::AlignCenter);
            contentLayout.addWidget(&selectGestureLabel);
            selectGestureLabel.setMaximumWidth(GESTURE_CONFIG_MAX_WIDTH);
            selectGestureLabel.setMinimumWidth(GESTURE_CONFIG_MIN_WIDTH);

            mainLayout.addWidget(menuLayout);
            mainLayout.addWidget(toolBarLayout);
            mainLayout.addLayout(&contentLayout);
            mainLayout.addWidget(statusBarLayout);

            centralWidget->setLayout(&mainLayout);
        }

        GestureConfigPanel *gestureConfigPanel()
        {
            return m_gestureConfigPanel;
        }

        void setGestureConfigPanel(GestureConfigPanel &panel)
        {
            QWidget *newPanel = static_cast<QWidget *>(panel);

            QLayoutItem *rightPanel = contentLayout.itemAt(1);
            if (rightPanel)
            {
                QWidget *oldPanel = rightPanel->widget();
                if (oldPanel == newPanel)
                    return;
                else if (oldPanel)
                {
                    contentLayout.takeAt(1);
                    oldPanel->setVisible(false);
                }
            }
            newPanel->setVisible(true);
            contentLayout.addWidget(newPanel);
            m_gestureConfigPanel = &panel;
        }

    private:
        GestureConfigPanel *m_gestureConfigPanel;
    };
}

#endif // MAINWINDOWLAYOUT_H
