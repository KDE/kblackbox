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
class QWidget;


#include <kgamedifficulty.h>
class KToggleAction;
#include <kxmlguiwindow.h>


class KBBGameDoc;
class KBBInfoWidget;
class KBBScalableGraphicWidget;
class KBBThemeManager;
class KBBTutorial;



/**
* @brief Main window of the game KBlackBox
*/
class KBBMainWindow : public KXmlGuiWindow
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

		/**
		 * @brief Player changed the level
		 */
		void levelChanged(KGameDifficulty::standardLevel level);


	private slots:
		/**
		 * @brief Ends the current game
		 *
		 * This function is used when the player is done.
		 * @see solve()
		 */
		void check();

		/**
		 * @brief Start a new game.
		 */
		void newGame();

		/**
		 * @brief Give up the current game
		 *
		 * This function is used when the player is giving up.
		 * @see check()
		 */
		void solve();

		/**
		 * @brief Start a game in the sandbox mode
		 */
		void startSandbox();

		/**
		 * @brief Start tutorial mode
		 */
		void startTutorial();


	private:
		/**
		 * Statusbar IDs.
		 */
		static const int SRUN = 0;
		static const int SSIZE = 1;


		/**
		 * @brief If needed, ask the player if he agrees to end the current game.
		 *
		 * @return if the current game may be aborted
		 */
		bool mayAbortGame();

		void solving();

		/**
		 * @brief Start a new game.
		 */
		void startGame(const int newBallNumber, const int newColumnNumber, const int newRowNumber, const bool newSandboxModeMode);


		// Actions
		QAction *m_solveAction;
		QAction *m_check;


		/**
		 * @brief The game document
		 */
		KBBGameDoc* m_gameDoc;

		/**
		 * @brief The game main widget
		 */
		KBBScalableGraphicWidget* m_gameWidget;


		// Custom difficulty level
		int m_customBallNumber;
		int m_customColumns;
		int m_customRows;

		// Various member variables
		int m_ballNumber;
		QWidget* m_centralWidget;
		int m_columns;
		KBBInfoWidget* m_infoWidget;
		KGameDifficulty::standardLevel m_level;
		int m_rows;
		bool m_running;
		bool m_sandboxMode;
		KBBThemeManager* m_themeManager;
		KBBTutorial* m_tutorial;
};

#endif // KBBMAINWINDOW_H
