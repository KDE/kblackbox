//
// KBlackbox
//
// A simple game inspired by an emacs module
// 
// File: kbbgame.cpp
//
// The implementation of the KBBGame widget
// 

#include <config.h>

#include <qpopupmenu.h>
#include <qkeycode.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtooltip.h> 
#include <qstring.h>
#include <qmessagebox.h> 
#include <stdlib.h>
#include <time.h>

#include <kapp.h>
#include <ktopwidget.h>
#include <kmenubar.h>
#include <kiconloader.h>
#include <klocale.h>
#define trans klocale   

#include "kbbgame.h"
#include "util.h"
#include "version.h"

/*
  Names of pixmap files.
*/

const char *pFNames[NROFTYPES] = {
  "white.xpm",
  "gray.xpm",
  "green.xpm",
  "red.xpm",
  "blue.xpm",
  "cyan.xpm",
  "brown.xpm",
  "green2.xpm"
};

/*
   Creates the KBBGame widget and sets saved options (if any).
*/

KBBGame::KBBGame() : KTopLevelWidget()     
{
  int i;

  setCaption(QString("KBlackBox ")+KBVERSION);

  menu                = new KMenuBar(this, "menu");
  QPopupMenu *game    = new QPopupMenu;
  QPopupMenu *file    = new QPopupMenu;
  sizesm  = new QPopupMenu;
  ballsm  = new QPopupMenu;
  QPopupMenu *help = new QPopupMenu;
  options = new QPopupMenu;
 
  CHECK_PTR( file );
  CHECK_PTR( game );
  CHECK_PTR( help );
  CHECK_PTR( sizesm );
  CHECK_PTR( ballsm );
  CHECK_PTR( options );
  CHECK_PTR( menu );

  help->insertItem( i18n("&Help"), ID_HELP );
  help->setAccel( CTRL+Key_H, ID_HELP );
  help->insertSeparator();
  help->insertItem( i18n("&About KBlackBox"), ID_ABOUT );
  help->setAccel( CTRL+Key_A, ID_ABOUT );
  help->insertItem( i18n("About &Qt"), ID_ABOUT_QT );

		    
  file->insertItem( i18n("&Quit"), ID_QUIT );
  file->setAccel( CTRL+Key_Q, ID_QUIT );

  game->insertItem( i18n("&New"), ID_NEW );
  game->insertItem( i18n("&Give up"), ID_GIVEUP );
  game->insertItem( i18n("&Done"), ID_DONE );
  game->insertSeparator();
  game->insertItem( i18n("&Resize"), ID_RESIZE );

  sizes1id = sizesm->insertItem( "  8 x 8  ", this, SLOT(size1()) );
  sizes2id = sizesm->insertItem( " 10 x 10 ", this, SLOT(size2()) );
  sizes3id = sizesm->insertItem( " 12 x 12 ", this, SLOT(size3()) );
  sizesm->setCheckable( TRUE );

  balls1id = ballsm->insertItem( " 4 ", this, SLOT(balls1()) );
  balls2id = ballsm->insertItem( " 6 ", this, SLOT(balls2()) );
  balls3id = ballsm->insertItem( " 8 ", this, SLOT(balls3()) );
  ballsm->setCheckable( TRUE );

  options->insertItem( i18n("&Size"), sizesm );  
  options->insertItem( i18n("&Balls"), ballsm );
  tut1id = options->insertItem( i18n("&Tutorial"),
				this, SLOT(tutorialSwitch()) );
  options->setCheckable( TRUE );

  connect( file, SIGNAL(activated(int)), SLOT(callBack(int)) );
  connect( help, SIGNAL(activated(int)), SLOT(callBack(int)) );
  connect( game, SIGNAL(activated(int)), SLOT(callBack(int)) );
 
  menu->insertItem( i18n("&File"), file );
  menu->insertItem( i18n("&Game"), game );
  menu->insertItem( i18n("&Options"), options );
  menu->insertSeparator();
  menu->insertItem( i18n("&Help"), help );

  menu->show(); 
  setMenu( menu );

  KIconLoader *loader = kapp->getIconLoader();
  QPixmap **pix = new (QPixmap *) [NROFTYPES];
  pix[0] = new QPixmap();
  *pix[0] = loader->loadIcon( pFNames[0] );
  if (!pix[0]->isNull()) {
    debug( "Pixmap \"%s\" loaded.", pFNames[0] );
    for (i = 1; i < NROFTYPES; i++) {
      pix[i] = new QPixmap;
      *pix[i] = loader->loadIcon( pFNames[i] );
      if (!pix[i]->isNull()) {
	debug( "Pixmap \"%s\" loaded.", pFNames[i] );
      } else {
	pix[i] = pix[i-1];
	pix[i]->detach();
	debug( "Cannot find pixmap \"%s\". Using previous one.", pFNames[i] );
      }
    }
  } else {
    debug( "Cannot find pixmap \"%s\". Pixmaps will not be loaded.",
	   pFNames[0] );
    delete pix[0];
    delete pix;
    pix = NULL;
  }
  gr = new KBBGraphic( pix, this );
  connect( gr, SIGNAL(inputAt(int,int,int)),
	  this, SLOT(gotInputAt(int,int,int)) );
  connect( this, SIGNAL(gameRuns(bool)),
	  gr, SLOT(setInputAccepted(bool)) );
  connect( gr, SIGNAL(endMouseClicked()),
	  this, SLOT(gameFinished()) );
  
  /*
  QToolTip::add( doneButton, i18n(
		 "Click here when you think you placed all the balls.") );
		 */

  QString tmps;
  stat = new KStatusBar( this );
  tmps.sprintf( "aaaaaaaa" );
  stat->insertItem( (const char *) tmps, SSCORE );
  tmps.sprintf( "aaaaaaaaaaa" );
  stat->insertItem( (const char *) tmps, SBALLS );
  tmps.sprintf( "aaaaaaa" );
  stat->insertItem( (const char *) tmps, SRUN );
  tmps.sprintf( "aaaaaaaaaa" );
  stat->insertItem( (const char *) tmps, SSIZE );
  setStatusBar( stat );

  tool = new KToolBar( this );
  tool->insertButton( loader->loadIcon("exit.xpm"),
		      ID_QUIT, TRUE, i18n("Quit") );
  tool->insertButton( loader->loadIcon("reload.xpm"),
		      ID_NEW, TRUE, i18n("New") );
  tool->insertButton( loader->loadIcon("giveup.xpm"),
		      ID_GIVEUP, TRUE, i18n("Give up") );
  tool->insertButton( loader->loadIcon("done.xpm"),
		      ID_DONE, TRUE, i18n("Done") );
  tool->insertSeparator();
  tool->insertButton( loader->loadIcon("help.xpm"), ID_HELP, TRUE,
		      i18n("Help") );
  connect( tool, SIGNAL(clicked(int)), SLOT(callBack(int)) );
  addToolBar( tool );
  tool->setBarPos( KToolBar::Top );
  tool->show();
  
  /*
     Game initializations
  */
  running = FALSE;
  gameBoard = NULL;

  KConfig *kConf;
  int j;
  kConf = KApplication::getKApplication()->getConfig();
  kConf->setGroup( "KBlackBox Setup" );
  if (kConf->hasKey( "Balls" )) {
    i = kConf->readNumEntry( "Balls" );
    balls = i;
    switch (i) {
    case 4: ballsm->setItemChecked( balls1id, TRUE ); break;
    case 6: ballsm->setItemChecked( balls2id, TRUE ); break;
    case 8: ballsm->setItemChecked( balls3id, TRUE ); break;
    }
  } else {
    balls = 4;
    ballsm->setItemChecked( balls1id, TRUE );
  }
  if ((kConf->hasKey( "Width" )) &&
      (kConf->hasKey( "Balls" ))) {
    i = kConf->readNumEntry( "Width" );
    j = kConf->readNumEntry( "Height" );
    gr->setSize( i+4, j+4 ); // +4 is the space for "lasers" and an edge...
    gameBoard = new RectOnArray( gr->numC(), gr->numR() );
    switch (i) {
    case 8: sizesm->setItemChecked( sizes1id, TRUE ); break;
    case 10: sizesm->setItemChecked( sizes2id, TRUE ); break;
    case 12: sizesm->setItemChecked( sizes3id, TRUE ); break;
    }
  } else {
    gr->setSize( 8+4, 8+4 ); // +4 is the space for "lasers" and an edge...
    gameBoard = new RectOnArray( gr->numC(), gr->numR() );
    sizesm->setItemChecked( sizes1id, TRUE );
  }
  if (kConf->hasKey( "tutorial" )) {
    tutorial = (bool) kConf->readNumEntry( "tutorial" );
  } else tutorial = FALSE;
  options->setItemChecked( tut1id, tutorial );

  QString s, s1;
  int pos;
  setMinSize();
  gameResize();
  if (kConf->hasKey( "appsize" )) {
    s = kConf->readEntry( "appsize" );
    debug("%s", (const char *) s);
    pos = s.find( 'x' );
    s1 = s.right( s.length() - pos - 1 );
    s.truncate( pos - 1 );
    //    debug("%s", (const char *) s);
    //    debug("%s", (const char *) s1);
    resize( s.toInt(), s1.toInt() );
  }

  setScore( 0 );
  ballsPlaced = 0;
  updateStats();

  setView( gr );

  newGame();
}

