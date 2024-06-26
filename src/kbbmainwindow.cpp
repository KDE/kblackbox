/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kbbmainwindow.h"

#include <QFile>
#include <QHBoxLayout>
#include <QPointer>
#include <QWidget>

#include <KActionCollection>
#include <KConfigDialog>
#include <KLocalizedString>
#include <KMessageBox>

#include <KGameClock>
#include <KGameHighScoreDialog>
#include <KGameStandardAction>

#include <QStatusBar>
#include <QIcon>
#include <QStandardPaths>

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
    //QT5 statusBar()->insertPermanentItem("", SRUN, 1);
    //QT5 statusBar()->insertPermanentItem(i18n("Time: 00:00"), STIME, 1);
    //QT5 statusBar()->insertPermanentItem(i18n("Size: 00 x 00"), SSIZE);
    //QT5 statusBar()->setItemAlignment(SRUN, Qt::AlignLeft | Qt::AlignVCenter);


	// Difficulty
	KGameDifficulty::global()->addStandardLevelRange(
		KGameDifficultyLevel::VeryEasy, KGameDifficultyLevel::ExtremelyHard,
		KGameDifficultyLevel::Medium //default
	);
	KGameDifficulty::global()->addLevel(new KGameDifficultyLevel(
		1000, QByteArray("config"), i18nc("Difficulty level title", "Configurable")
	));
	KGameDifficultyGUI::init(this);
	connect(KGameDifficulty::global(), &KGameDifficulty::currentLevelChanged, this, &KBBMainWindow::levelChanged);


	// Menu "Game"
	KGameStandardAction::gameNew(this, &KBBMainWindow::newGame, actionCollection());
	m_pauseAction = KGameStandardAction::pause(this, &KBBMainWindow::pause, actionCollection());
	QAction* tutorial = actionCollection()->addAction( QStringLiteral( "game_tutorial" ));
	tutorial->setText(i18nc("@action", "Start Tutorial"));
	tutorial->setIcon(QIcon::fromTheme( QStringLiteral( "footprint" )));
	tutorial->setToolTip(i18nc("@info:tooltip", "Start tutorial"));
	tutorial->setWhatsThis(i18n("<qt>The <b>tutorial</b> is a fast, user friendly and interactive way to learn the rules of the game. Start it if you do not know them!</qt>"));
	connect(tutorial, &QAction::triggered, this, &KBBMainWindow::startTutorial);
	KGameStandardAction::quit(this, &QWidget::close, actionCollection());
	QAction* sandbox = actionCollection()->addAction( QStringLiteral( "game_sandbox" ));
	sandbox->setText(i18nc("@action", "New Sandbox Game"));
	sandbox->setToolTip(i18nc("@info:tooltip", "Start a new sandbox game"));
	sandbox->setWhatsThis(i18n("<qt><p>In a <b>sandbox game</b>, the solution is displayed at the beginning of the game. This is useful to understand the game principles.</p><p>However: after a while, it is not really fun and you should try to start a real game!</p></qt>"));
	connect(sandbox, &QAction::triggered, this, &KBBMainWindow::startSandbox);
	KGameStandardAction::highscores(this, &KBBMainWindow::showHighscores, actionCollection());

	// Menu "Move"
	m_doneAction = actionCollection()->addAction( QStringLiteral( "move_done" ));
	m_doneAction->setText(i18nc("@action the last action of a game to check the result, when the user is done", "Done!"));
	m_doneAction->setWhatsThis(i18n("<qt><ul><li>First, you have to place all the balls on the black box. To guess the correct positions of the balls and see how they interact with laser beams, you should use the lasers that are positioned around the black box.</li><li><b>When you think you are done</b>, you should click here.</li></ul><p>Note that it is only possible to click here if you have placed the correct number of balls.</p></qt>"));
	m_doneAction->setIcon(QIcon::fromTheme( QStringLiteral( "dialog-ok" )));
	connect(m_doneAction, &QAction::triggered, this, &KBBMainWindow::done);
	m_solveAction = KGameStandardAction::solve(this, &KBBMainWindow::solve, actionCollection());
	m_solveAction->setToolTip(i18nc("@info:tooltip", "Give up the game"));
	m_solveAction->setWhatsThis(i18n("<qt><p>Choose \"<b>Solve</b>\" if you want to give up the current game. The solution will be displayed.</p><p>If you placed all the balls and do not want to give up, choose \"Done!\".</p></qt>"));

	// Menu "Settings"
	KStandardAction::preferences(this, &KBBMainWindow::settingsDialog, actionCollection());
	m_toggleCursorAction = actionCollection()->addAction( QStringLiteral( "toggle_cursor" ));
	m_toggleCursorAction->setText(i18nc("@option:check", "Enable Highlight under Mouse"));
	m_toggleCursorAction->setCheckable(true);
	const KConfigGroup group = KSharedConfig::openConfig()->group(QStringLiteral("default"));
	m_toggleCursorAction->setChecked(group.readEntry<bool>("highlight_enabled", true));
	connect(m_toggleCursorAction, &QAction::triggered, this, &KBBMainWindow::toggleCursor);

	// Theme manager
	QString svgzFile = KBBPrefs::theme();
	if (!QFile(svgzFile).exists())
        svgzFile = QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("themes/default.svgz"));
	m_themeManager = new KBBThemeManager(svgzFile);


	// Tutorial widget
	m_tutorial = new KBBTutorial(this);


	// Board
	m_gameDoc = new KBBGameDoc(this, m_tutorial);
	connect(m_gameDoc, &KBBGameDoc::updateStats, this, &KBBMainWindow::updateStats);
	connect(m_gameDoc, &KBBGameDoc::isRunning, this, &KBBMainWindow::setRunning);


	// Game widget
	m_gameWidget = new KBBScalableGraphicWidget(m_gameDoc, m_themeManager, m_doneAction);
	m_tutorial->setGameWidget(m_gameWidget, new KBBGraphicsItemTutorialMarker(m_gameWidget, m_themeManager, KBBTutorial::COLUMNS, KBBTutorial::ROWS));


	// Central Widget
	m_centralWidget = new QWidget(this);
	QHBoxLayout *widgetLayout = new QHBoxLayout();
	widgetLayout->setContentsMargins(0, 0, 0, 0);
	m_centralWidget->setLayout(widgetLayout);
	widgetLayout->addWidget(m_gameWidget);
	widgetLayout->addWidget(m_tutorial);
	setCentralWidget(m_centralWidget);


	// Keyboard only
	QAction * action = actionCollection()->addAction( QStringLiteral(  "move_down" ) );
	action->setText( i18nc("@action", "Move Down") );
	connect(action, &QAction::triggered, m_gameWidget, &KBBScalableGraphicWidget::keyboardMoveDown);
	KActionCollection::setDefaultShortcut(action, Qt::Key_Down);
	addAction(action);

	action = actionCollection()->addAction( QStringLiteral(  "move_up" ) );
	action->setText( i18nc("@action", "Move Up") );
	connect(action, &QAction::triggered, m_gameWidget, &KBBScalableGraphicWidget::keyboardMoveUp);
	KActionCollection::setDefaultShortcut(action,Qt::Key_Up);
	addAction(action);

	action = actionCollection()->addAction( QStringLiteral(  "move_left" ) );
	action->setText( i18nc("@action", "Move Left") );
	connect(action, &QAction::triggered, m_gameWidget, &KBBScalableGraphicWidget::keyboardMoveLeft);
	KActionCollection::setDefaultShortcut(action, Qt::Key_Left);
	addAction(action);

	action = actionCollection()->addAction( QStringLiteral(  "move_right" ) );
	action->setText( i18nc("@action", "Move Right") );
	connect(action, &QAction::triggered, m_gameWidget, &KBBScalableGraphicWidget::keyboardMoveRight);
	KActionCollection::setDefaultShortcut(action, Qt::Key_Right);
	addAction(action);

	action = actionCollection()->addAction( QStringLiteral( "switch_ball" ));
	action->setText(i18nc("@action", "Switch Ball or Shoot Laser"));
	connect(action, &QAction::triggered, m_gameWidget, &KBBScalableGraphicWidget::keyboardEnter);
	KActionCollection::setDefaultShortcut(action, Qt::Key_Return);
	addAction(action);

	action = actionCollection()->addAction( QStringLiteral( "switch_marker" ));
	action->setText(i18nc("@action", "Switch Marker"));
	connect(action, &QAction::triggered, m_gameWidget, &KBBScalableGraphicWidget::keyboardSpace);
	KActionCollection::setDefaultShortcut(action, Qt::Key_Space);
	addAction(action);


	m_gameClock = new KGameClock(this, KGameClock::MinSecOnly);
	connect(m_gameClock, &KGameClock::timeChanged, this, &KBBMainWindow::updateStats);
	connect(m_gameClock, &KGameClock::timeChanged, m_gameDoc, &KBBGameDoc::timeChanged);


	levelChanged();

	setupGUI();

	if (m_toggleCursorAction->isChecked())
		toggleCursor();

	// start a new game
	startGame(false);
}


