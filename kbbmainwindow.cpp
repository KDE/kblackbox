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

#include "kbbmainwindow.h"



#include <QFile>
#include <QHBoxLayout>
#include <QWidget>


#include <kactioncollection.h>
#include <kconfigdialog.h>
#include <kgameclock.h>
#include <kgamedifficulty.h>
#include <kgamepopupitem.h>
#include <kglobal.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kstandardgameaction.h>
#include <kstatusbar.h>
#include <ktoggleaction.h>


#include "kbbgamedoc.h"
#include "kbbgraphicsitemtutorialmarker.h"
#include "kbbinfowidget.h"
#include "kbblevelconfigurationwidget.h"
#include "kbbprefs.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"
#include "kbbtutorial.h"



//
// Constructor / Destructor
//

KBBMainWindow::KBBMainWindow()
{
	m_boardEnabled = false;

	// Status bar
	statusBar()->insertItem(i18n("Run: yesno"), SRUN);
	statusBar()->insertItem(i18n("Time: 00:00"), STIME);
	statusBar()->insertItem(i18n("Size: 00 x 00"), SSIZE);


	// Difficulty
	KGameDifficulty::init(this, this, SLOT(levelChanged(KGameDifficulty::standardLevel)));
	KGameDifficulty::addStandardLevel(KGameDifficulty::veryEasy);
	KGameDifficulty::addStandardLevel(KGameDifficulty::easy);
	KGameDifficulty::addStandardLevel(KGameDifficulty::medium);
	KGameDifficulty::addStandardLevel(KGameDifficulty::hard);
	KGameDifficulty::addStandardLevel(KGameDifficulty::veryHard);
	KGameDifficulty::addStandardLevel(KGameDifficulty::extremelyHard);
	KGameDifficulty::addStandardLevel(KGameDifficulty::configurable);


	// Menu "Game"
	KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
	m_pauseAction = KStandardGameAction::pause(this, SLOT(pause(bool)), actionCollection());
	QAction* tutorial = actionCollection()->addAction("game_tutorial");
	tutorial->setText(i18n("Start tutorial"));
	tutorial->setIcon(KIcon("footprint"));
	connect(tutorial, SIGNAL(triggered(bool)), SLOT(startTutorial()));
	KStandardGameAction::quit(this, SLOT(close()), actionCollection());
	QAction* sandbox = actionCollection()->addAction("game_sandbox");
	sandbox->setText(i18n("New sandbox game"));
	connect(sandbox, SIGNAL(triggered(bool)), SLOT(startSandbox()));

	// Menu "Move"
	m_check = actionCollection()->addAction("move_check");
	m_check->setText(i18n("Check positions"));
	m_check->setIcon(KIcon("ok"));
	connect(m_check, SIGNAL(triggered(bool)), SLOT(check()));
	m_solveAction = KStandardGameAction::solve(this, SLOT(solve()), actionCollection());

	// Menu "Settings"
	KStandardAction::preferences(this, SLOT(settingsDialog()), actionCollection());


	// Theme manager
	QString svgzFile = KBBPrefs::theme();
	if (!QFile(svgzFile).exists())
		svgzFile = KStandardDirs::locate("appdata", "pics/kblackbox.svgz");
	m_themeManager = new KBBThemeManager(svgzFile);
	
	
	// Info widget
	m_infoWidget = new KBBInfoWidget(m_themeManager, m_check);
	
	
	// Tutorial widget
	m_tutorial = new KBBTutorial(this);


	// Board
	m_gameDoc = new KBBGameDoc(this, m_tutorial);
	connect(m_gameDoc, SIGNAL(updateStats()), this, SLOT(updateStats()) );
	connect(m_gameDoc, SIGNAL(isRunning(bool)), SLOT(setRunning(bool)));


	// Game widget
	m_gameWidget = new KBBScalableGraphicWidget(m_gameDoc, m_themeManager);
	m_tutorial->setGameWidget(m_gameWidget, new KBBGraphicsItemTutorialMarker(m_gameWidget, m_themeManager, KBBTutorial::COLUMNS, KBBTutorial::ROWS));


	// Central Widget
	m_centralWidget = new QWidget(this);
	QHBoxLayout *widgetLayout = new QHBoxLayout();
	widgetLayout->setMargin(0);
	m_centralWidget->setLayout(widgetLayout);
	widgetLayout->addWidget(m_infoWidget);
	widgetLayout->addWidget(m_gameWidget);
	widgetLayout->addWidget(m_tutorial);
	setCentralWidget(m_centralWidget);


	// Keyboard only
	QAction* action = actionCollection()->addAction( "move_down" );
	action->setText( i18n("Move Down") );
	connect(action, SIGNAL(triggered(bool) ), m_gameWidget, SLOT(keyboardMoveDown()));
	action->setShortcut(Qt::Key_Down);
	addAction(action);
	
	action = actionCollection()->addAction( "move_up" );
	action->setText( i18n("Move Up") );
	connect(action, SIGNAL(triggered(bool) ), m_gameWidget, SLOT(keyboardMoveUp()));
	action->setShortcut(Qt::Key_Up);
	addAction(action);
	
	action = actionCollection()->addAction( "move_left" );
	action->setText( i18n("Move Left") );
	connect(action, SIGNAL(triggered(bool) ), m_gameWidget, SLOT(keyboardMoveLeft()));
	action->setShortcut(Qt::Key_Left);
	addAction(action);
	
	action = actionCollection()->addAction( "move_right" );
	action->setText( i18n("Move Right") );
	connect(action, SIGNAL(triggered(bool) ), m_gameWidget, SLOT(keyboardMoveRight()));
	action->setShortcut(Qt::Key_Right);
	addAction(action);
	
	action = actionCollection()->addAction("switch_ball");
	action->setText(i18n("Switch ball or shoot laser"));
	connect(action, SIGNAL(triggered(bool)), m_gameWidget, SLOT(keyboardEnter()));
	action->setShortcut(Qt::Key_Return);
	addAction(action);
	
	action = actionCollection()->addAction("switch_marker");
	action->setText(i18n("Switch marker"));
	connect(action, SIGNAL(triggered(bool)), m_gameWidget, SLOT(keyboardSpace()));
	action->setShortcut(Qt::Key_Space);
	addAction(action);


	m_gameClock = new KGameClock(this, KGameClock::MinSecOnly);
	connect(m_gameClock, SIGNAL(timeChanged(const QString&)), SLOT(updateStats()));


	//Read configuration options
	m_customBallNumber = KBBPrefs::balls();
	m_customColumns = KBBPrefs::columns();
	m_customRows = KBBPrefs::rows();

	levelChanged((KGameDifficulty::standardLevel) (KBBPrefs::level()));
	KGameDifficulty::setLevel(m_level);

	newGame();
	setupGUI();
}


