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


#include <KActionCollection>
#include <KConfigDialog>
#include <KGameClock>
#include <KGameDifficulty>
#include <KGamePopupItem>
#include <KGlobal>
#include <KLocale>
#include <KMessageBox>
#include <KScoreDialog>
#include <KStandardDirs>
#include <KStandardGameAction>
#include <KStatusBar>
#include <KToggleAction>


#include "kbbgamedoc.h"
#include "kbbgraphicsitemtutorialmarker.h"
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

	//Read configuration options
	m_customBallNumber = KBBPrefs::balls();
	m_customColumns = KBBPrefs::columns();
	m_customRows = KBBPrefs::rows();


	// Status bar
	statusBar()->insertPermanentItem("", SRUN, 1);
	statusBar()->insertPermanentItem(i18n("Time: 00:00"), STIME, 1);
	statusBar()->insertPermanentItem(i18n("Size: 00 x 00"), SSIZE);
	statusBar()->setItemAlignment(SRUN, Qt::AlignLeft & Qt::AlignVCenter);


	// Difficulty
	KGameDifficulty::init(this, this, SLOT(levelChanged(KGameDifficulty::standardLevel)));
	KGameDifficulty::addStandardLevel(KGameDifficulty::VeryEasy);
	KGameDifficulty::addStandardLevel(KGameDifficulty::Easy);
	KGameDifficulty::addStandardLevel(KGameDifficulty::Medium);
	KGameDifficulty::addStandardLevel(KGameDifficulty::Hard);
	KGameDifficulty::addStandardLevel(KGameDifficulty::VeryHard);
	KGameDifficulty::addStandardLevel(KGameDifficulty::ExtremelyHard);
	KGameDifficulty::addStandardLevel(KGameDifficulty::Configurable);


	// Menu "Game"
	KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
	m_pauseAction = KStandardGameAction::pause(this, SLOT(pause(bool)), actionCollection());
	QAction* tutorial = actionCollection()->addAction("game_tutorial");
	tutorial->setText(i18n("Start Tutorial"));
	tutorial->setIcon(KIcon("footprint"));
	tutorial->setToolTip(i18n("Start tutorial"));
	tutorial->setWhatsThis(i18n("<qt>The <b>tutorial</b> is a fast, user friendly and interactive way to learn the rules of the game. Start it if you do not know them!</qt>"));
	connect(tutorial, SIGNAL(triggered(bool)), SLOT(startTutorial()));
	KStandardGameAction::quit(this, SLOT(close()), actionCollection());
	QAction* sandbox = actionCollection()->addAction("game_sandbox");
	sandbox->setText(i18n("New Sandbox Game"));
	sandbox->setToolTip(i18n("Start a new sandbox game"));
	sandbox->setWhatsThis(i18n("<qt><p>In a <b>sandbox game</b>, the solution is displayed at the beginning of the game. This is useful to understand the game principles.</p><p>However: after a while, it is not really fun and you should try to start a real game!</p></qt>"));
	connect(sandbox, SIGNAL(triggered(bool)), SLOT(startSandbox()));
	KStandardGameAction::highscores(this, SLOT(showHighscores()), actionCollection());

	// Menu "Move"
	m_doneAction = actionCollection()->addAction("move_done");
	m_doneAction->setText(i18n("Done!"));
	m_doneAction->setWhatsThis(i18n("<qt><ul><li>First, you have to place all the balls on the black box. To guess the right possitions of the balls and see how they interact with laser beams, you should use the lasers that are disposed around the black box.</li><li><b>When you think you are done</b>, you should click here.</li></ul><p>Note that it is possible to click here only if you placed the right number of balls.</p></qt>"));
	m_doneAction->setIcon(KIcon("ok"));
	connect(m_doneAction, SIGNAL(triggered(bool)), SLOT(done()));
	m_solveAction = KStandardGameAction::solve(this, SLOT(solve()), actionCollection());
	m_solveAction->setToolTip(i18n("Give up the game"));
	m_solveAction->setWhatsThis(i18n("<qt><p>Choose \"<b>Solve</b>\" if you want to give up the current game. The solution will be displayed.</p><p>If you placed all the balls and do not want to give up, choose \"Done!\".</p></qt>"));

	// Menu "Settings"
	KStandardAction::preferences(this, SLOT(settingsDialog()), actionCollection());


	// Theme manager
	QString svgzFile = KBBPrefs::theme();
	if (!QFile(svgzFile).exists())
		svgzFile = KStandardDirs::locate("appdata", "pics/kblackbox.svgz");
	m_themeManager = new KBBThemeManager(svgzFile);
	
	
	// Tutorial widget
	m_tutorial = new KBBTutorial(this);


	// Board
	m_gameDoc = new KBBGameDoc(this, m_tutorial);
	connect(m_gameDoc, SIGNAL(updateStats()), this, SLOT(updateStats()) );
	connect(m_gameDoc, SIGNAL(isRunning(bool)), SLOT(setRunning(bool)));


	// Game widget
	m_gameWidget = new KBBScalableGraphicWidget(m_gameDoc, m_themeManager, m_doneAction);
	m_tutorial->setGameWidget(m_gameWidget, new KBBGraphicsItemTutorialMarker(m_gameWidget, m_themeManager, KBBTutorial::COLUMNS, KBBTutorial::ROWS));


	// Central Widget
	m_centralWidget = new QWidget(this);
	QHBoxLayout *widgetLayout = new QHBoxLayout();
	widgetLayout->setMargin(0);
	m_centralWidget->setLayout(widgetLayout);
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
	connect(m_gameClock, SIGNAL(timeChanged(const QString&)), m_gameDoc, SLOT(timeChanged()));


	levelChanged((KGameDifficulty::standardLevel) (KBBPrefs::level()));
	KGameDifficulty::setLevel(m_level);

	// Highscores
	m_scoreDialog = new KScoreDialog(KScoreDialog::Score | KScoreDialog::Name, this);

	setupGUI();

	// start a new game
	startGame(false);
}


