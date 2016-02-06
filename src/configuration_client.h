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

#ifndef CONFIGURATIONCLIENT_H
#define CONFIGURATIONCLIENT_H

#include <QSettings>

#include <map>
#include <utility>

#include "gesture.h"
#include "action_base.h"

namespace kinesix
{
    class ConfigurationClient
    {
    public:
        ConfigurationClient();

    public:
        void setCustomCommand(GestureType gesture, int fingerCount, const QString &command);
        void setKeyCombination(GestureType gesture, int fingerCount, const QString &keyCombination);
        std::map<std::pair<GestureType, int>, ActionBase *> readAll();

    private:
        QSettings m_configuration;
    };
}

#endif // CONFIGURATIONCLIENT_H
