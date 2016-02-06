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

#include "single_application.h"
#include "main_window.h"

using kinesix::MainWindow;

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("kicsyromy");
    QApplication::setApplicationName("Kinesix");

    SingleApplication a(argc, argv);

    MainWindow w;
    w.show();

    QObject::connect(&a, &SingleApplication::activate, &w, &MainWindow::show);
    QObject::connect(&a, &SingleApplication::activate, &w, &MainWindow::raise);

    return a.exec();
}