KBBMainWindow::~KBBMainWindow()
{
	KBBPrefs::self()->writeConfig();

	delete m_gameWidget;
	delete m_themeManager;
}



//
// Public slots
//

void KBBMainWindow::levelChanged(KGameDifficulty::standardLevel level)
{
	switch(level) {
		case KGameDifficulty::VeryEasy:
			m_ballNumber = 2;
			m_columns = 6;
			m_rows = 6;
			break;
		case KGameDifficulty::Easy:
		default:
			m_ballNumber = 4;
			m_columns = 8;
			m_rows = 8;
			level = KGameDifficulty::Medium;
			break;
		case KGameDifficulty::Medium:
			m_ballNumber = 6;
			m_columns = 10;
			m_rows = 10;
			break;
		case KGameDifficulty::Hard:
			m_ballNumber = 8;
			m_columns = 12;
			m_rows = 12;
			break;
		case KGameDifficulty::VeryHard:
			m_ballNumber = 11;
			m_columns = 14;
			m_rows = 10;
			break;
		case KGameDifficulty::ExtremelyHard:
			m_ballNumber = 15;
			m_columns = 20;
			m_rows = 12;
			break;
		case KGameDifficulty::Configurable:
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
	if (r) {
		m_gameClock->resume();
		m_gameDoc->timeChanged(); // It's important to end the current seconde before pausing so that the player cannot cheat with pause.
	} else
		m_gameClock->pause();

	// Pause
	m_pauseAction->setEnabled(r);
}


void KBBMainWindow::updateStats()
{
	int ballsLeftToPlace = m_gameDoc->numberOfBallsToPlace() - m_gameDoc->numberOfBallsPlaced();

	m_doneAction->setEnabled(m_solveAction->isEnabled() && (ballsLeftToPlace==0));

	if (ballsLeftToPlace<0)
		m_doneAction->setToolTip(i18np("First, you need to remove 1 ball from the black box.", "First, you need to remove %1 balls from the black box.", -ballsLeftToPlace));
	else if (ballsLeftToPlace==0) {
		m_doneAction->setToolTip(i18n("To check if you successful guessed the ball positions, click here!"));
	} else if (ballsLeftToPlace>0) {
		m_doneAction->setToolTip(i18np("You need to place 1 more ball on the black box.", "You need to place %1 more balls on the black box.", ballsLeftToPlace));
	}

	if (!m_boardEnabled)
		m_doneAction->setToolTip(i18n("Game over."));
	if (m_pauseAction->isChecked())
		m_doneAction->setToolTip(i18n("Game paused."));


	// Status bar
	if (m_tutorial->isVisible())
		statusBar()->changeItem(i18n("Tutorial"), SRUN );
	if (!m_tutorial->isVisible()) {
		if (m_boardEnabled) {
			if (ballsLeftToPlace<0) {
				statusBar()->changeItem((i18np("1 ball too much!", "%1 balls too much!", -ballsLeftToPlace)), SRUN);
			} else if (ballsLeftToPlace==0) {
				statusBar()->changeItem(i18n("No more ball to place"), SRUN);
			} else if (ballsLeftToPlace>0) {
				statusBar()->changeItem(i18np("1 ball to place", "%1 balls to place", ballsLeftToPlace), SRUN);
			}	
		} else
			statusBar()->changeItem(i18n("Game over"), SRUN );
	}

	statusBar()->changeItem(i18n("Time: %1", m_gameClock->timeString()), STIME);

	statusBar()->changeItem( i18n("Size: %1 x %2", m_gameDoc->columns(), m_gameDoc->rows()), SSIZE );
	
	
	// 2. Info Widget
	m_gameWidget->setScore(m_gameDoc->score());
}



//
// Private slots
//

void KBBMainWindow::done()
{
	if (m_tutorial->isVisible() && !m_tutorial->maySolve()) {
		KMessageBox::sorry(this, i18n("Clicking on \"Done!\" is the normal way to check the positions of the balls at the end of the game. However, it is not possible in the tutorial to end the game before you reached the last step.\nPlease first finish the tutorial."), i18n("Check positions"));
	} else {
		solving();

		const int score = m_gameDoc->score();
		QString s;
		if (score <= (m_ballNumber*35)) {
			s = i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "Your final score is: %1.<br />You did really well!", score);
			if (m_sandboxMode)
				s += QString("<br /><br />") + i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "But it does not count<br />because <b>it is the sandbox!</b>");
		} else
			s = i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "Your final score is: %1.<br />I guess you need more practice.", score);

		if ((!m_tutorial->isVisible()) && (!m_sandboxMode) && (KGameDifficulty::level() != KGameDifficulty::Configurable) && (score<KBBGameDoc::SCORE_LOST)) {
			m_scoreDialog->setConfigGroup(KGameDifficulty::levelString());
	
			KScoreDialog::FieldInfo scoreInfo;
			scoreInfo[KScoreDialog::Score].setNum(score);
			if(m_scoreDialog->addScore(scoreInfo, KScoreDialog::LessIsMore) != 0)
				m_scoreDialog->exec();
		}

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
	m_solveAction->setEnabled(!state);

	updateStats();
	m_gameWidget->setPause(state);
}


