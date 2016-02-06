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

#ifndef GESTURECONFIGPANEL_H
#define GESTURECONFIGPANEL_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>

namespace kinesix
{
    constexpr char CONTENT_STYLE[] =
    {
        "QWidget#gestureConfigContent {"
        "   background-color: white;"
        "   border-radius: 5px;"
        "   border-width: 1px;"
        "   border-style: solid;"
        "   border-color: rgba(0, 0, 0, 50);"
        "}"
    };

    constexpr char LABEL_STYLE[] =
    {
        "font-weight: bold;"
        "font-size: 16px;"
    };

    constexpr int GESTURE_CONFIG_MIN_WIDTH = 400;
    constexpr int GESTURE_CONFIG_MAX_WIDTH = 600;

    struct GestureConfigSection
    {
        QWidget mainWidget;
        QVBoxLayout mainLayout;
        QLabel titleLabel;
        QWidget content;
        QVBoxLayout contentLayout;

        QLabel keybindLabel;
        QLineEdit keybindEdit;
        QPushButton keybindButton;
        QHBoxLayout keybindLayout;

        QLabel commandLabel;
        QLineEdit commandEdit;
        QPushButton commandButton;
        QHBoxLayout commandLayout;

        GestureConfigSection(int fingerCount) :
            mainWidget(),
            mainLayout(),
            titleLabel(QString().sprintf("Configure %d finger gesture", fingerCount)),
            content(),
            contentLayout(),
            keybindLabel("Key combination:"),
            keybindEdit(),
            keybindButton("Assign"),
            keybindLayout(),
            commandLabel("Execute command:"),
            commandEdit(),
            commandButton("Browse"),
            commandLayout()
        {
            titleLabel.setStyleSheet(LABEL_STYLE);
            mainLayout.addWidget(&titleLabel);

            content.setObjectName("gestureConfigContent");
            content.setStyleSheet(CONTENT_STYLE);
            content.setFixedHeight(180);

            keybindLabel.setBuddy(&keybindEdit);
            keybindEdit.setEnabled(false);
            keybindEdit.setText("None");
            keybindLayout.addWidget(&keybindLabel);
            keybindLayout.addWidget(&keybindEdit);
            keybindLayout.addWidget(&keybindButton);

            commandLabel.setBuddy(&commandEdit);
            commandLayout.addWidget(&commandLabel);
            commandLayout.addWidget(&commandEdit);
            commandLayout.addWidget(&commandButton);

            contentLayout.addLayout(&keybindLayout);
            contentLayout.addLayout(&commandLayout);
            content.setLayout(&contentLayout);

            mainLayout.addWidget(&content);

            mainWidget.setLayout(&mainLayout);
            mainWidget.setMaximumWidth(GESTURE_CONFIG_MAX_WIDTH);
            mainWidget.setMinimumWidth(GESTURE_CONFIG_MIN_WIDTH);
        }

        operator QWidget *()
        {
            return &mainWidget;
        }
    };

    struct GestureConfigPanel
    {
        QScrollArea mainArea;
        QWidget mainWidget;
        QVBoxLayout mainLayout;
        GestureConfigSection swipe3Fingers;
        GestureConfigSection swipe4Fingers;
        GestureConfigSection swipe5Fingers;

        GestureConfigPanel() :
            mainArea(),
            mainWidget(),
            mainLayout(),
            swipe3Fingers(3),
            swipe4Fingers(4),
            swipe5Fingers(5)
        {
            mainLayout.setSpacing(10);
            mainLayout.setMargin(0);
            mainLayout.addWidget(swipe3Fingers);
            mainLayout.addWidget(swipe4Fingers);
            mainLayout.addWidget(swipe5Fingers);
            mainLayout.addStretch();
            mainLayout.setSizeConstraint(QLayout::SetMinAndMaxSize);
            mainWidget.setLayout(&mainLayout);

            mainArea.setWidget(&mainWidget);
            mainArea.setWidgetResizable(true);
            mainArea.setAlignment(Qt::AlignHCenter);
            mainArea.setMaximumWidth(GESTURE_CONFIG_MAX_WIDTH);
            mainArea.setMinimumWidth(GESTURE_CONFIG_MIN_WIDTH + 20 /* make room for the frigging scrollbar */);
            mainArea.setMinimumHeight(swipe3Fingers.mainWidget.height());
        }

        operator QWidget *()
        {
            return &mainArea;
        }
    };
}

#endif // GESTURECONFIGPANEL_H