KBBMainWindow::~KBBMainWindow()
{
	KBBPrefs::self()->save();

	delete m_gameWidget;
	delete m_themeManager;
}



//
// Public slots
//

void KBBMainWindow::levelChanged()
{
	KGameDifficultyLevel::StandardLevel level = KGameDifficulty::globalLevel();
	switch(level) {
		case KGameDifficultyLevel::VeryEasy:
			m_ballNumber = 2;
			m_columns = 6;
			m_rows = 6;
			break;
		case KGameDifficultyLevel::Easy:
		default:
			m_ballNumber = 4;
			m_columns = 8;
			m_rows = 8;
			level = KGameDifficultyLevel::Medium;
			break;
		case KGameDifficultyLevel::Medium:
			m_ballNumber = 6;
			m_columns = 10;
			m_rows = 10;
			break;
		case KGameDifficultyLevel::Hard:
			m_ballNumber = 8;
			m_columns = 12;
			m_rows = 12;
			break;
		case KGameDifficultyLevel::VeryHard:
			m_ballNumber = 11;
			m_columns = 14;
			m_rows = 10;
			break;
		case KGameDifficultyLevel::ExtremelyHard:
			m_ballNumber = 15;
			m_columns = 20;
			m_rows = 12;
			break;
		case KGameDifficultyLevel::Custom:
			m_gameWidget->popupText(i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "Note: You can change<br />the parameters of<br />custom games in the<br />Settings dialog."));
			break;
	}

	m_level = level;
	startGame(m_sandboxMode);
}