void KBBMainWindow::settingsChanged()
{
	m_customBallNumber = m_levelConfig->balls();
	m_customColumns = m_levelConfig->columns();
	m_customRows = m_levelConfig->rows();
	
	if (m_level==KGameDifficulty::Configurable) {
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
		dialog->addPage(m_levelConfig, i18n("Custom game"), "games-config-custom");
		connect(dialog, SIGNAL(settingsChanged(const QString&)), this, SLOT(settingsChanged()));
		dialog->show();
	}
}


void KBBMainWindow::showHighscores()
{
	m_scoreDialog->setConfigGroup( KGameDifficulty::levelString() );
	m_scoreDialog->exec();
}


void KBBMainWindow::solve()
{
	if (m_tutorial->isVisible() && !m_tutorial->maySolve()) {
		KMessageBox::sorry(this, i18n("Sorry, you may not give up the tutorial."), i18n("Solve"));
	} else {
		if (m_gameDoc->numberOfBallsToPlace()==m_gameDoc->numberOfBallsPlaced()) {
			if (KMessageBox::warningContinueCancel(this, i18n("You placed all the balls. Great!\nYou should now click on \"Done!\" to end the game and check if you guessed right.\nSo, do you really want to give up this game?"), QString(), KGuiItem(i18n("Give up"))) == KMessageBox::Continue)
				solving();
		} else if (KMessageBox::warningContinueCancel(this, i18np("You should place %1 ball!\n", "You should place %1 balls!\n", m_gameDoc->numberOfBallsToPlace()) + i18np("You have placed %1.\n", "You have placed %1.\n", m_gameDoc->numberOfBallsPlaced()) + i18n("Do you really want to give up this game?"), QString(), KGuiItem(i18n("Give up"))) == KMessageBox::Continue)
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
	m_doneAction->setEnabled(false);
	m_gameDoc->gameOver();
	m_gameWidget->solve(false);
	updateStats();
}


void KBBMainWindow::startGame(bool sandboxMode)
{
	if (m_level==KGameDifficulty::Configurable) {
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
	m_gameWidget->newGame(m_columns, m_rows, m_ballNumber);
	if (m_sandboxMode)
		m_gameWidget->solve(true);

	// Reset clock but don't start it yet.
	m_gameClock->restart();
	m_gameClock->pause();

	updateStats();
}



#include "kbbmainwindow.moc"
