//
// KBlackbox
//
// A simple game inspired by an emacs module
//
// File: kbbgame.cpp
//
// The implementation of the KBBGame widget
//

#include <QLabel>
#include <QPushButton>
#include <QToolTip>
#include <QString>
//Added by qt3to4:
#include <QPixmap>
#include <ktoolbar.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kapplication.h>
#include <klocale.h>
#include <kconfig.h>
#include <kglobal.h>
#include <kmenubar.h>
#include <kiconloader.h>
#include <khelpmenu.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kstatusbar.h>
#include <kstandardgameaction.h>
#include <ktoggleaction.h>
#include <kactioncollection.h>
#include <kicon.h>
#include "kbbgame.h"
#include "util.h"
#include "version.h"

/*
  Names of pixmap files.
*/

const char *pFNames[NROFTYPES] = {
  "white",
  "gray",
  "green",
  "red",
  "blue",
  "cyan",
  "brown",
  "green2"
};

/*
   Creates the KBBGame widget and sets saved options (if any).
*/

KBBGame::KBBGame()
{
  int i;

  QPixmap **pix = new QPixmap * [NROFTYPES];
  pix[0] = new QPixmap();
  *pix[0] = BarIcon( pFNames[0] );
  if (!pix[0]->isNull()) {
    kDebug(12009) << "Pixmap \"" << pFNames[0] << "\" loaded." << endl;
    for (i = 1; i < NROFTYPES; i++) {
      pix[i] = new QPixmap;
      *pix[i] = BarIcon( pFNames[i] );
      if (!pix[i]->isNull()) {
	kDebug(12009) << "Pixmap \"" << pFNames[i] << "\" loaded." << endl;
      } else {
	pix[i] = pix[i-1];
	pix[i]->detach();
	kDebug(12009) << "Cannot find pixmap \"" << pFNames[i] << "\". Using previous one." << endl;
      }
    }
  } else {
    kDebug(12009) << "Cannot find pixmap \"" << pFNames[0] << "\". Pixmaps will not be loaded." << endl;
    delete pix[0];
    delete [] pix;
    pix = 0;
  }
  gr = new KBBGraphic( pix, this );
  gr->setObjectName("KBBGraphic");

  statusBar()->insertItem(i18n("Score: 0000"), SSCORE);
  statusBar()->insertItem(i18n("Placed: 00 / 00"), SBALLS);
  statusBar()->insertItem(i18n("Run: yesno"), SRUN);
  statusBar()->insertItem(i18n("Size: 00 x 00"), SSIZE);

  initKAction();

  connect( gr, SIGNAL(inputAt(int,int,int)),
	  this, SLOT(gotInputAt(int,int,int)) );
  connect( this, SIGNAL(gameRuns(bool)),
	  gr, SLOT(setInputAccepted(bool)) );
  connect( gr, SIGNAL(endMouseClicked()),
	  this, SLOT(gameFinished()) );

  /*
  doneButton->setToolTip( i18n(
		 "Click here when you think you placed all the balls.") );
		 */

  /*
     Game initializations
  */
  running = false;
  gameBoard = NULL;

  KConfig *kConf;
  int j;
  kConf = KGlobal::config();
  kConf->setGroup( "KBlackBox Setup" );
  if (kConf->hasKey( "Balls" )) {
    i = kConf->readEntry( "Balls",0 );
    balls = i;
    switch (i) {
    case 4: ballsAction->setCurrentItem(0); break;
    case 6: ballsAction->setCurrentItem(1); break;
    case 8: ballsAction->setCurrentItem(2); break;
    }
  } else {
    balls = 4;
    ballsAction->setCurrentItem(0);
  }
  if ((kConf->hasKey( "Width" )) &&
      (kConf->hasKey( "Balls" ))) {
    i = kConf->readEntry( "Width", 0 );
    j = kConf->readEntry( "Height",0 );
    gr->setSize( i+4, j+4 ); // +4 is the space for "lasers" and an edge...
    gameBoard = new RectOnArray( gr->numC(), gr->numR() );
    switch (i) {
    case 8: sizeAction->setCurrentItem(0); break;
    case 10: sizeAction->setCurrentItem(1); break;
    case 12: sizeAction->setCurrentItem(2); break;
    }
  } else {
    gr->setSize( 8+4, 8+4 ); // +4 is the space for "lasers" and an edge...
    gameBoard = new RectOnArray( gr->numC(), gr->numR() );
    sizeAction->setCurrentItem(0);
  }
  if (kConf->hasKey( "tutorial" )) {
    tutorial = (bool) kConf->readEntry( "tutorial",0 );
  } else tutorial = false;
  tutorialAction->setChecked(tutorial);

  setCentralWidget( gr );

  setScore( 0 );
  ballsPlaced = 0;

	updateStats();

  newGame();
	setMinSize();

  setupGUI();
}

/*
   Saves the options and destroys the KBBGame widget.
*/

