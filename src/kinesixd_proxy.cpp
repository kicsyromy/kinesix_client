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

#include "kinesixd_proxy.h"

using namespace kinesix;

namespace
{
    constexpr char KINESIXD_DBUS_NAME[]             = "org.kicsyromy.kinesixd";
    constexpr char KINESIXD_OBJECT_PATH[]           = "/org/kicsyromy/kinesixd";
    constexpr char KINESIXD_INTERFACE_NAME[]        = "org.kicsyromy.kinesixd";
    constexpr char KINESIXD_GET_VALID_DEVICE_LIST[] = "GetValidDeviceList";
    constexpr char KINESIXD_SET_ACTIVE_DEVICE[]     = "SetActiveDevice";
}

KinesixdProxy::KinesixdProxy(const QDBusConnection &connection, QObject *parent) :
            QDBusAbstractInterface(KINESIXD_DBUS_NAME,
                                   KINESIXD_OBJECT_PATH,
                                   KINESIXD_INTERFACE_NAME,
                                   connection,
                                   parent)
        {}

std::vector<Device> KinesixdProxy::GetValidDeviceList()
{
    QDBusMessage reply = call(QDBus::BlockWithGui, KINESIXD_GET_VALID_DEVICE_LIST);
    QDBusArgument arg = reply.arguments().at(0).value<QDBusArgument>();

    std::vector<Device> deviceList;
    arg.beginArray();
    while (!arg.atEnd())
    {
        Device d;
        arg >> d;
        deviceList.push_back(d);
    }
    arg.endArray();

    return deviceList;
}

void KinesixdProxy::SetActiveDevice(const Device &device)
{
    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(device);
    callWithArgumentList(QDBus::NoBlock, KINESIXD_SET_ACTIVE_DEVICE, argumentList);
}
