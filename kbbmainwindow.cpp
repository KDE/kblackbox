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



#include <QString>


#include <kactioncollection.h>
#include <kglobal.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandardgameaction.h>
#include <kstatusbar.h>
#include <ktoggleaction.h>


#include "kbbboard.h"
#include "kbbmainwindow.h"
#include "kbbprefs.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBMainWindow::KBBMainWindow()
{
	m_running = false;
	
	
	// Board
	m_board = new KBBBoard(this);
	m_gameWidget = new KBBScalableGraphicWidget(m_board);
	connect( m_board, SIGNAL(updateStats()), this, SLOT(updateStats()) );
	setCentralWidget(m_gameWidget);
	
	
	// Menu "Game"
	QAction *newAct = KStandardGameAction::gameNew(this, SLOT(newGame()), this);
	actionCollection()->addAction(newAct->objectName(), newAct);
	
	KStandardGameAction::quit(this, SLOT(close()), actionCollection());
	
	
	// Menu "Move"
	m_solveAction = KStandardGameAction::solve(this, SLOT(solve()), this);
	actionCollection()->addAction(m_solveAction->objectName(), m_solveAction);
	
	
	// Menu "Settings"
	m_sizeAction = new KSelectAction( i18n("&Size"), this);
	actionCollection()->addAction("options_size", m_sizeAction);
	connect(m_sizeAction, SIGNAL(triggered(int)), this, SLOT(slotSize(int)));
	QStringList list;
	list.append(i18n("  8 x  8 "));
	list.append(i18n(" 10 x 10 "));
	list.append(i18n(" 12 x 12 "));
	list.append(i18n(" 18 x 12 "));
	m_sizeAction->setItems(list);
	
	m_ballsAction = new KSelectAction( i18n("&Balls"), this);
	actionCollection()->addAction("options_balls", m_ballsAction);
	connect(m_ballsAction, SIGNAL(triggered(int)), this, SLOT(slotBalls(int)));
	list.clear();
	list.append(i18n("4 balls"));
	list.append(i18n("6 balls"));
	list.append(i18n("8 balls"));
	list.append(i18n("10 balls"));
	m_ballsAction->setItems(list);
	m_tutorialAction = new KToggleAction( i18n("&Tutorial"), this );
	actionCollection()->addAction( "options_tutorial", m_tutorialAction );
	connect(m_tutorialAction, SIGNAL(triggered(bool) ), SLOT(tutorialSwitch()));
	
	
	// Keyboard only
	QAction* action = actionCollection()->addAction( "move_down" );
	action->setText( i18n("Move Down") );
	connect(action, SIGNAL(triggered(bool) ), m_gameWidget, SLOT(slotDown()));
	action->setShortcut(Qt::Key_Down);
	addAction(action);
	
	action = actionCollection()->addAction( "move_up" );
	action->setText( i18n("Move Up") );
	connect(action, SIGNAL(triggered(bool) ), m_gameWidget, SLOT(slotUp()));
	action->setShortcut(Qt::Key_Up);
	addAction(action);
	
	action = actionCollection()->addAction( "move_left" );
	action->setText( i18n("Move Left") );
	connect(action, SIGNAL(triggered(bool) ), m_gameWidget, SLOT(slotLeft()));
	action->setShortcut(Qt::Key_Left);
	addAction(action);
	
	action = actionCollection()->addAction( "move_right" );
	action->setText( i18n("Move Right") );
	connect(action, SIGNAL(triggered(bool) ), m_gameWidget, SLOT(slotRight()));
	action->setShortcut(Qt::Key_Right);
	addAction(action);
	
	action = actionCollection()->addAction( "move_trigger" );
	action->setText( i18n("Trigger Action") );
	connect(action, SIGNAL(triggered(bool) ), m_gameWidget, SLOT(slotInput()));
	action->setShortcut(Qt::Key_Return);
	addAction(action);
	
	
	//Read configuration options
	m_ballNumber = KBBPrefs::balls();
	const int menuNumber[4] = {4, 6, 8, 10};
	for (int i=0; i<4; i++)
		if (menuNumber[i]==m_ballNumber)
			m_ballsAction->setCurrentItem(i);
	
	m_columns = KBBPrefs::columns();
	m_rows = KBBPrefs::rows();
	const int menuSizeColumns[4] = {8, 10, 12, 18};
	const int menuSizeRows[4] = {8, 10, 12, 12};
	for (int i=0; i<4; i++)
		if ((menuSizeColumns[i]==m_columns) && (menuSizeRows[i]==m_rows))
			m_sizeAction->setCurrentItem(i);
	
	m_tutorial = KBBPrefs::tutorial();
	m_tutorialAction->setChecked(m_tutorial);
	
	
	// Status bar
	statusBar()->insertItem(i18n("Score: 0000"), SSCORE);
	statusBar()->insertItem(i18n("Placed: 00 / 00"), SBALLS);
	statusBar()->insertItem(i18n("Run: yesno"), SRUN);
	statusBar()->insertItem(i18n("Size: 00 x 00"), SSIZE);
	
	
	newGame();
	setupGUI();
}