/*
   Saves the options and destroys the KBBGame widget. 
*/

KBBGame::~KBBGame()
{
  KConfig *kConf;
  QString s;

  kConf = KApplication::getKApplication()->getConfig();
  kConf->setGroup( "KBlackBox Setup" );
  kConf->writeEntry( "Balls", balls );
  kConf->writeEntry( "Width", gr->numC() - 4);
  kConf->writeEntry( "Height", gr->numR() - 4);
  kConf->writeEntry( "tutorial", (int) tutorial );
  s.sprintf( "%d x %d", this->width(), this->height() );
  kConf->writeEntry( "appsize", s );
  
  delete gr;
  if (gameBoard != NULL) delete gameBoard;
  delete stat;
  delete tool;
}

/*
 * GUI callbacks.
 */
void KBBGame::callBack( int witch )
{
  switch (witch) {
  case ID_HELP:
    help();
    break;
  case ID_ABOUT:
    about();
    break;
  case ID_ABOUT_QT:
    aboutQt();
    break;
  case ID_QUIT:
    kapp->quit();
    break;
  case ID_NEW:
    newGame();
    break;
  case ID_GIVEUP:
    giveUp();
    break;
  case ID_DONE:
    gameFinished();
    break;
  case ID_RESIZE:
    setMinSize();
    gameResize();
    break;
  }
}

