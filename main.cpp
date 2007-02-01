//
// KBlackbox
//
// A simple game inspired by an emacs module
//
// The main() function
//


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
      RESTORE(KBBGame)
  else {
      KBBGame *game = new KBBGame;
      game->show();
  }
  return application.exec();
}
