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


static const char *description = I18N_NOOP("KDE Game");

/*
  The program starts here. 
*/

int main( int argc, char **argv )
{
  KCmdLineArgs::init(argc, argv, "kblackbox", description, KBVERSION);

  KApplication a;
  KBBGame v;
  
  a.setMainWidget( &v );
  debug( "Thank you for running KBlackBox, version %s.", KBVERSION );
  v.show();
  return a.exec();
}