/*
   Resize event of the KBBGame widget.
*/

void KBBGame::resizeEvent( QResizeEvent *e )
{
  KTopLevelWidget::resizeEvent(e);
}


/*
   Resizes yourself to fit the contents perfectly, from menu.
*/

void KBBGame::gameResize()
{
  resize( gr->wHint(), gr->hHint() + menu->height() + stat->height() +
      tool->height() );
}

void KBBGame::setMinSize()
{
  setMinimumSize( gr->wHint(), gr->hHint() + menu->height() + stat->height() +
      tool->height() );
}

/*
   Settings of various options.
*/

void KBBGame::size1() 
{ 
  if (setSize( 8, 8 )) {
    sizesm->setItemChecked( sizes1id, TRUE );
    sizesm->setItemChecked( sizes2id, FALSE );
    sizesm->setItemChecked( sizes3id, FALSE );
  }
}

void KBBGame::size2()
{
  if (setSize( 10, 10 )) {
    sizesm->setItemChecked( sizes2id, TRUE );
    sizesm->setItemChecked( sizes1id, FALSE );
    sizesm->setItemChecked( sizes3id, FALSE );
  }
}

void KBBGame::size3()
{ 
  if (setSize( 12, 12 )) {
    sizesm->setItemChecked( sizes3id, TRUE );
    sizesm->setItemChecked( sizes2id, FALSE );
    sizesm->setItemChecked( sizes1id, FALSE );
  }
}

void KBBGame::balls1()
{ 
  if (setBalls( 4 )) {
    ballsm->setItemChecked( balls1id, TRUE );
    ballsm->setItemChecked( balls2id, FALSE );
    ballsm->setItemChecked( balls3id, FALSE );
  }
}

void KBBGame::balls2()
{ 
  if (setBalls( 6 )) {
    ballsm->setItemChecked( balls2id, TRUE );
    ballsm->setItemChecked( balls1id, FALSE );
    ballsm->setItemChecked( balls3id, FALSE );
  }
}

void KBBGame::balls3()
{ 
  if (setBalls( 8 )) {
    ballsm->setItemChecked( balls3id, TRUE );
    ballsm->setItemChecked( balls2id, FALSE );
    ballsm->setItemChecked( balls1id, FALSE );
  }
}

void KBBGame::tutorialSwitch()
{
  tutorial = !tutorial;
  options->setItemChecked( tut1id, tutorial );
}

/*
  Display various infos.
*/

