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



/*
 * Statusbar IDs.
 */
#define SSCORE 0
#define SBALLS 1
#define SRUN   2
#define SSIZE  3



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
  void updateStats();


private slots:
  void slotSize(int selection);
  void slotBalls(int selection);

  void tutorialSwitch();

  /**
   * @brief Start a new game afer user confirmation (if needed).
   */
  void newGame();

  bool setSize( int w, int h );
  bool setBalls( int n );
  void gameFinished();
  void abortGame();


private:
  /**
  * @brief As the user if he wants to end the game, if needed.
  * 
  * If the game is running and really started, the user has to confirm the end of the game.
  * If the game is not running or running but not really started, the game may end without user confirmation.
  */
  bool comfirmGameEndIfNeeded();

  /**
   * @brief Start a new game (without user comfirmation)
   * 
   * @see newGame()
   */
  void startGame();

  void initKAction();

  int balls;
  int m_columns;
  int m_rows;
  QAction *m_solveAction;
  KBBBoard *m_board;
  bool running;
  bool tutorial;

  KSelectAction *ballsAction, *sizeAction;
  KToggleAction *tutorialAction;
};

#endif // KBBMAINWINDOW_H
