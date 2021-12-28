/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/




#include <KLocalizedString>

#include <KAboutData>
#include <QApplication>
#include <QCommandLineParser>
#include <Kdelibs4ConfigMigrator>
#include <KCrash>

#include "kbbmainwindow.h"
#include "kblackbox_version.h"
#include <KDBusService>


int main( int argc, char **argv )
{
    // Fixes blurry icons with fractional scaling
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    
    QApplication application(argc, argv);

    Kdelibs4ConfigMigrator migrate(QStringLiteral("kblackbox"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kblackboxrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kblackboxui.rc"));
    migrate.migrate();
    KLocalizedString::setApplicationDomain("kblackbox");
    KAboutData aboutData( QStringLiteral("kblackbox"), i18n("KBlackBox"), QStringLiteral(KBLACKBOX_VERSION_STRING), i18n("Find the balls hidden in the black box by shooting laser beams!"), KAboutLicense::GPL, i18n("(c) 2007, Nicolas Roffet\n(c) 1999-2000, Robert Cimrman"));
    aboutData.addAuthor(i18n("Nicolas Roffet"),i18n("Developer of version 0.4."), QStringLiteral("nicolas-kde@roffet.com"));
    aboutData.addAuthor(i18n("Robert Cimrman"),i18n("Original developer"), QStringLiteral("cimrman3@students.zcu.cz"));
    aboutData.addCredit(i18n("Johann Ollivier Lapeyre"), i18n("Artist"), QStringLiteral("johann.ollivierlapeyre@gmail.com"));
    aboutData.setHomepage(QStringLiteral("https://kde.org/applications/games/org.kde.kblackbox"));
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    KCrash::initialize();
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
