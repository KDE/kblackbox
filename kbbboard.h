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
#include <QList>
class QWidget;


class KMainWindow;
#include <krandomsequence.h>


class KBBGraphic;
class KBBScalableGraphicWidget;



#define DIM_X 0
#define DIM_Y 1
#define DIM_MAX 2



/**
 * @brief Logical board of the game
 *
 * The logical board manages:
 *    - the score
 *    - the position and the number of the balls the user places on the board
 *    - the position of the real (hidden) balls of the black box
 *
 * It computes also the trajectory of the laser ray.
 * It contains also the graphic widget (that displays the game status and manages the user inputs).
 *
 * There are 3 different kinds of coordinates for object positions.
 * - The 1st one is the (absolute) position in 2 dimensions between (0,0) and (2 + m_columns + 2, 2 + m_rows + 2). It is used to manage the positions of the graphic elements but also to calculate the laser ray trajectory.
 * - The 2nd one is the border position in 1 dimension between 0 and (2 * m_rows + 2 * m_columns -1). Only borders can be managed with this coordinate.
 * - The 3rd one is the box position in 1 dimension between 0 and (m_columns*m_rows - 1). It is used to manage the postion of the balls in the black box.
 */
class KBBBoard : public QObject
{
	Q_OBJECT

	public:
		KBBBoard(KMainWindow *parent);
		
		
		static const int HIT_POSITION = -1;
		
		
		/**
		 * @brief Convert (absolute) position to border position
		 *
		 * @param position The (absolute) position to convert.
		 * @return The result of the conversion: border position.
		 * @see borderPositionToAbsolutePosition(int borderPosition, int position[DIM_MAX])
		 */
		int absolutePositionToBorderPosition(int position[DIM_MAX]);	

		/**
		 * @brief Convert border position to (abosulte) position
		 *
		 * @param borderPosition The border position to convert.
		 * @param position The result of the conversion: (absolute) position.
		 * @see borderPositionToAbsolutePosition(int position[DIM_MAX])
		 */
		void borderPositionToAbsolutePosition(int borderPosition, int position[DIM_MAX]);
		
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
		 * @brief Find the position where the laser ray leaves the black box
		 *
		 * @param position[DIM_MAX] Current incoming (absolute) position. It can be on a border or in the black box.
		 * @param incomingDirection[DIM_MAX] Direction to move in the black box as a vector (difference of (absolute) positions)
		 */
		void getOutgoingPosition( int position[DIM_MAX], int incomingDirection[DIM_MAX] );
		
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


	public slots:
		/**
		 * @brief Player places a new ball on the board
		 *
		 * @param boxPosition Box position of the player ball
		 */
		void addPlayerBall( int boxPosition );
		
		/**
		 * @brief Check if there is a ball at the given position in the black box
		 *
		 * @param boxPosition Box position to check
		 */
		bool containsBall( int boxPosition );
		
		/**
		 * @brief Player removes a ball from the board
		 *
		 * If there is no ball at the given position, nothing happens.
		 *
		 * @param boxPosition Box position of the ball to remove
		 */
		void removePlayerBall( int boxPosition );


	private:
		/**
		 * @brief Check if the given (absolute) position is in the box
		 *
		 * @param position (Absolute) position to check
		 */
		bool positionInTheBox( int position[DIM_MAX] );
		
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
		QList<int> m_balls;
		QList<int> m_ballsPlaced;
};

#endif //KBBBOARD_H
