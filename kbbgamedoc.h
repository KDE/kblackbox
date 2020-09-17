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



#ifndef KBBGAMEDOC_H
#define KBBGAMEDOC_H

#include <QObject>
#include <QRandomGenerator>

class KBBBallsOnBoard;
#include "kbbmainwindow.h"
class KBBTutorial;



/**
 * @brief Game document (Logical board)
 *
 * The game document (=logical board) manages a game, that is:
 *    - the score
 *    - the balls the user placed on the board
 *    - the real (hidden) balls of the black box
 */
class KBBGameDoc : public QObject
{
	Q_OBJECT

	public:
		/**
		 * When a laser ray enter the black box, it exits on a defined border position, except if the laser ray hits a ball. In this case, it doesn't exit the black box. In this case, we defined the exit position as the HIT_POSITION.
		 * This position is "virtual" and must be different to all other possible "real" border positions. (That's why we defined it as a negative (<0) position).
		 */
		static const int HIT_POSITION = -1;

		static const int SCORE_LOST = 999;
		static const int SCORE_ONE = 3;
		static const int SCORE_TWO = 9;


		/**
		 * @brief Constructor
		 */
		KBBGameDoc(KBBMainWindow *parent, KBBTutorial* tutorial);


		/**
		 * @brief Get the number of columns
		 */
		int columns() const;

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
		
		bool mayShootRay(const int incomingPosition) const;

		/**
		 * @brief Create new board game and initialize game
		 * 
		 * @param balls Number of balls to place on the board
		 * @param columns Number of columns
		 * @param rows Number of rows
		 */
		void newGame(int balls, int columns, int rows);
		
		/**
		 * @brief Number of balls the user placed on the board
		 */
		int numberOfBallsPlaced();

		/**
		 * @brief Number of balls the user has to place on the board
		 */
		int numberOfBallsToPlace();

		/**
		 * @brief Get the number of rows
		 */
		int rows() const;

		/**
		 * @brief Get current score
		 */
		int score();

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

		/**
		 * @brief Initialize the tutorial
		 */
		void startTutorial();


		KBBBallsOnBoard* m_balls;
		KBBBallsOnBoard* m_ballsPlaced;


	public Q_SLOTS:
		void timeChanged();


	Q_SIGNALS:
		void isRunning(bool);
		void updateStats();


	private:
		void setRunning(bool r);

		/**
		 * @brief Sets the score value
		 *
		 * @param n New score
		 */
		void setScore( int n );

		void clean(const int columns, const int rows);

		int m_columns;
		bool m_gameReallyStarted;
		int m_rows;
		QRandomGenerator m_random;
		int m_score;
		KBBTutorial* m_tutorial;
};

#endif //KBBGAMEDOC_H
