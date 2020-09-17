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



#ifndef KBBBALLSONBOARD_H
#define KBBBALLSONBOARD_H


#include <QList>
#include <QObject>


class KBBGameDoc;



#define DIM_X 0
#define DIM_Y 1
#define DIM_MAX 2



/**
 * @brief Set of balls (or various objects) with positions on the board
 *
 * The set of balls manages the position and the number of the balls. The balls can be placed of any kind (placed by the player or the hiden balls to find for instance).
 *
 * It computes also the trajectory of the laser ray with the given balls.
 *
 * There are 3 different kinds of coordinates for object positions.
 * - The 1st one is the (absolute) position in 2 dimensions between (0,0) and (2 + m_columns + 2, 2 + m_rows + 2). It is used to manage the positions of the graphic elements but also to calculate the laser ray trajectory.
 * - The 2nd one is the border position in 1 dimension between 0 and (2 * m_rows + 2 * m_columns -1). Only borders can be managed with this coordinate.
 * - The 3rd one is the box position in 1 dimension between 0 and (m_columns*m_rows - 1). It is used to manage the postion of the balls in the black box.
 */
class KBBBallsOnBoard : public QObject
{
	Q_OBJECT

	public:
		/**
		 * @brief Constructor
		 */
		KBBBallsOnBoard(KBBGameDoc* parent, const int columns, const int rows);


		/**
		 * @brief Convert (absolute) position to border position
		 *
		 * @param position The (absolute) position to convert.
		 * @return The result of the conversion: border position.
		 * @see borderPositionToAbsolutePosition(int borderPosition, int position[DIM_MAX])
		 * @see absolutePositionToBorderPosition(int position[DIM_MAX])
		 */
		int absolutePositionToBorderPosition(int position[DIM_MAX]);
		
		/**
		 * @brief Convert (absolute) position to box position
		 *
		 * @param position The (absolute) position to convert.
		 * @return The result of the conversion: box position.
		 * @see absolutePositionToBorderPosition(int position[DIM_MAX])
		 */
		int absolutePositionToBoxPosition(int position[DIM_MAX]);
		
		/**
		 * @brief Add a ball on this board
		 *
		 * @param boxPosition The box position of the ball to add
		 * @see remove(int boxPosition)
		 */
		void add(int boxPosition);
		
		/**
		 * @brief Convert border position to (abosulte) position
		 *
		 * @param borderPosition The border position to convert.
		 * @param position The result of the conversion: (absolute) position.
		 * @see borderPositionToAbsolutePosition(int position[DIM_MAX])
		 */
		void borderPositionToAbsolutePosition(int borderPosition, int position[DIM_MAX]);
		
		int columns();
		
		/**
		 * @brief Check if there is a ball at the given position in the black box
		 *
		 * @param boxPosition Box position to check
		 */
		bool contains(int boxPosition);
		
		/**
		 * @brief Number of balls on this board
		 */
		int count();
		
		/**
		 * @brief Define a new board and remove all balls
		 *
		 * @param columns Number of columns
		 * @param rows Number of rows
		 */
		void newBoard(const int columns, const int rows);
		
		/**
		 * @brief Compares 2 boards and return the number of differences
		 *
		 * @param otherBoard Other set of balls in a board
		 * @return Number of balls in the set that are not in the other given set
		 */
		int numberOfBallsNotIn(KBBBallsOnBoard* otherBoard);
		
		/**
		 * @brief Compute the opposite border position of the given position
		 *
		 * @param borderPosition The border position
		 */
		int oppositeBorderPosition(int borderPosition);
		
		int oppositeBorderPositionWithPoints(const int borderPosition, QList<int> &points);
		
		/**
		 * @brief Compute the trajectory of a ray with the balls of the set
		 *
		 * @param borderPosition The border position
		 */
		void ray(const int borderPosition, QList<int> &points);

		/**
		 * @brief Remove a ball on this board
		 *
		 * @param boxPosition The box position of the ball to be removed
		 * @see add(int boxPosition);
		 */
		void remove(int boxPosition);
		
		int rows();


	Q_SIGNALS:
		void changes();


	private:
		/**
		 * @brief Find the position where the laser ray leaves the black box
		 *
		 * @param position[DIM_MAX] Current incoming (absolute) position. It can be on a border or in the black box.
		 * @param incomingDirection[DIM_MAX] Direction to move in the black box as a vector (difference of (absolute) positions)
		 */
		void getOutgoingPosition( int position[DIM_MAX], int incomingDirection[DIM_MAX], QList<int> &points );
		
		/**
		 * @brief Check if the given (absolute) position is in the box
		 *
		 * @param position (Absolute) position to check
		 */
		bool positionInTheBox( int position[DIM_MAX] );
		
		
		QList<int> m_balls;
		int m_columns;
		int m_rows;
};

#endif // KBBBALLSONBOARD_H