KBBMainWindow::~KBBMainWindow()
{
	KBBPrefs::writeConfig();
}



//
// Public slots
//

void KBBMainWindow::updateStats()
{
	if (m_running)
		statusBar()->changeItem(i18n("Run: Yes"), SRUN );
	else
		statusBar()->changeItem(i18n("Run: No"), SRUN );
	
	statusBar()->changeItem( i18n("Size: %1 x %2", m_columns, m_rows), SSIZE );
	
	statusBar()->changeItem( i18n("Placed: %1 / %2", m_board->numberOfBallsPlaced(), m_ballNumber), SBALLS );
	
	statusBar()->changeItem( i18n("Score: %1", m_board->getScore()), SSCORE );
}



//
// Private slots
//

void KBBMainWindow::newGame()
{
	startGame(m_ballNumber, m_columns, m_rows, m_tutorial);
}


void KBBMainWindow::slotBalls(int selection)
{
	const int ARRAY_SIZE = 4;
	const int newNumber[ARRAY_SIZE] = {4, 6, 8, 10};
	
	if (m_ballNumber != newNumber[selection]) {
		if (startGame(newNumber[selection], m_columns, m_rows, m_tutorial))
			KBBPrefs::setBalls(newNumber[selection]);
		else {
			for (int i=0; i<ARRAY_SIZE; i++)
				if (newNumber[i]==m_ballNumber)
					m_ballsAction->setCurrentItem(i);
		}
	}
}


void KBBMainWindow::slotSize(int selection)
{
	const int ARRAY_SIZE = 4;
	const int newSizesColumns[ARRAY_SIZE] = {8, 10, 12, 18};
	const int newSizesRows[ARRAY_SIZE] = {8, 10, 12, 12};

	if ((newSizesColumns[selection] != m_columns) || (newSizesRows[selection] != m_rows)) {
		if (startGame(m_ballNumber, newSizesColumns[selection], newSizesRows[selection], m_tutorial)) {
			KBBPrefs::setColumns(m_columns);
			KBBPrefs::setRows(m_rows);
		} else {
			for (int i=0; i<ARRAY_SIZE; i++)
				if ((newSizesColumns[i]==m_columns) && (newSizesRows[i]==m_rows))
					m_sizeAction->setCurrentItem(i);
		}
	}
}


void KBBMainWindow::solve()
{
	if (m_board->numberOfBallsPlaced() != m_ballNumber)
		if (KMessageBox::warningContinueCancel(this, i18n( "You should place %1 balls!\nYou have placed %2.\nDo you want to give up this game?", KGlobal::locale()->formatNumber(m_ballNumber, 0), KGlobal::locale()->formatNumber(m_board->numberOfBallsPlaced(), 0)), QString(), KGuiItem(i18n("Give Up"))) != KMessageBox::Continue)
			return;
	
	m_running = false;
	m_solveAction->setEnabled(false);
	m_board->gameOver();
	m_gameWidget->solve(m_board->m_balls);
	updateStats();
	
	
	if (m_board->numberOfBallsPlaced() == m_ballNumber) {
		const int score = m_board->getScore();
		QString s;
		if (score <= (m_ballNumber*3))
			s = i18n("Your final score is: %1\nYou did really well!", score);
		else
			s = i18n("Your final score is: %1\nI guess you need more practice.", score);
		KMessageBox::information(this, s);
	}
}


void KBBMainWindow::tutorialSwitch()
{
	if (startGame(m_ballNumber, m_columns, m_rows, !m_tutorial))
		KBBPrefs::setTutorial(m_tutorial);
	else
		m_tutorialAction->setChecked(m_tutorial);
}



//
// Private
//

bool KBBMainWindow::startGame(const int newBallNumber, const int newColumnNumber, const int newRowNumber, const bool newTutorialMode)
{
	bool start = true;
	if (m_board->gameReallyStarted())
		start = ( KMessageBox::warningContinueCancel(0, i18n("This will be the end of the current game!"), QString(), KGuiItem(i18n("End Game"))) == KMessageBox::Continue );
	
	if (start) {
		m_running = true;
		m_ballNumber = newBallNumber;
		m_columns = newColumnNumber;
		m_rows = newRowNumber;
		m_tutorial = newTutorialMode;

		m_solveAction->setEnabled(true);
		m_board->newGame(m_ballNumber, m_columns, m_rows, m_tutorial);
		m_gameWidget->newGame(m_columns, m_rows, m_board->m_ballsPlaced);
		if (m_tutorial)
			m_gameWidget->solve(m_board->m_balls);
		updateStats();
	}
	
	return start;
}



#include "kbbmainwindow.moc"