void KBBGame::about()
{
  QMessageBox::information( 0, i18n("About"), 
			    i18n(
            "KBlackBox logical game\nauthor: Robert Cimrman\ne-mail: cimrman3@students.zcu.cz"), i18n("OK") );
}

void KBBGame::aboutQt()
{
  QMessageBox::aboutQt( 0, i18n("Qt information") );
}

void KBBGame::help()
{
  //  QMessageBox::message( "Help", "Read the file \"help.txt\", please.", "OK" );
  KApplication::getKApplication()->invokeHTMLHelp("", "");
}

/*
   Creates a new game.
*/

void KBBGame::newGame() 
{
  int i, j;

  if (running) {
    bool ok;
    ok = QMessageBox::warning(0, i18n("Warning!"), 
			    i18n(
			    "Do you really want to give up this game?"), 
			    i18n("Yes, I'm burnt out."),
			    i18n("No, not yet...") );
    if (!ok) {
      abortGame();
    } else return;
  }

  gameBoard->fill( INNERBBT );
  for (j = 0; j < (gr->numR()); j++) {
    gameBoard->set( 0, j, OUTERBBT );
    gameBoard->set( gr->numC()-1, j, OUTERBBT );
  }
  for (i = 0; i < (gr->numC()); i++) {
    gameBoard->set( i, 0, OUTERBBT );
    gameBoard->set( i, gr->numR()-1, OUTERBBT );
  }
  for (j = 2; j < (gr->numR()-2); j++) {
    gameBoard->set( 1, j, LASERBBT );
    gameBoard->set( gr->numC()-2, j, LASERBBT );
  }
  for (i = 2; i < (gr->numC()-2); i++) {
    gameBoard->set( i, 1, LASERBBT );
    gameBoard->set( i, gr->numR()-2, LASERBBT );
  }
  gameBoard->set( 1, 1, OUTERBBT );
  gameBoard->set( 1, gr->numR()-2, OUTERBBT );
  gameBoard->set( gr->numC()-2, 1, OUTERBBT );
  gameBoard->set( gr->numC()-2, gr->numR()-2, OUTERBBT );

  randomBalls( balls );
  remap( gameBoard, gr->getGraphicBoard() );
  gr->repaint( TRUE );
  setScore( 0 );
  detourCounter = -1;
  ballsPlaced = 0;
  running = TRUE;
  updateStats();
  emit gameRuns( running );
}

/*
   Ends the current game.
*/

void KBBGame::gameFinished()
{
  if (running) {
    QMessageBox mb;
    QString s;
    if (ballsPlaced == balls) {
      getResults();
      abortGame();
      s = i18n("Your final score is: %1.").arg(score);
      if (score <= (balls*3))
	mb.setButtonText( QMessageBox::Ok, i18n("Wow!") );
      else
	mb.setButtonText( QMessageBox::Ok, i18n("Damned!") );
    } else {
      s = i18n( "You should place %1 balls!\nYou have placed %1.").arg(balls).
	arg(ballsPlaced);
      mb.setButtonText( QMessageBox::Ok, i18n("D'accord") );
    }
    mb.setText( s );
    mb.show();  
  }
}

/*
   Computes the final score and indicate errors.
*/

void KBBGame::getResults()
{
  int i, j, tgam, tgra;
  RectOnArray *r = gr->getGraphicBoard();
  for (j = 0; j < (gr->numR()); j++) {
    for (i = 0; i < (gr->numC()); i++) {
      tgam = gameBoard->get( i, j );
      tgra = r->get( i, j );
      if ((tgam == BALLBBT) && (tgra != TBALLBBG)) {
	setScore( score+5 );
	r->set( i, j, WBALLBBG );
	gr->updateElement( i, j );
      }
      if ((tgam != BALLBBT) && (tgra == TBALLBBG)) {
	r->set( i, j, FBALLBBG );
	gr->updateElement( i, j );
      }
    }
  }
}

/*
   Aborts the current game.
*/

void KBBGame::abortGame()
{
  if (running) {
    running = FALSE;
    ballsPlaced = 0;
    updateStats();
    gr->clearFocus();
    emit gameRuns( running );
  }
}

/*
   Gives the game up.
*/

void KBBGame::giveUp()
{
  if (running) {
    bool ok;
    ok = QMessageBox::warning(0, i18n("Warning!"), 
			    i18n(
			    "Do you really want to give up this game?"), 
			    i18n("Yes, I'm burnt out."),
			    i18n("No, not yet...") );
    if (!ok) {
      getResults();
      abortGame();
    }
  }
}

