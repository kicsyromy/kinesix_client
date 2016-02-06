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

#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QMetaType>
#include <QDBusMetaType>

namespace kinesix
{
    struct Device
    {
        qint32 id;
        QString path;
        QString name;
        quint32 productId;
        quint32 vendorId;
    };
}
Q_DECLARE_METATYPE(kinesix::Device)

template <class OStream>
OStream &operator<<(OStream &stream, const kinesix::Device &out)
{
    stream << out.id;
    stream << out.path;
    stream << out.name;
    stream << out.productId;
    stream << out.vendorId;

    return stream;
}

template <class IStream>
const IStream &operator>>(const IStream &stream, kinesix::Device &in)
{
    stream >> in.id;
    stream >> in.path;
    stream >> in.name;
    stream >> in.productId;
    stream >> in.vendorId;

    return stream;
}

QDBusArgument &operator<<(QDBusArgument &arg, const kinesix::Device &out);
const QDBusArgument &operator>>(const QDBusArgument &arg, kinesix::Device &in);

#endif // DEVICE_H
