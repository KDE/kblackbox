//
// KBlackBox
//
// A simple game inspired by an emacs module
//
/***************************************************************************
 *   Copyright (c) 1999-2000, Robert Cimrman                               *
 *   cimrman3@students.zcu.cz                                              *
 *                                                                         *
 *   Copyright (c) 2007, Nicolas Roffet                                    *
 *   nicolas-kde@roffet.com                                                *
 *                                                                         *
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
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA               *
 ***************************************************************************/




#include <KLocalizedString>

#include <KAboutData>
#include <QApplication>
#include <QCommandLineParser>
#include <kdelibs4configmigrator.h>


#include "kbbmainwindow.h"
#include <KDBusService>


int main( int argc, char **argv )
{
    QApplication application(argc, argv);

    Kdelibs4ConfigMigrator migrate(QStringLiteral("kblackbox"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kblackboxrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kblackboxui.rc"));
    migrate.migrate();
    KLocalizedString::setApplicationDomain("kblackbox");
    KAboutData aboutData( QLatin1String("kblackbox"), i18n("KBlackBox"), QLatin1String("0.5.0"), i18n("Find the balls hidden in the black box by shooting laser beams!"), KAboutLicense::GPL, i18n("(c) 2007, Nicolas Roffet\n(c) 1999-2000, Robert Cimrman"));
    aboutData.addAuthor(i18n("Nicolas Roffet"),i18n("Developer of version 0.4."), QLatin1String("nicolas-kde@roffet.com"));
    aboutData.addAuthor(i18n("Robert Cimrman"),i18n("Original developer"), QLatin1String("cimrman3@students.zcu.cz"));
    aboutData.addCredit(i18n("Johann Ollivier Lapeyre"), i18n("Artist"), QLatin1String("johann.ollivierlapeyre@gmail.com"));
    aboutData.setHomepage(QLatin1String("http://games.kde.org/kblackbox"));
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(application);
    aboutData.processCommandLine(&parser);
    KDBusService service;

    application.setWindowIcon(QIcon::fromTheme(QStringLiteral("kblackbox")));


	if (application.isSessionRestored())
		kRestoreMainWindows<KBBMainWindow>();
	else {
		KBBMainWindow *mainWindow = new KBBMainWindow;
		mainWindow->show();
	}

	return application.exec();
}