/*
   Displays game statistics.
*/

void KBBGame::updateStats()
{
  QString s = i18n("Run: ");
  if (running)
    s += i18n("Yes");
  else
    s += i18n("No");
  stat->changeItem( (const char *) s, SRUN );
  s.sprintf( "%s: %2d x %2d", i18n( "Size" ).ascii(),
	       gr->numC()-4, gr->numR()-4 );
  stat->changeItem( (const char *) s, SSIZE );
  s.sprintf( "%s: %2d / %2d", i18n("Placed").ascii(),
	     ballsPlaced, balls );
  stat->changeItem( (const char *) s, SBALLS );
}

/*
   Sets the score value to n.
*/

void KBBGame::setScore( int n )
{
  score = n;
  QString s;
  s.sprintf( "%s: %d", i18n("Score").ascii(), n );
  stat->changeItem( (const char *) s, SSCORE );
}

/*
   Sets the size of the black box.
*/

bool KBBGame::setSize( int w, int h )
{
  bool ok = FALSE;
  if (((w+4) != gr->numC()) || ((h+4) != gr->numR())) {
    if (running) {
      ok = !QMessageBox::warning(0, i18n("Warning!"), 
			      i18n(
			      "This will be the end of the current game!"), 
			      i18n("Never mind..."),
			      i18n("Oh, no!") );
    } else ok = TRUE;
    if (ok) {
      gr->setSize( w+4, h+4 ); // +4 is the space for "lasers" and an edge...
      setMinSize();
      gameResize();
      if (gameBoard != NULL) delete gameBoard;
      gameBoard = new RectOnArray( gr->numC(), gr->numR() );
      if (running) abortGame();
      newGame();
      //      gr->repaint( TRUE );
    }
  }
  return ok;
}

/*
   Sets the number of balls in the black box to n.
*/

bool KBBGame::setBalls( int n )
{
  bool ok = FALSE;
  if (balls != n) {
    if (running) {
      ok = !QMessageBox::warning(0, i18n("Warning!"), 
			      i18n(
			      "This will be the end of the current game!"), 
			      i18n("Never mind..."),
			      i18n("Oh, no!") );
    } else ok = TRUE;
    if (ok) {
      balls = n;
      if (running) abortGame();
      newGame();
    }
  }
  return ok;
}

/*
   Puts n balls in the black box on random positions.
*/

void KBBGame::randomBalls( int n )
{
  int i;
  for (i = 0; i < n; i++) {
    srandom( (int) time( NULL ) );
    int x=0, y=0; // there is OUTERBBT...
    while (gameBoard->get( x, y ) != INNERBBT ) {
      x =  2 + random() % gameBoard->width() - 3;
      y =  2 + random() % gameBoard->height() - 3;
    }
    gameBoard->set( x, y, BALLBBT );
  }
}

/*
   This is, in fact, the whole game...
*/

int KBBGame::traceRay( int startX, int startY, int *endX, int *endY )
{
  int type, x, y, d, refl;
  int slx, scx, srx, sly, scy, sry;
  bool directionChanged;
  *endX = x = startX;
  *endY = y = startY;
  /*
    Just to avoid compiler warnings
  */
  type = slx = scx = srx = sly = scy = sry = 0;
  /*
     Get the initial direction d.
     0 .. up, 1 .. right, 2 .. down, 3 .. left
     (0,0) is the upper-left corner.
  */
  if ((gameBoard->get( x, y-1 ) == INNERBBT) ||
      (gameBoard->get( x, y-1 ) == BALLBBT)) { d = 0; }
  else if ((gameBoard->get( x+1, y ) == INNERBBT) ||
	   (gameBoard->get( x+1, y ) == BALLBBT)) { d = 1; }
  else if ((gameBoard->get( x, y+1 ) == INNERBBT) ||
	   (gameBoard->get( x, y+1 ) == BALLBBT)) { d = 2; }
  else if ((gameBoard->get( x-1, y ) == INNERBBT) ||
	   (gameBoard->get( x-1, y ) == BALLBBT)) { d = 3; }
  else return WRONGSTART;
  /*
     And now trace the ray.
  */
  while (1) {
    switch (d) {
    case 0:
      slx = -1; scx = 0; srx = 1;
      sly = -1; scy = -1; sry = -1;
      break;
    case 1:
      slx = 1; scx = 1; srx = 1;
      sly = -1; scy = 0; sry = 1;
      break;
    case 2:
      slx = 1; scx = 0; srx = -1;
      sly = 1; scy = 1; sry = 1;
      break;
    case 3:
      slx = -1; scx = -1; srx = -1;
      sly = 1; scy = 0; sry = -1;
      break;
    }
    directionChanged = FALSE;
    if (gameBoard->get( x+scx, y+scy ) == LASERBBT) {
      type = DETOUR;
      *endX = x+scx;
      *endY = y+scy;
      break;
    }
    if (gameBoard->get( x+scx, y+scy ) == BALLBBT) {
      type = HIT;
      break;
    }
    refl = 0;
    if (gameBoard->get( x+slx, y+sly ) == BALLBBT) {
      type = REFLECTION;
      if (gameBoard->get( x, y ) == LASERBBT) break;
      directionChanged = TRUE;
      refl += 1;
    }
    if (gameBoard->get( x+srx, y+sry ) == BALLBBT) {
      type = REFLECTION;
      if (gameBoard->get( x, y ) == LASERBBT) break;
      directionChanged = TRUE;
      refl +=2;
    }
    // turn to the right
    if (refl == 1) d = (d + 1) % 4;
    // turn to the left
    if (refl == 2) if ((d -= 1) < 0) d += 4; 
    // turn back -- no need to trace again the same way
    if (refl == 3) break;
    if (!directionChanged) {
      x += scx;
      y += scy;
    }
  }
  return type;
}