KBBGame::~KBBGame()
{
  KConfig *kConf;
  QString s;

  kConf = KGlobal::config();
  kConf->setGroup( "KBlackBox Setup" );
  kConf->writeEntry( "Balls", balls );
  kConf->writeEntry( "Width", gr->numC() - 4);
  kConf->writeEntry( "Height", gr->numR() - 4);
  kConf->writeEntry( "tutorial", (int) tutorial );

  delete gameBoard;
  // All the rest has "this" for parent so it doesn't need to be deleted.
}


/*
   Resizes yourself to fit the contents perfectly, from menu.
*/
void KBBGame::gameResize()
{
  resize( gr->wHint(), gr->hHint() + menuBar()->height() + statusBar()->height() +
      toolBar()->height() );
}

void KBBGame::setMinSize()
{
  setMinimumSize( gr->wHint(), gr->hHint() + menuBar()->height() + statusBar()->height() +
      toolBar()->height() );
}

/*
   Settings of various options.
*/
void KBBGame::slotSize()
{
  int i = sizeAction->currentItem();
  bool ok = false;
  switch (i) {
    case 0:
      ok = setSize( 8, 8 );
      break;
    case 1:
      ok = setSize( 10, 10 );
      break;
    case 2:
      ok = setSize( 12, 12 );
      break;
  }

  if (!ok) {
    switch(gr->numR() - 4) {
      case 8:
        sizeAction->setCurrentItem(0); break;
      case 10:
        sizeAction->setCurrentItem(1); break;
      case 12:
        sizeAction->setCurrentItem(2); break;
    }
  }
}

void KBBGame::slotBalls()
{
  int i = ballsAction->currentItem();
  bool ok = false;
  switch (i) {
      case 0:
          ok = setBalls( 4 );
          break;
      case 1:
          ok = setBalls( 6 );
    break;
        case 2:
          ok = setBalls( 8 );
    break;
  }
  if (!ok) {
    switch (balls) {
      case 4:
        ballsAction->setCurrentItem(0); break;
      case 6:
        ballsAction->setCurrentItem(1); break;
      case 8:
        ballsAction->setCurrentItem(2); break;
    }
  }
}

void KBBGame::tutorialSwitch()
{
  tutorial = !tutorial;
}

