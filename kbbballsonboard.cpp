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



#include <QObject>


#include "kbbballsonboard.h"
#include "kbbgamedoc.h"



//
// Constructor / Destructor
//

KBBBallsOnBoard::KBBBallsOnBoard(KBBGameDoc* parent, const int columns, const int rows) : QObject(parent)
{
	m_columns = columns;
	m_rows = rows;
}



//
// Public
//

int KBBBallsOnBoard::absolutePositionToBorderPosition(int position[DIM_MAX])
{
	int borderPosition = KBBGameDoc::HIT_POSITION;
	if (position[DIM_Y] == 1)
		borderPosition = position[DIM_X] - 2;
	else if (position[DIM_X] == m_columns + 2)
		borderPosition = position[DIM_Y] - 2 + m_columns;
	else if (position[DIM_Y] == m_rows + 2)
		borderPosition = 1 - position[DIM_X] + 2 * m_columns + m_rows;
	else if (position[DIM_X] == 1)
		borderPosition = 1 - position[DIM_Y] + 2*(m_rows + m_columns);
	
	return borderPosition;
}


int KBBBallsOnBoard::absolutePositionToBoxPosition(int position[DIM_MAX])
{
	return (position[DIM_X]-2)+(position[DIM_Y]-2)*(m_columns);
}


void KBBBallsOnBoard::add(int boxPosition)
{
	m_balls.append(boxPosition);
	emit changes();
}


void KBBBallsOnBoard::borderPositionToAbsolutePosition(int borderPosition, int position[DIM_MAX])
{
	if (borderPosition < m_columns) {
		position[DIM_X] = borderPosition + 2;
		position[DIM_Y] = 1;
	} else if ((borderPosition >= m_columns) && (borderPosition < m_columns + m_rows)) {
		position[DIM_X] = m_columns + 2;
		position[DIM_Y] = (borderPosition - m_columns) + 2;
	} else if ((borderPosition >= m_columns + m_rows) && (borderPosition < 2*m_columns + m_rows)) {
		position[DIM_X] = (m_columns - (borderPosition - m_columns - m_rows)) + 1;
		position[DIM_Y] = m_rows + 2;
	} else if (borderPosition >= 2*m_columns + m_rows) {
		position[DIM_X] = 1;
		position[DIM_Y] = (m_rows - (borderPosition - 2*m_columns - m_rows)) + 1;
	} else {
		position[DIM_X] = KBBGameDoc::HIT_POSITION;
		position[DIM_Y] = KBBGameDoc::HIT_POSITION;
	}
}


const int KBBBallsOnBoard::columns()
{
	return m_columns;
}


bool KBBBallsOnBoard::contains(int boxPosition)
{
	return m_balls.contains(boxPosition);
}


int KBBBallsOnBoard::count()
{
	return m_balls.count();
}


void KBBBallsOnBoard::newBoard(const int columns, const int rows)
{
	m_balls.clear();
	m_columns = columns;
	m_rows = rows;
	emit changes();
}


int KBBBallsOnBoard::numberOfBallsNotIn(KBBBallsOnBoard* otherBoard)
{
	int diff = 0;
	
	for (int i=0; i<m_balls.count(); i++)
		if (!otherBoard->contains(m_balls[i]))
			diff++;
	
	return diff;
}


int KBBBallsOnBoard::oppositeBorderPosition(int borderPosition) {
	QList<int> points;
	return oppositeBorderPositionWithPoints(borderPosition, points);
}


int KBBBallsOnBoard::oppositeBorderPositionWithPoints(const int borderPosition,  QList<int> &points) {
	// 1. Conversion "border position -> (Absolute) position"
	int position[DIM_MAX];
	borderPositionToAbsolutePosition(borderPosition, position);

	// 2. Get start direction
	int direction[DIM_MAX];
	if (borderPosition < m_columns) {
		direction[DIM_X] = 0;
		direction[DIM_Y] = 1;
	} else if ((borderPosition >= m_columns) && (borderPosition < m_columns + m_rows)) {
		direction[DIM_X] = -1;
		direction[DIM_Y] = 0;
	} else if ((borderPosition >= m_columns + m_rows) && (borderPosition < 2*m_columns + m_rows)) {
		direction[DIM_X] = 0;
		direction[DIM_Y] = -1;
	} else if (borderPosition >= 2*m_columns + m_rows) {
		direction[DIM_X] = 1;
		direction[DIM_Y] = 0;
	}

	// 3. Get the outgoing (absolute) position
	getOutgoingPosition(position, direction, points);
	
	// 4. Conversion "(absolute) position -> border position"
	return absolutePositionToBorderPosition(position);
}


