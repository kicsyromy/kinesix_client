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

#include "gesture_list_delegate.h"

#include <QPainter>
#include <QTextDocument>

#include "gesture.h"
#include "gesture_config_panel.h"

using namespace kinesix;

GestureListDelegate::GestureListDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void GestureListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.color(QPalette::Highlight));

    QRect alignmentRect;
    QTextDocument gestureNameDescripton;

    alignmentRect = option.rect.adjusted(5, 5, -5, -5);
    QIcon gestureIcon  = QIcon(qvariant_cast<QPixmap>(index.data(Icon)));
    gestureIcon.paint(painter, alignmentRect, Qt::AlignVCenter | Qt::AlignLeft);

    alignmentRect = alignmentRect.adjusted(alignmentRect.height() + 10, 8, 0, 0);
    gestureNameDescripton.setHtml(QString().sprintf("<b>%s</b>", index.data(Name).toString().toStdString().c_str()));
    painter->save();
    painter->translate(alignmentRect.left(), alignmentRect.top());
    gestureNameDescripton.drawContents(painter);
    painter->restore();

    alignmentRect = alignmentRect.adjusted(0, 18, 0, 0);
    gestureNameDescripton.setPlainText(index.data(Description).toString());
    painter->save();
    painter->translate(alignmentRect.left(), alignmentRect.top());
    gestureNameDescripton.drawContents(painter);
    painter->restore();
}

QSize GestureListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    /* Ramdom value is random */
    return QSize(0, 70);
}
