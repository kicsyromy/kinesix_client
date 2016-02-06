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

#include "configuration_client.h"
#include <QStandardPaths>

using namespace kinesix;

ConfigurationClient::ConfigurationClient() :
    m_configuration(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation), QSettings::IniFormat)
{
}

void ConfigurationClient::setCustomCommand(GestureType gesture, int fingerCount, const QString &command)
{
    m_configuration.beginGroup(gestureToString(gesture));
    m_configuration.setValue(QString().sprintf("Command/%d", fingerCount), command);
    m_configuration.endGroup();
}

void ConfigurationClient::setKeyCombination(GestureType gesture, int fingerCount, const QString &keyCombination)
{
    m_configuration.beginGroup(gestureToString(gesture));
    m_configuration.setValue(QString().sprintf("KeyCombination/%d", fingerCount), keyCombination);
    m_configuration.endGroup();
}

std::map<std::pair<GestureType, int>, ActionBase *> ConfigurationClient::readAll()
{
    return std::map<std::pair<GestureType, int>, ActionBase *>();
}