void KBBMainWindow::setRunning(bool r)
{
	// Difficulty
	KGameDifficulty::global()->setGameRunning(r);

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
		m_doneAction->setToolTip(i18n("To check if you successfully guessed the ball positions, click here!"));
	} else if (ballsLeftToPlace>0) {
		m_doneAction->setToolTip(i18np("You need to place 1 more ball on the black box.", "You need to place %1 more balls on the black box.", ballsLeftToPlace));
	}

	if (!m_boardEnabled)
		m_doneAction->setToolTip(i18n("Game over."));
	if (m_pauseAction->isChecked())
		m_doneAction->setToolTip(i18n("Game paused."));


	// Status bar
    if (m_tutorial->isVisible()) {
        //QT5 statusBar()->changeItem(i18n("Tutorial"), SRUN );
    }
	if (!m_tutorial->isVisible()) {
		if (m_boardEnabled) {
			if (ballsLeftToPlace<0) {
                //QT5 statusBar()->changeItem((i18np("1 ball too many!", "%1 balls too many!", -ballsLeftToPlace)), SRUN);
			} else if (ballsLeftToPlace==0) {
                //QT5 statusBar()->changeItem(i18n("No more balls to place"), SRUN);
			} else if (ballsLeftToPlace>0) {
                //QT5 statusBar()->changeItem(i18np("1 ball to place", "%1 balls to place", ballsLeftToPlace), SRUN);
			}
        } else {
            //QT5 statusBar()->changeItem(i18n("Game over"), SRUN );
        }
	}

    //QT5 statusBar()->changeItem(i18n("Time: %1", m_gameClock->timeString()), STIME);

    //QT5 statusBar()->changeItem( i18n("Size: %1 x %2", m_gameDoc->columns(), m_gameDoc->rows()), SSIZE );


	// 2. Info Widget
	m_gameWidget->setScore(m_gameDoc->score());
}



//
// Private slots
//

void KBBMainWindow::done()
{
	if (m_tutorial->isVisible() && !m_tutorial->maySolve()) {
		KMessageBox::information(this, i18n("Clicking on \"Done!\" is the normal way to check the positions of the balls at the end of the game. However, it is not possible in the tutorial to end the game before you reached the last step.\nPlease first finish the tutorial."), i18nc("@title:window", "Check Positions"));
	} else {
		solving();

		const int score = m_gameDoc->score();
		QString s;
		if (score <= (m_ballNumber*35)) {
			s = i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "Your final score is: %1.<br />You did really well!", score);
			if (m_sandboxMode)
                s += QLatin1String("<br /><br />") + i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "But it does not count<br />because <b>it is the sandbox!</b>");
		} else
			s = i18nc("The text may not be too wide. So please use some HTML-BR-tags to have something more or less as wide as in english. Thanks!", "Your final score is: %1.<br />I guess you need more practice.", score);

		if ((!m_tutorial->isVisible()) && (!m_sandboxMode) && (KGameDifficulty::globalLevel() != KGameDifficultyLevel::Custom) && (score<KBBGameDoc::SCORE_LOST)) {
			QPointer<KGameHighScoreDialog> scoreDialog = new KGameHighScoreDialog(KGameHighScoreDialog::Score | KGameHighScoreDialog::Name, this);
			scoreDialog->initFromDifficulty(KGameDifficulty::global());

			KGameHighScoreDialog::FieldInfo scoreInfo;
			scoreInfo[KGameHighScoreDialog::Score].setNum(score);
			if(scoreDialog->addScore(scoreInfo, KGameHighScoreDialog::LessIsMore) != 0)
				scoreDialog->exec();
			delete scoreDialog;
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
		m_gameWidget->popupText(i18n("Game paused.<br />Press \"%1\" to resume.", m_pauseAction->shortcut().toString(QKeySequence::NativeText)), 0);
	} else {
		m_gameClock->resume();
        m_gameWidget->popupText(QString());
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

	if (m_level==KGameDifficultyLevel::Custom) {
		bool mayRestart = true;
		if (m_gameDoc->gameReallyStarted())
			if (KMessageBox::questionTwoActions(this,
			    i18n("Do you want to cancel the current custom game and start a new one with the new parameters?"), QString(),
			    KGuiItem(i18nc("@action:button", "Start New Game"), QStringLiteral("view-refresh")),
			    KStandardGuiItem::cont())
			    == KMessageBox::SecondaryAction)
				mayRestart = false;

		if (mayRestart)
			startGame(m_sandboxMode);
	}
}


