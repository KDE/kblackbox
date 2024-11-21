/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kbbmainwindow.h"
#include "kblackbox_version.h"

#include <KDBusService>
#include <KLocalizedString>
#include <KAboutData>
#include <QCommandLineParser>
#include <KCrash>
#define HAVE_KICONTHEME __has_include(<KIconTheme>)
#if HAVE_KICONTHEME
#include <KIconTheme>
#endif

#define HAVE_STYLE_MANAGER __has_include(<KStyleManager>)
#if HAVE_STYLE_MANAGER
#include <KStyleManager>
#endif
#include <QApplication>

int main( int argc, char **argv )
{
#if HAVE_KICONTHEME
    KIconTheme::initTheme();
#endif
    QApplication application(argc, argv);
#if HAVE_STYLE_MANAGER
    KStyleManager::initStyle();
#else // !HAVE_STYLE_MANAGER
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif // defined(Q_OS_MACOS) || defined(Q_OS_WIN)
#endif // HAVE_STYLE_MANAGER
    KLocalizedString::setApplicationDomain(QByteArrayLiteral("kblackbox"));

    KAboutData aboutData( QStringLiteral("kblackbox"),
                          i18n("KBlackBox"),
                          QStringLiteral(KBLACKBOX_VERSION_STRING),
                          i18n("Find the balls hidden in the black box by shooting laser beams!"),
                          KAboutLicense::GPL,
                          i18n("(c) 2007, Nicolas Roffet\n(c) 1999-2000, Robert Cimrman"),
                          QString(),
                          QStringLiteral("https://apps.kde.org/kblackbox"));
    aboutData.addAuthor(i18n("Nicolas Roffet"),i18n("Developer of version 0.4."), QStringLiteral("nicolas-kde@roffet.com"));
    aboutData.addAuthor(i18n("Robert Cimrman"),i18n("Original developer"), QStringLiteral("cimrman3@students.zcu.cz"));
    aboutData.addCredit(i18n("Johann Ollivier Lapeyre"), i18n("Artist"), QStringLiteral("johann.ollivierlapeyre@gmail.com"));

    KAboutData::setApplicationData(aboutData);

    KCrash::initialize();

    QCommandLineParser parser;
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
