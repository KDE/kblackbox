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

#include "kbbgame.h"
#include "version.h"

/*
  The program starts here. 
*/

int main( int argc, char **argv )
{
  KApplication a(argc,argv);			
  KBBGame v;
  
  a.setMainWidget( &v );
  debug( "Thank you for running KBlackBox, version %s.", KBVERSION );
  v.show();
  return a.exec();
}
