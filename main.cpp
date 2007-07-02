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



#include <kapplication.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kglobal.h>


#include "kbbmainwindow.h"



static const char KBBDESCRIPTION[] = I18N_NOOP("KDE Black Box Game: Find the hidden balls in the black box");
static const char KBBVERSION[] = "0.4.0-SVN";


int main( int argc, char **argv )
{
  KAboutData aboutData( "kblackbox", 0, ki18n("KBlackBox"),
    KBBVERSION, ki18n(KBBDESCRIPTION), KAboutData::License_GPL,
    ki18n("(c) 1999-2000, Robert Cimrman\n(c) 2007, Nicolas Roffet"));
  aboutData.addAuthor(ki18n("Robert Cimrman"),KLocalizedString(), "cimrman3@students.zcu.cz");
  aboutData.addAuthor(ki18n("Nicolas Roffet"),KLocalizedString(), "nicolas-kde@roffet.com");
  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication application;
  KGlobal::locale()->insertCatalog("libkdegames");

  if (application.isSessionRestored())
      kRestoreMainWindows<KBBMainWindow>();
  else {
      KBBMainWindow *mainWindow = new KBBMainWindow;
      mainWindow->show();
  }

  return application.exec();
}
