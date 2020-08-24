//Copyright (C) 2020  Illotros GmbH

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QMetaType>
#include <QTranslator>
#include <QString>

extern "C"
{
    #include "sodium.h"
}

int main(int argc, char *argv[])
{
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized, it is not safe to use */
        return 0;
    }
    int currentExitCode = 0;

    do {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor);
        QApplication a(argc, argv);
        a.setOrganizationName("Illotros");
        a.setOrganizationDomain("illotros.ch");
        a.setApplicationName("SnowHaze VPN");

        QTranslator translator;
        QLocale local;
        QString language = local.name().split("_")[0];
        translator.load(QString(":/translations/translations/shvpn_") + language);
        a.installTranslator(&translator);

        MainWindow w;
        w.show();
        w.setWindowTitle("SnowHaze VPN");
        currentExitCode = a.exec();
    } while( currentExitCode == MainWindow::EXIT_CODE_REBOOT);
    return currentExitCode;
}
