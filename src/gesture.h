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

#ifndef GESTURE_H
#define GESTURE_H

#include <qnamespace.h>

#include <map>

namespace kinesix
{
    enum RoleNames
    {
        Name = Qt::UserRole + 1,
        Description,
        Icon,
        ConfigurationPanel
    };

    enum class GestureType: int
    {
        Unknown = -1,
        SwipeUp = 0,
        SwipeDown,
        SwipeLeft,
        SwipeRight,
        PinchIn,
        PinchOut,
        Count
    };
    constexpr int gestureCount = static_cast<int>(GestureType::Count);
    constexpr int gestureIndex(const GestureType &value) { return static_cast<int>(value); }
    constexpr GestureType gestureValue(const int index) { return static_cast<GestureType>(index); }
    inline const char * gestureToString(const GestureType &g)
    {
        static const char *strings[] =
        {
            "Swipe Up",
            "Swipe Down",
            "Swipe Left",
            "Swipe Right",
            "Pinch In",
            "Pinch Out"
        };

        return strings[static_cast<int>(g)];
    }
}

#endif // GESTURE_H
