#ifndef STATUS_NOTIFIER_LAYOUT_H
#define STATUS_NOTIFIER_LAYOUT_H

#include <QSystemTrayIcon>
#include <QMenu>

namespace kinesix
{
    struct StatusNotifierLayout
    {
        QSystemTrayIcon statusNotifierItem;
        QMenu mainMenu;
        QAction &showAction;
        QAction &quitAction;

        StatusNotifierLayout() :
            statusNotifierItem(QIcon::fromTheme("input-tablet")),
            mainMenu(),
            showAction(*(mainMenu.addAction("Show"))),
            quitAction(*(mainMenu.addAction("Quit")))
        {
            statusNotifierItem.setContextMenu(&mainMenu);
            statusNotifierItem.show();
        }

        void setMessage(const QString &message)
        {
            statusNotifierItem.setToolTip(message);
        }

        operator QSystemTrayIcon *()
        {
            return &statusNotifierItem;
        }

        operator QObject *()
        {
            return &statusNotifierItem;
        }
    };
}

#endif // STATUS_NOTIFIER_LAYOUT_H