/*
   Creates a new game.
*/
void KBBGame::newGame()
{
  int i, j;

  if (running) {
    bool cancel;
    cancel = KMessageBox::warningContinueCancel(0,
         i18n("Do you really want to give up this game?"),QString::null,KGuiItem(i18n("Give Up")))
      == KMessageBox::Cancel;
    if (cancel)
      return;

    abortGame();
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
  gr->repaint();
  setScore( 0 );
  detourCounter = -1;
  ballsPlaced = 0;
  running = true;
  updateStats();
  emit gameRuns( running );
}

/*
   Ends the current game.
*/

void KBBGame::gameFinished()
{
  if (running) {
    QString s;
    if (ballsPlaced == balls) {
      getResults();
      abortGame();
      if (score <= (balls*3))
	s = i18n("Your final score is: %1\n"
		 "You did really well!", score);
      else
	s = i18n("Your final score is: %1\n"
		 "I guess you need more practice.", score);

      KMessageBox::information(this, s);
    } else {
      s = i18n( "You should place %1 balls!\n"
		"You have placed %2.",
	 KGlobal::locale()->formatNumber(balls, 0),
	 KGlobal::locale()->formatNumber(ballsPlaced, 0));

      KMessageBox::sorry(this, s);
    }
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
    running = false;
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
    bool stop;
    stop = KMessageBox::warningContinueCancel(0,
			    i18n(
			    "Do you really want to give up this game?"),QString::null,KGuiItem(i18n("Give Up")))
      == KMessageBox::Continue;

    if (stop) {
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
  QString tmp;
  QString s = i18n("Run: ");
  if (running)
    s += i18n("Yes");
  else
    s += i18n("No");
  statusBar()->changeItem( s, SRUN );
  s = i18n( "Size: " );
  s += tmp.sprintf( "%2d x %2d",
	       gr->numC()-4, gr->numR()-4 );
  statusBar()->changeItem( s, SSIZE );
  s = i18n( "Placed: " );
  s += tmp.sprintf( "%2d / %2d",
	     ballsPlaced, balls );
  statusBar()->changeItem( s, SBALLS );
}

/*
   Sets the score value to n.
*/

void KBBGame::setScore( int n )
{
  score = n;
  statusBar()->changeItem( i18n("Score: %1", n), SSCORE );
}

/*
   Sets the size of the black box.
*/

bool KBBGame::setSize( int w, int h )
{
  bool ok = false;
  if (((w+4) != gr->numC()) || ((h+4) != gr->numR())) {
    if (running) {
      ok = KMessageBox::warningContinueCancel(0,
			      i18n(
			      "This will be the end of the current game!"),QString::null,KGuiItem(i18n("End Game")))
	== KMessageBox::Continue;

    } else ok = true;
    if (ok) {
      gr->setSize( w+4, h+4 ); // +4 is the space for "lasers" and an edge...
      setMinSize();
      gameResize();
      delete gameBoard;
      gameBoard = new RectOnArray( gr->numC(), gr->numR() );
      if (running) abortGame();
      newGame();
      //      gr->repaint( true );
    }
  }
  return ok;
}

/*
   Sets the number of balls in the black box to n.
*/

bool KBBGame::setBalls( int n )
{
  bool ok = false;
  if (balls != n) {
    if (running) {
      ok = KMessageBox::warningContinueCancel(0,
			 i18n("This will be the end of the current game!"),QString::null,KGuiItem(i18n("End Game")))
	== KMessageBox::Continue;
    } else ok = true;
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
  random.setSeed(0);
  for (i = 0; i < n; i++) {
    int x=0, y=0; // there is OUTERBBT...
    while (gameBoard->get( x, y ) != INNERBBT ) {
      x =  2 + random.getLong(gameBoard->width()-4);
      y =  2 + random.getLong(gameBoard->height()-4);
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
    directionChanged = false;
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
      directionChanged = true;
      refl += 1;
    }
    if (gameBoard->get( x+srx, y+sry ) == BALLBBT) {
      type = REFLECTION;
      if (gameBoard->get( x, y ) == LASERBBT) break;
      directionChanged = true;
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

  if (state & Qt::LeftButton) {
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
      //kDebug << endX << " " << endY << endl;
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
	kDebug(12009) << "Wrong start?! It should't happen!!" << endl;
	break;
      }
      break;
    }
  } else if (state & Qt::RightButton) {
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

void KBBGame::initKAction()
{
// game
  QAction *newAct = KStandardGameAction::gameNew(this, SLOT(newGame()), this);
  actionCollection()->addAction(newAct->objectName(), newAct);
  QAction *giveUpAct = actionCollection()->addAction("game_giveup");
  giveUpAct->setIcon(KIcon(SmallIcon("giveup")));
  giveUpAct->setText(i18n("&Give Up"));
  connect(giveUpAct, SIGNAL(triggered(bool)), SLOT(giveUp()));
  QAction *doneAct = actionCollection()->addAction("game_done" );
  doneAct->setIcon(KIcon(SmallIcon("done")));
  doneAct->setText(i18n("&Done"));
  connect(doneAct, SIGNAL(triggered(bool)), SLOT(gameFinished()));
  QAction *action = actionCollection()->addAction("game_resize");
  action->setText(i18n("&Resize"));
  connect(action, SIGNAL(triggered(bool) ), SLOT(slotResize()));
  KStandardGameAction::quit(this, SLOT(close()), actionCollection());


// settings
  sizeAction = new KSelectAction( i18n("&Size"), this);
  actionCollection()->addAction("options_size", sizeAction);
  connect(sizeAction, SIGNAL(triggered(bool)), SLOT(slotSize()));
  QStringList list;
  list.append(i18n("  8 x  8 "));
  list.append(i18n(" 10 x 10 "));
  list.append(i18n(" 12 x 12 "));
  sizeAction->setItems(list);

  ballsAction = new KSelectAction( i18n("&Balls"), this);
  actionCollection()->addAction("options_balls", ballsAction);
  connect(ballsAction, SIGNAL(triggered(bool)), SLOT(slotBalls()));
  list.clear();
  list.append(i18n(" 4 "));
  list.append(i18n(" 6 "));
  list.append(i18n(" 8 "));
  ballsAction->setItems(list);
  tutorialAction = new KToggleAction( i18n("&Tutorial"), this );
  actionCollection()->addAction( "options_tutorial", tutorialAction );
  connect(tutorialAction, SIGNAL(triggered(bool) ), SLOT(tutorialSwitch()));
//  KStandardAction::keyBindings(guiFactory(), SLOT(configureShortcuts()),
//actionCollection());

// keyboard only
  action = actionCollection()->addAction( "move_down" );
  action->setText( i18n("Move Down") );
  connect(action, SIGNAL(triggered(bool) ), gr, SLOT(slotDown()));
  action->setShortcut(Qt::Key_Down);
  addAction(action);

  action = actionCollection()->addAction( "move_up" );
  action->setText( i18n("Move Up") );
  connect(action, SIGNAL(triggered(bool) ), gr, SLOT(slotUp()));
  action->setShortcut(Qt::Key_Up);
  addAction(action);

  action = actionCollection()->addAction( "move_left" );
  action->setText( i18n("Move Left") );
  connect(action, SIGNAL(triggered(bool) ), gr, SLOT(slotLeft()));
  action->setShortcut(Qt::Key_Left);
  addAction(action);

  action = actionCollection()->addAction( "move_right" );
  action->setText( i18n("Move Right") );
  connect(action, SIGNAL(triggered(bool) ), gr, SLOT(slotRight()));
  action->setShortcut(Qt::Key_Right);
  addAction(action);

  action = actionCollection()->addAction( "move_trigger" );
  action->setText( i18n("Trigger Action") );
  connect(action, SIGNAL(triggered(bool) ), gr, SLOT(slotInput()));
  action->setShortcut(Qt::Key_Return);
  addAction(action);
}

void KBBGame::slotResize()
{
    setMinSize();
    gameResize();
}

#include "kbbgame.moc"
