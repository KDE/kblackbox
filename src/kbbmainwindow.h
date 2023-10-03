/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBLACKBOX_KBBMAINWINDOW_H
#define KBLACKBOX_KBBMAINWINDOW_H

class QAction;
class QWidget;

class KGameClock;
#include <kxmlguiwindow.h>
#include <KGameDifficulty>

class KBBGameDoc;
class KBBLevelConfigurationWidget;
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
		~KBBMainWindow() override;


	public Q_SLOTS:
		/**
		 * @brief Player changed the level
		 */
		void levelChanged();

		/**
		 * @brief Set if the game is running
		 */
		void setRunning(bool r);

		/**
		 * @brief Displays game statistics on the status bar
		 */
		void updateStats();

	private Q_SLOTS:
		/**
		 * @brief Ends the current game
		 *
		 * This function is used when the player is done.
		 * @see solve()
		 */
		void done();

		/**
		 * @brief Start a new game.
		 */
		void newGame();

		/**
		 * @brief Pause the game.
		 */
		void pause(bool state);

		/**
		 * @brief Settings changed
		 */
		void settingsChanged();

		/**
		 * @brief Show the settings dialog
		 */
		void settingsDialog();

		/**
		 * @brief Show the highscores
		 */
		void showHighscores();

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

		void toggleCursor();

	private:
		/**
		 * Statusbar IDs.
		 */
		static const int SRUN = 0;
		static const int STIME = 1;
		static const int SSIZE = 2;


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
		void startGame(bool newSandboxModeMode);


		// Actions
		QAction *m_doneAction;
		QAction *m_pauseAction;
		QAction *m_solveAction;
		QAction *m_toggleCursorAction;


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
		KGameClock* m_gameClock;
		KGameDifficultyLevel::StandardLevel m_level;
		KBBLevelConfigurationWidget* m_levelConfig;
		int m_rows;
		bool m_boardEnabled;
		bool m_sandboxMode;
		KBBThemeManager* m_themeManager;
		KBBTutorial* m_tutorial;
};

#endif // KBLACKBOX_KBBMAINWINDOW_H
