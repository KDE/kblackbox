//
// KBlackbox
//
// A simple game inspired by an emacs module
// 
// File: main.cpp
//
// The main() function
// 


#include <kapp.h>
#include <klocale.h>
#include <kcmdlineargs.h>

#include "kbbgame.h"
#include "version.h"
#include <kaboutdata.h>


static const char *description = I18N_NOOP("KDE Blackbox Game");

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
  KBBGame v;
  
  a.setMainWidget( &v );
  v.show();
  return a.exec();
}
