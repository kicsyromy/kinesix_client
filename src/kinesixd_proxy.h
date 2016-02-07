#ifndef KINESIXD_PROXY_H
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

#define KINESIXD_PROXY_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

#include "device.h"

namespace kinesix
{

    class KinesixdProxy : public QDBusAbstractInterface
    {
        Q_OBJECT

    public:
        KinesixdProxy(const QDBusConnection &connection, QObject *parent = Q_NULLPTR);

    public slots:
        std::vector<Device> GetValidDeviceList();
        void SetActiveDevice(const Device &device);

    signals:
        void Swiped(int direction, int finger_count);
        void Pinch(int pinch_type, int finger_count);
    };
}

namespace org
{
    namespace kicsyromy
    {
        typedef ::kinesix::KinesixdProxy kinesixd;
    }
}
#endif // KINETIXD_PROXY_H
