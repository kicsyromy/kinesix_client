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

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>

#include <QObject>

namespace kinesix
{
    class InputHandler : public QObject
    {
        Q_OBJECT

    public:
        InputHandler();

    public:
        void startListening(QObject &target);

    protected:
        virtual bool eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE;

    signals:
        void keyPressed(const QString &keyString, bool first);
        void keybindFinished(const std::vector<int64_t> &keys, bool canceled = false);

    private:
        std::vector<int64_t> m_pressedKeys;
        QObject *m_target;
    };
}

#endif // INPUTHANDLER_H
