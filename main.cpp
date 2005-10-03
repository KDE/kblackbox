//
// KBlackbox
//
// A simple game inspired by an emacs module
//
// File: main.cpp
//
// The main() function
//


#include <kapplication.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

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
    "(c) 1999-2000, Robert Cimrman");
  aboutData.addAuthor("Robert Cimrman",0, "cimrman3@students.zcu.cz");
  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication a;
  KGlobal::locale()->insertCatalog("libkdegames");

  if (a.isRestored())
      RESTORE(KBBGame)
  else {
      KBBGame *w = new KBBGame;
      a.setMainWidget(w);
      w->show();
  }
  return a.exec();
}
