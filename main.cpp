//
// KBlackbox
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

#include "kbbgame.h"
#include "version.h"


static const char description[] = I18N_NOOP("KDE Blackbox Game");

/*
  The program starts here.
*/

int main( int argc, char **argv )
{
  KAboutData aboutData( "kblackbox", I18N_NOOP("KBlackBox"),
    KBVERSION, description, KAboutData::License_GPL,
    "(c) 1999-2000, Robert Cimrman\n(c) 2007, Nicolas Roffet");
  aboutData.addAuthor("Robert Cimrman",0, "cimrman3@students.zcu.cz");
  aboutData.addAuthor("Nicolas Roffet",0, "nicolas-kde@roffet.com");
  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication application;
  KGlobal::locale()->insertCatalog("libkdegames");

  if (application.isSessionRestored())
      kRestoreMainWindows<KBBGame>();
  else {
      KBBGame *game = new KBBGame;
      game->show();
  }

  return application.exec();
}
