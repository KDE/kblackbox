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



#ifndef KBBMAINWINDOW_H
#define KBBMAINWINDOW_H


class QAction;


#include <kmainwindow.h>
class KSelectAction;
class KToggleAction;


class KBBBoard;
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
* @brief Main window of the game KBlackBox
*/
class KBBMainWindow : public KMainWindow
{
	Q_OBJECT


	public:
		KBBMainWindow();
		~KBBMainWindow();


	public slots:
		/**
		 * @brief Displays game statistics on the status bar
		 */
		void updateStats();


	private slots:
		/**
		 * @brief Start a new game.
		 */
		void newGame();
		
		void slotSize(int selection);
		void slotBalls(int selection);
		
		/**
		 * @brief Ends the current game
		 * 
		 * This function is used when the player is giving up or when he is done.
		 */
		void solve();
		
		void tutorialSwitch();


	private:
		/**
		 * Statusbar IDs.
		 */
		static const int SSCORE = 0;
		static const int SBALLS = 1;
		static const int SRUN = 2;
		static const int SSIZE = 3;
		
		
		/**
		 * @brief Start a new game afer user confirmation (if needed).
		 *
		 * If the game is running and really started, the user has to confirm the end of the game.
		 * If the game is not running or running but not really started, the game may end and a new game may start without user confirmation.
		 * 
		 * @return if a new game started or not
		 */
		bool startGame(const int newBallNumber, const int newColumnNumber, const int newRowNumber, const bool newTutorialMode);
		
		
		// Actions
		KSelectAction *m_ballsAction;
		KSelectAction *m_sizeAction;
		QAction *m_solveAction;
		KToggleAction *m_tutorialAction;
		
		
		/**
		 * @brief The game document
		 */
		KBBBoard* m_board;
		
		/**
		 * @brief The game main widget
		 */
		KBBScalableGraphicWidget* m_gameWidget;
		
		
		// Various member variables
		int m_ballNumber;
		int m_columns;
		int m_rows;
		bool m_running;
		bool m_tutorial;
		KBBThemeManager* m_themeManager;
};

#endif // KBBMAINWINDOW_H