/*
   Remaps the gameBoard to its graphic representation.
*/

void KBBGame::remap( RectOnArray *gam, RectOnArray *gra )
{
  int i, j;
  for (j = 0; j < (gam->height()); j++) {
    for (i = 0; i < (gam->width()); i++) {
      switch (gam->get( i,j )) {
      case BALLBBT: if (tutorial) { gra->set( i,j, WBALLBBG ); break; }
      case INNERBBT: gra->set( i,j, INNERBBG ); break;
      case OUTERBBT: gra->set( i,j, OUTERBBG ); break;
      case LASERBBT: gra->set( i,j, LASERBBG ); break;
      default: gra->set( i,j, OUTERBBG );
      }
    }
  }
}

/*
   Processes the user input.
*/

void KBBGame::gotInputAt( int col, int row, int state )
{
  RectOnArray *r = gr->getGraphicBoard();
  int type = r->get( col, row );
  int x, y;
  int ex, ey;
  int w = gameBoard->width() - 2;
  int h = gameBoard->height() - 2;

  if (state & LeftButton) {
    switch (type) {
    case WBALLBBG: // because of the tutorial mode
    case INNERBBG:
      r->set( col, row, TBALLBBG ); 
      ballsPlaced++;
      break;
    case MARK1BBG:
      r->set( col, row, INNERBBG );
      break;
    case TBALLBBG:
      r->set( col, row, INNERBBG );
      ballsPlaced--;
      break;
    case LASERBBG:
      int endX, endY, result;
      result = traceRay( col, row, &endX, &endY );
      r->set( col, row, LFIREBBG );
      //debug( "%d %d", endX, endY ); 
      if (col == 1) x = 0; else
      if (col == w) x = w + 1;
      else x = col;

      if (row == 1) y = 0; else
      if (row == h) y = h + 1;
      else y = row;

      switch (result) {
      case DETOUR:
	r->set( endX, endY, LFIREBBG );
	r->set( x, y, detourCounter );
	if (endX == 1) ex = 0; else
	  if (endX == w) ex = w + 1;
	  else ex = endX;
	if (endY == 1) ey = 0; else
	  if (endY == h) ey = h + 1;
	  else ey = endY;
	r->set( ex, ey, detourCounter-- );
	gr->updateElement( x, y );
	gr->updateElement( ex, ey );
	gr->updateElement( endX, endY );
	setScore( score+2 );
	break;
      case REFLECTION:
	r->set( x, y, RLASERBBG );
	gr->updateElement( x, y );
	setScore( score+1 );
	break;
      case HIT:
	r->set( x, y, HLASERBBG );
	gr->updateElement( x, y );
	setScore( score+1 );
	break;
      case WRONGSTART:
	debug( "Wrong start?! It should't happen!!" );
	break;
      }
      break;
    }
  } else if (state & RightButton) {
    switch (type) {
    case INNERBBG:
      r->set( col, row, MARK1BBG ); 
      break;
      /*case MARK1BBG:
	r->set( col, row, INNERBBG ); 
      break;*/
    }
  }
  gr->updateElement( col, row );
  updateStats();
}
