//
// KBlackbox
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



#ifndef KBBBOARD_H
#define KBBBOARD_H


#include <QObject>
class QWidget;


class KMainWindow;
#include <krandomsequence.h>


class KBBBallsOnBoard;
class KBBGraphic;
class KBBScalableGraphicWidget;



/**
 * @brief Logical board of the game
 *
 * The logical board manages a game, that is:
 *    - the score
 *    - the balls the user places on the board
 *    - the real (hidden) balls of the black box
 *
 * It contains also the graphic widget (that displays the game status and manages the user inputs).
 */
class KBBBoard : public QObject
{
	Q_OBJECT

	public:
		static const int HIT_POSITION = -1;
		
		
		explicit KBBBoard(KMainWindow *parent);
		
		
		/**
		 * @brief Stop the game, show solution and compute final score
		 */
		void gameOver();
		
		/**
		 * @brief Check if the player started to play
		 * 
		 * Check if the game is running and if the player shot at least one laser ray.
		 * Before that, the player doesn't need to confirm the end of the game, if he tries to start a new game.
		 */
		bool gameReallyStarted();
		
		/**
		 * @brief Get current score
		 */
		int getScore();
		
		/**
		 * @brief Access the central graphic widget of the game
		 */
		QWidget* getWidget();
		
		/**
		 * @brief Height of the central graphic widget of the game
		 */
		int getWidgetHeight();
		
		/**
		 * @brief Width of the central graphic widget of the game
		 */
		int getWidgetWidth();
		
		/**
		 * @brief Create new board game and initialize game
		 * 
		 * @param balls Number of balls to place on the board
		 * @param columns Number of columns
		 * @param rows Number of rows
		 * @param tutorial Define if the game is in tutorial mode
		 */
		void newGame( int balls, int columns, int rows, bool tutorial );
		
		/**
		 * @brief Number of balls the user placed on the board
		 */
		int numberOfBallsPlaced();
		
		/**
		 * @brief Shoot a ray
		 * 
		 * This is the main player action. A laser ray is shot from a border position, interacts with the balls in the black box and get out (or hit a ball).
		 * This costs 1 or 2 score points, depending where the laser ray exits.
		 *
		 * @param borderPosition Incoming border position, where the laser ray starts
		 * @return Outgoing border position, where the laser leaves the black box. If the laser hits a ball, the result is HIT_POSITION (that is not a valid border position). 
		 */
		int shootRay(int borderPosition);


	signals:
		void updateStats();


	private:
		/**
		 * @brief Sets the score value
		 *
		 * @param n New score
		 */
		void setScore( int n );

		// SWITCH THE CENTRAL WIDGET HERE
		//KBBGraphic *gr;
		KBBScalableGraphicWidget *gr;
		int m_columns;
		bool m_gameReallyStarted;
		int m_rows;
		KRandomSequence random;
		int score;
		KBBBallsOnBoard* m_balls;
		KBBBallsOnBoard* m_ballsPlaced;
};

#endif //KBBBOARD_H
