/***************************************************************************
 *   Copyright (C) 2009 by Kai Dombrowe <just89@gmx.de>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

// own
#include "application.h"
#include "mainwindow.h"


// KDE
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kdebug.h>



int main(int argc, char *argv[])
{

    KAboutData about("recorditnow",
                     0,
                     ki18n("RecordItNow"),
                     "0.8",
                     ki18n("Plugin based desktop recorder"),
                     KAboutData::License_GPL,
                     ki18n("(C) 2009 Kai Dombrowe"),
                     KLocalizedString(),
                     0,
                     "just89@gmx.de");
    about.setProgramIconName("recorditnow");
    about.setHomepage("http://recorditnow.sourceforge.net/index.html");

    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("x <offset>", ki18n("Offset in x direction."));
    options.add("y <offset>", ki18n("Offset in y direction."));
    options.add("width <width>", ki18n("Width of recorded window."));
    options.add("height <height>", ki18n("Height of recorded window."));
    options.add("backend <backend>", ki18n("The Backend to use. (Example: RecordMyDesktop/Screenshot)"));
    options.add("timer <time>", ki18n("Wait \"time\" seconds."));
    options.add("o filename", ki18n("Name of recorded video/image."));
    KCmdLineArgs::addCmdLineOptions(options);

    Application app;

    if (app.isSessionRestored()) { // see if we are starting with session management
        RESTORE(MainWindow);
    } else { // no session.. just start up normally        
        KCmdLineArgs *parsed = KCmdLineArgs::parsedArgs();

        bool hasArgs = false;
        QRect geometry = QRect(0, 0, 100, 100);
        QString backend;
        QString file;
        int time = 0;


        if (parsed->isSet("x")) {
            hasArgs = true;
            geometry.setX(parsed->getOption("x").toInt());
        }
        if (parsed->isSet("y")) {
            hasArgs = true;
            geometry.setY(parsed->getOption("y").toInt());
        }
        if (parsed->isSet("width")) {
            hasArgs = true;
            geometry.setWidth(parsed->getOption("width").toInt());
        }
        if (parsed->isSet("height")) {
            hasArgs = true;
            geometry.setHeight(parsed->getOption("height").toInt());
        }
        if (parsed->isSet("backend")) {
            hasArgs = true;
            backend = parsed->getOption("backend");
        }
        if (parsed->isSet("timer")) {
            hasArgs = true;
            time = parsed->getOption("timer").toInt();
        }
        if (parsed->isSet("o")) {
            hasArgs = true;
            file = parsed->getOption("o");
        }

        MainWindow *window = new MainWindow;
        window->show();
        parsed->clear();

        if (hasArgs) {
            window->startWithArgs(backend, file, time, geometry);
        }
    }

    return app.exec();

}