void KBBMainWindow::settingsDialog()
{
    if (!KConfigDialog::showDialog(QStringLiteral("settings"))) {
        KConfigDialog *dialog = new KConfigDialog(this, QStringLiteral("settings"), KBBPrefs::self());
		m_levelConfig = new KBBLevelConfigurationWidget(dialog, m_customBallNumber, m_customColumns, m_customRows, m_themeManager);
        dialog->addPage(m_levelConfig, i18nc("@title:tab", "Custom Game"), QStringLiteral("games-config-custom"));
		connect(dialog, &KConfigDialog::settingsChanged, this, &KBBMainWindow::settingsChanged);
                //QT5 dialog->setHelp(QString(), "kblackbox");
		dialog->show();
	}
}


void KBBMainWindow::showHighscores()
{
	KGameHighScoreDialog scoreDialog(KGameHighScoreDialog::Score | KGameHighScoreDialog::Name, this);
	scoreDialog.initFromDifficulty(KGameDifficulty::global());
	scoreDialog.exec();
}


void KBBMainWindow::solve()
{
	if (m_tutorial->isVisible() && !m_tutorial->maySolve()) {
		KMessageBox::information(this, i18n("Sorry, you may not give up during the tutorial."), i18nc("@title:window", "Solve"));
	} else {
		if (m_gameDoc->numberOfBallsToPlace()==m_gameDoc->numberOfBallsPlaced()) {
			if (KMessageBox::warningContinueCancel(this, i18n("You placed all the balls. Great!\nYou should now click on \"Done!\" to end the game and check if you guessed correctly.\nSo, do you really want to give up this game?"), QString(), KGuiItem(i18nc("@action:button", "Give Up"))) == KMessageBox::Continue)
				solving();
		} else if (KMessageBox::warningContinueCancel(this, i18np("You should place %1 ball!\n", "You should place %1 balls!\n", m_gameDoc->numberOfBallsToPlace()) + i18np("You have placed %1.\n", "You have placed %1.\n", m_gameDoc->numberOfBallsPlaced()) + i18n("Do you really want to give up this game?"), QString(), KGuiItem(i18nc("@action:button", "Give Up"))) == KMessageBox::Continue)
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
		KGameDifficulty::global()->setEditable(false);

		// Reset clock but don't start it yet.
		m_gameClock->restart();
		m_gameClock->pause();

		updateStats();
	}
}

void KBBMainWindow::toggleCursor()
{
	m_gameWidget->toggleCursor();

	KConfigGroup group = KSharedConfig::openConfig()->group(QStringLiteral("default"));
	group.writeEntry<bool>("highlight_enabled", m_toggleCursorAction->isChecked());
}


//
// Private
//

bool KBBMainWindow::mayAbortGame()
{
	bool mayAbort = true;

	if (m_gameDoc->gameReallyStarted())
		mayAbort = ( KMessageBox::warningContinueCancel(nullptr, i18n("This will be the end of the current game!"), QString(), KGuiItem(i18nc("@action:button", "Start New Game"), QStringLiteral("document-new"))) == KMessageBox::Continue );

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
	if (m_level==KGameDifficultyLevel::Custom) {
		m_ballNumber = m_customBallNumber;
		m_columns = m_customColumns;
		m_rows = m_customRows;
	}

	m_boardEnabled = true;
	m_sandboxMode = sandboxMode;

	m_solveAction->setEnabled(true);
	m_pauseAction->setChecked(false);
	KGameDifficulty::global()->setEditable(true);
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

#include "moc_kbbmainwindow.cpp"
