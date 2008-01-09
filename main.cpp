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



int main( int argc, char **argv )
{
	KAboutData aboutData( "kblackbox", 0, ki18n("KBlackBox"), "0.4.0", ki18n("Find the balls hidden in the black box by shooting laser beams!"), KAboutData::License_GPL, ki18n("(c) 2007, Nicolas Roffet\n(c) 1999-2000, Robert Cimrman"));
	aboutData.addAuthor(ki18n("Nicolas Roffet"),ki18n("Developper of version 0.4."), "nicolas-kde@roffet.com");
	aboutData.addAuthor(ki18n("Robert Cimrman"),ki18n("Original developer"), "cimrman3@students.zcu.cz");
	aboutData.addCredit(ki18n("Johann Ollivier Lapeyre"), ki18n("Artist"), "johann.ollivierlapeyre@gmail.com");
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