void KBBBallsOnBoard::ray(const int borderPosition, QList<int> &points)
{
	oppositeBorderPositionWithPoints(borderPosition, points);
}


void KBBBallsOnBoard::remove(int boxPosition)
{
	m_balls.removeAll(boxPosition);
	emit changes();
}


const int KBBBallsOnBoard::rows()
{
	return m_rows;
}



//
// Private
//

void KBBBallsOnBoard::getOutgoingPosition( int position[DIM_MAX], int incomingDirection[DIM_MAX], QList<int> &points)
{
	int outgoingDirection[DIM_MAX];
	
	int nextPosition[DIM_MAX];
	nextPosition[DIM_X] = position[DIM_X] + incomingDirection[DIM_X];
	nextPosition[DIM_Y] = position[DIM_Y] + incomingDirection[DIM_Y];

	int nextLeftPosition[DIM_MAX];
	nextLeftPosition[DIM_X] = nextPosition[DIM_X] + incomingDirection[DIM_Y];
	nextLeftPosition[DIM_Y] = nextPosition[DIM_Y] + incomingDirection[DIM_X];

	int nextRightPosition[DIM_MAX];
	nextRightPosition[DIM_X] = nextPosition[DIM_X] - incomingDirection[DIM_Y];
	nextRightPosition[DIM_Y] = nextPosition[DIM_Y] - incomingDirection[DIM_X];

	bool deviation = false;
	if (positionInTheBox(nextPosition) && contains((nextPosition[DIM_X] - 2) + (nextPosition[DIM_Y] - 2) * m_columns)) {
		// HIT
		position[DIM_X] = KBBGameDoc::HIT_POSITION;
		position[DIM_Y] = KBBGameDoc::HIT_POSITION;
		points.append(absolutePositionToBoxPosition(nextPosition));
	} else if (positionInTheBox(nextLeftPosition) && contains((nextLeftPosition[DIM_X] - 2) + (nextLeftPosition[DIM_Y] - 2) * m_columns)) {
		// DEVIATION 1
		outgoingDirection[DIM_X] = -incomingDirection[DIM_Y];
		outgoingDirection[DIM_Y] = -incomingDirection[DIM_X];
		deviation = true;
	} else if (positionInTheBox(nextRightPosition) && contains((nextRightPosition[DIM_X] - 2) + (nextRightPosition[DIM_Y] - 2) * m_columns)) {
		// DEVIATION 2
		outgoingDirection[DIM_X] = incomingDirection[DIM_Y];
		outgoingDirection[DIM_Y] = incomingDirection[DIM_X];
		deviation = true;
	} else {
		//NORMAL
		position[DIM_X] = nextPosition[DIM_X];
		position[DIM_Y] = nextPosition[DIM_Y];
		outgoingDirection[DIM_X] = incomingDirection[DIM_X];
		outgoingDirection[DIM_Y] = incomingDirection[DIM_Y];
	}
	
	// Out of the Black box? (loop exit condition)
	if (positionInTheBox(position)) {
		points.append(absolutePositionToBoxPosition(position));
		getOutgoingPosition( position, outgoingDirection, points );
	} else if (deviation)
		// special case: Deviation entering the black box. Player should see the laser ray entering, even if it's in fact deviated before entering...
		points.append(absolutePositionToBoxPosition(nextPosition));
	
	return;
}


bool KBBBallsOnBoard::positionInTheBox( int position[DIM_MAX] )
{
	return !((position[DIM_X] < 2) || (position[DIM_X] > m_columns + 1) || (position[DIM_Y] < 2) || (position[DIM_Y] > m_rows + 1));
}

#include "kbbballsonboard.moc"
