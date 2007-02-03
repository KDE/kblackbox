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

#include <ktoolbar.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kconfig.h>
#include <kglobal.h>
#include <kmenubar.h>
#include <khelpmenu.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kstatusbar.h>
#include <kstandardgameaction.h>
#include <ktoggleaction.h>
#include <kactioncollection.h>
#include <kicon.h>
#include "kbbgame.h"
#include "kbbboard.h"
#include "version.h"


/*
   Creates the KBBGame widget and sets saved options (if any).
*/
KBBGame::KBBGame()
{
  m_board = new KBBBoard(this);

  statusBar()->insertItem(i18n("Score: 0000"), SSCORE);
  statusBar()->insertItem(i18n("Placed: 00 / 00"), SBALLS);
  statusBar()->insertItem(i18n("Run: yesno"), SRUN);
  statusBar()->insertItem(i18n("Size: 00 x 00"), SSIZE);


  initKAction();

  connect( m_board, SIGNAL(updateStats()),
	  this, SLOT(updateStats()) );

  /*
  doneButton->setToolTip( i18n(
		 "Click here when you think you placed all the balls.") );
		 */

  /*
     Game initializations
  */
  running = false;

  KSharedConfigPtr kConf = KGlobal::config();
  kConf->setGroup( "KBlackBox Setup" );
  if (kConf->hasKey( "Balls" )) {
    balls = kConf->readEntry( "Balls",0 );
    switch (balls) {
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
    m_columns = kConf->readEntry( "Width", 0 );
    m_rows = kConf->readEntry( "Height",0 );
    switch (m_columns) {
    case 8: sizeAction->setCurrentItem(0); break;
    case 10: sizeAction->setCurrentItem(1); break;
    case 12: sizeAction->setCurrentItem(2); break;
    }
  } else {
    m_columns = 8;
    m_rows = 8;
    sizeAction->setCurrentItem(0);
  }
  if (kConf->hasKey( "tutorial" )) {
    tutorial = (bool) kConf->readEntry( "tutorial",0 );
  } else tutorial = false;
  tutorialAction->setChecked(tutorial);


  setCentralWidget( m_board->getWidget() );


  startGame();
  setupGUI();
  setMinSize();
}

/*
   Saves the options and destroys the KBBGame widget.
*/
KBBGame::~KBBGame()
{
  KSharedConfigPtr kConf = KGlobal::config();
  QString s;

  kConf->setGroup( "KBlackBox Setup" );
  kConf->writeEntry( "Balls", balls );
  kConf->writeEntry( "Width", m_columns);
  kConf->writeEntry( "Height", m_rows);
  kConf->writeEntry( "tutorial", (int) tutorial );

  // All the rest has "this" for parent so it doesn't need to be deleted.
}

void KBBGame::gameResize()
{
  resize( this->minimumWidth(), this->minimumHeight());
}

void KBBGame::setMinSize()
{
  setMinimumSize( m_board->getWidgetWidth(), m_board->getWidgetHeight() + menuBar()->height() + statusBar()->height() + toolBar()->height() );
}

/*
   Settings of various options.
*/
void KBBGame::slotSize(int selection)
{
  bool ok = false;
  switch (selection) {
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
    switch(m_columns) {
      case 8:
        sizeAction->setCurrentItem(0); break;
      case 10:
        sizeAction->setCurrentItem(1); break;
      case 12:
        sizeAction->setCurrentItem(2); break;
    } 
  }
}

void KBBGame::slotBalls(int selection)
{
  bool ok = false;

  switch (selection) {
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
  if (!comfirmGameEndIfNeeded()) 
    return;

  startGame();
}  

void KBBGame::startGame()
{
	running = true;
	m_board->newGame( balls, m_columns, m_rows, tutorial );
	updateStats();
}

/*
   Ends the current game.
*/
void KBBGame::gameFinished()
{
  if (running) {
    QString s;
    if (m_board->numberOfBallsPlaced() == balls) {
      abortGame();
      int score = m_board->getScore();
      if (score <= (balls*3))
	s = i18n("Your final score is: %1\n"
		 "You did really well!", score);
      else
	s = i18n("Your final score is: %1\n"
		 "I guess you need more practice.", score);

      KMessageBox::information(this, s);
    } else {
      s = i18n( "You should place %1 balls!\nYou have placed %2.\nDo you want to give up this game?", KGlobal::locale()->formatNumber(balls, 0), KGlobal::locale()->formatNumber(m_board->numberOfBallsPlaced(), 0));

      if ( KMessageBox::warningContinueCancel(this, s, QString::null, KGuiItem(i18n("Give Up"))) == KMessageBox::Continue )
        abortGame();
    }
  }
}

bool KBBGame::comfirmGameEndIfNeeded()
{
	bool gameMayEnd = true;

	if (m_board->gameReallyStarted())
		gameMayEnd = ( KMessageBox::warningContinueCancel(0, i18n("This will be the end of the current game!"), QString::null, KGuiItem(i18n("End Game"))) == KMessageBox::Continue );

	return gameMayEnd;
}

/*
   Aborts the current game.
*/
void KBBGame::abortGame()
{
    running = false;
    updateStats();
    m_board->gameOver();
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
  s += tmp.sprintf( "%2d x %2d", m_columns, m_rows );
  statusBar()->changeItem( s, SSIZE );
  s = i18n( "Placed: " );
  s += tmp.sprintf( "%2d / %2d", m_board->numberOfBallsPlaced(), balls );
  statusBar()->changeItem( s, SBALLS );

  statusBar()->changeItem( i18n("Score: %1", m_board->getScore()), SSCORE );
}


/*
   Sets the size of the black box.
*/
bool KBBGame::setSize( int w, int h )
{
	bool ok = false;
	if ((w != m_columns) || (h != m_rows)) {
		if (comfirmGameEndIfNeeded()) {
			ok = true;
			m_columns = w;
			m_rows = h;
			startGame();
			setMinSize();
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
		if (comfirmGameEndIfNeeded()) {
			ok = true;
			balls = n;
			startGame();
		}
	}
	return ok;
}



void KBBGame::initKAction()
{
// game
  QAction *newAct = KStandardGameAction::gameNew(this, SLOT(newGame()), this);
  actionCollection()->addAction(newAct->objectName(), newAct);

  QAction *doneAct = KStandardGameAction::solve(this, SLOT(gameFinished()), this);
  actionCollection()->addAction(doneAct->objectName(), doneAct);

  QAction *action = actionCollection()->addAction("game_resize");
  action->setText(i18n("&Resize"));
  connect(action, SIGNAL(triggered(bool) ), SLOT(gameResize()));
  KStandardGameAction::quit(this, SLOT(close()), actionCollection());


// settings
  sizeAction = new KSelectAction( i18n("&Size"), this);
  actionCollection()->addAction("options_size", sizeAction);
  connect(sizeAction, SIGNAL(triggered(int)), this, SLOT(slotSize(int)));
  QStringList list;
  list.append(i18n("  8 x  8 "));
  list.append(i18n(" 10 x 10 "));
  list.append(i18n(" 12 x 12 "));
  sizeAction->setItems(list);

  ballsAction = new KSelectAction( i18n("&Balls"), this);
  actionCollection()->addAction("options_balls", ballsAction);
  connect(ballsAction, SIGNAL(triggered(int)), this, SLOT(slotBalls(int)));
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
  connect(action, SIGNAL(triggered(bool) ), m_board->getWidget(), SLOT(slotDown()));
  action->setShortcut(Qt::Key_Down);
  addAction(action);

  action = actionCollection()->addAction( "move_up" );
  action->setText( i18n("Move Up") );
  connect(action, SIGNAL(triggered(bool) ), m_board->getWidget(), SLOT(slotUp()));
  action->setShortcut(Qt::Key_Up);
  addAction(action);

  action = actionCollection()->addAction( "move_left" );
  action->setText( i18n("Move Left") );
  connect(action, SIGNAL(triggered(bool) ), m_board->getWidget(), SLOT(slotLeft()));
  action->setShortcut(Qt::Key_Left);
  addAction(action);

  action = actionCollection()->addAction( "move_right" );
  action->setText( i18n("Move Right") );
  connect(action, SIGNAL(triggered(bool) ), m_board->getWidget(), SLOT(slotRight()));
  action->setShortcut(Qt::Key_Right);
  addAction(action);

  action = actionCollection()->addAction( "move_trigger" );
  action->setText( i18n("Trigger Action") );
  connect(action, SIGNAL(triggered(bool) ), m_board->getWidget(), SLOT(slotInput()));
  action->setShortcut(Qt::Key_Return);
  addAction(action);
}

#include "kbbgame.moc"