KBBMainWindow::~KBBMainWindow()
{
	KBBPrefs::self()->writeConfig();
}



//
// Public slots
//

void KBBMainWindow::levelChanged(KGameDifficulty::standardLevel level)
{
	switch(level) {
		case KGameDifficulty::veryEasy:
			m_ballNumber = 2;
			m_columns = 6;
			m_rows = 6;
			break;
		case KGameDifficulty::easy:
		default:
			m_ballNumber = 4;
			m_columns = 8;
			m_rows = 8;
			level = KGameDifficulty::medium;
			break;
		case KGameDifficulty::medium:
			m_ballNumber = 6;
			m_columns = 10;
			m_rows = 10;
			break;
		case KGameDifficulty::hard:
			m_ballNumber = 8;
			m_columns = 12;
			m_rows = 12;
			break;
		case KGameDifficulty::veryHard:
			m_ballNumber = 10;
			m_columns = 14;
			m_rows = 10;
			break;
		case KGameDifficulty::extremelyHard:
			m_ballNumber = 12;
			m_columns = 18;
			m_rows = 12;
			break;
		case KGameDifficulty::configurable:
			m_gameWidget->popupText(i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "Note: You can change<br />the parameters of<br />custom games in the<br />setting dialog."));
			break;
	}

	m_level = level;
	KBBPrefs::setLevel((int)(m_level));
	startGame(m_sandboxMode);
}


void KBBMainWindow::setRunning(bool r)
{
	// Difficulty
	KGameDifficulty::setRunning(r);

	// Clock
	if (r)
		m_gameClock->resume();
	else
		m_gameClock->pause();

	// Pause
	m_pauseAction->setEnabled(r);
}


void KBBMainWindow::updateStats()
{
	m_check->setEnabled(m_solveAction->isEnabled() && (m_gameDoc->numberOfBallsPlaced() == m_gameDoc->numberOfBallsToPlace()));
	m_infoWidget->setEnabled(m_solveAction->isEnabled());

	// 1. Status bar
	if (m_tutorial->isVisible())
		statusBar()->changeItem(i18n("Run: Tutorial"), SRUN );
	if (!m_tutorial->isVisible()) {
		if (m_boardEnabled)
			statusBar()->changeItem(i18n("Run: Yes"), SRUN );
		else
			statusBar()->changeItem(i18n("Run: No"), SRUN );
	}

	statusBar()->changeItem(i18n("Time: %1", m_gameClock->timeString()), STIME);

	statusBar()->changeItem( i18n("Size: %1 x %2", m_gameDoc->columns(), m_gameDoc->rows()), SSIZE );
	
	
	// 2. Info Widget
	m_infoWidget->setPlacedBalls(m_gameDoc->numberOfBallsPlaced());
	m_infoWidget->setScore(m_gameDoc->getScore());
}



//
// Private slots
//

void KBBMainWindow::check()
{
	if (m_tutorial->isVisible() && !m_tutorial->maySolve()) {
		KMessageBox::sorry(this, i18n("Please first finish the tutorial.\nCheck the positions of the balls when you have reached the last step."), i18n("Check positions"));
	} else {
		solving();

		const int score = m_gameDoc->getScore();
		QString s;
		if (score <= (m_ballNumber*3)) {
			s = i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "Your final score is: %1.<br />You did really well!", score);
			if (m_sandboxMode)
				s += QString("<br /><br />") + i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "But it does not count<br />because <b>it is the sandbox!</b>");
		} else
			s = i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "Your final score is: %1.<br />I guess you need more practice.", score);
		m_gameWidget->popupText(s);
	}
}


