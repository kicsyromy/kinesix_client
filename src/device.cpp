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

#include "device.h"

using kinesix::Device;

namespace
{
    struct DBusDeviceRegisterHelper
    {
        DBusDeviceRegisterHelper()
        {
            qDBusRegisterMetaType<Device>();
        }
    };

    DBusDeviceRegisterHelper helper;
}

QDBusArgument &operator<<(QDBusArgument &arg, const Device &out)
{
    arg.beginStructure();
    arg << out.id;
    arg << out.path;
    arg << out.name;
    arg << out.productId;
    arg << out.vendorId;
    arg.endStructure();

    return arg;
}

const QDBusArgument &operator>>(const QDBusArgument &arg, Device &in)
{
    arg.beginStructure();
    arg >> in.id;
    arg >> in.path;
    arg >> in.name;
    arg >> in.productId;
    arg >> in.vendorId;
    arg.endStructure();

    return arg;
}