void KBBMainWindow::newGame()
{
	if (mayAbortGame())
		startGame(false);
}


void KBBMainWindow::pause(bool state)
{
	if (state) {
		m_gameClock->pause();
		m_gameWidget->popupText(i18n("Game paused.<br />Press \"%1\" to resume.", m_pauseAction->shortcut().toString()), 0);
	} else {
		m_gameClock->resume();
		m_gameWidget->popupText("");
	}

	m_gameWidget->setPause(state);
}


void KBBMainWindow::settingsChanged()
{
	m_customBallNumber = m_levelConfig->balls();
	m_customColumns = m_levelConfig->columns();
	m_customRows = m_levelConfig->rows();
	
	if (m_level==KGameDifficulty::configurable) {
		bool mayRestart = true;
		if (m_gameDoc->gameReallyStarted())
			if (KMessageBox::questionYesNo(this, i18n("Do you want to cancel the current custom game and start a new one with the new parameters?"), QString(), KGuiItem(i18n("Start new game"))) == KMessageBox::No)
				mayRestart = false;

		if (mayRestart)
			startGame(m_sandboxMode);
	}
}


void KBBMainWindow::settingsDialog()
{
	if (!KConfigDialog::showDialog("settings")) {
		KConfigDialog *dialog = new KConfigDialog(this, "settings", KBBPrefs::self());
		m_levelConfig = new KBBLevelConfigurationWidget(dialog, m_customBallNumber, m_customColumns, m_customRows, m_themeManager);
		dialog->addPage(m_levelConfig, i18n("Custom game"), "games-difficult");
		connect(dialog, SIGNAL(settingsChanged(const QString&)), this, SLOT(settingsChanged()));
		dialog->show();
	}
}


void KBBMainWindow::solve()
{
	if (m_tutorial->isVisible() && !m_tutorial->maySolve()) {
		KMessageBox::sorry(this, i18n("Sorry, you may not give up the tutorial."), i18n("Solve"));
	} else {
		if (KMessageBox::warningContinueCancel(this, i18np("You should place %1 ball!\n", "You should place %1 balls!\n", m_gameDoc->numberOfBallsToPlace()) + i18np("You have placed %1.\n", "You have placed %1.\n", m_gameDoc->numberOfBallsPlaced()) + i18n("Do you really want to give up this game?"), QString(), KGuiItem(i18n("Give up"))) == KMessageBox::Continue)
			solving();
	}
}


void KBBMainWindow::startSandbox()
{
	if (mayAbortGame()) {
		startGame(true);
		m_gameWidget->popupText(i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "Note: In the sandbox mode,<br />the solution is already displayed.<br />Have fun!"));
	}
}


void KBBMainWindow::startTutorial()
{
	if (mayAbortGame()) {
		m_gameDoc->startTutorial();
		m_solveAction->setEnabled(true);
		m_pauseAction->setChecked(false);
		KGameDifficulty::setEnabled(false);
		m_infoWidget->setGameParameters(KBBTutorial::BALLS, KBBTutorial::BALLS*3);

		// Reset clock but don't start it yet.
		m_gameClock->restart();
		m_gameClock->pause();

		updateStats();
	}
}



//
// Private
//

bool KBBMainWindow::mayAbortGame()
{
	bool mayAbort = true;

	if (m_gameDoc->gameReallyStarted())
		mayAbort = ( KMessageBox::warningContinueCancel(0, i18n("This will be the end of the current game!"), QString(), KGuiItem(i18n("Start new game"))) == KMessageBox::Continue );

	return mayAbort;
}


void KBBMainWindow::solving()
{
	m_boardEnabled = false;
	m_solveAction->setEnabled(false);
	m_gameDoc->gameOver();
	m_gameWidget->solve(false);
	updateStats();
}


void KBBMainWindow::startGame(bool sandboxMode)
{
	if (m_level==KGameDifficulty::configurable) {
		m_ballNumber = m_customBallNumber;
		m_columns = m_customColumns;
		m_rows = m_customRows;
	}

	m_boardEnabled = true;
	m_sandboxMode = sandboxMode;

	m_solveAction->setEnabled(true);
	m_pauseAction->setChecked(false);
	KGameDifficulty::setEnabled(true);
	m_tutorial->hide();
	m_gameDoc->newGame(m_ballNumber, m_columns, m_rows);
	m_gameWidget->newGame(m_columns, m_rows);
	if (m_sandboxMode)
		m_gameWidget->solve(true);

	m_infoWidget->setGameParameters(m_ballNumber, m_ballNumber*3);

	// Reset clock but don't start it yet.
	m_gameClock->restart();
	m_gameClock->pause();

	updateStats();
}



#include "kbbmainwindow.moc"
