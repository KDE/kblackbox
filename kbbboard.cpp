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



#include <QWidget>


#include <kdebug.h>
#include <kmainwindow.h>
#include <krandomsequence.h>


#include "kbbboard.h"
#include "kbbgfx.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBBoard::KBBBoard(KMainWindow *parent)
{
	random.setSeed(0);

	// SWITCH THE CENTRAL WIDGET HERE
	//gr = new KBBGraphic( this );
	gr = new KBBScalableGraphicWidget( this );

	connect( gr, SIGNAL(endMouseClicked()), parent, SLOT(gameFinished()) );
	connect( gr, SIGNAL(addPlayerBall(int)), this, SLOT(addPlayerBall(int)) );
	connect( gr, SIGNAL(removePlayerBall(int)), this, SLOT(removePlayerBall(int)) );
}



//
// Public
//

int KBBBoard::absolutePositionToBorderPosition(int position[DIM_MAX])
{
	int borderPosition = HIT_POSITION;
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


void KBBBoard::borderPositionToAbsolutePosition(int borderPosition, int position[DIM_MAX]) 
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
		position[DIM_X] = HIT_POSITION;
		position[DIM_Y] = HIT_POSITION;
	}
}


void KBBBoard::gameOver()
{
	// Clear
	m_gameReallyStarted = false;
	
	// Compute final score
	for (int i=0;i<m_balls.count();i++)
		if (!m_ballsPlaced.contains(m_balls.value(i)))
			setScore( score + 5 );
	
	//Show solution (and indicate errors).
	gr->solve();
	
	// Deactivate central widget
	gr->setInputAccepted(false);
}


bool KBBBoard::gameReallyStarted()
{
	return m_gameReallyStarted;
}


void KBBBoard::getOutgoingPosition( int position[DIM_MAX], int incomingDirection[DIM_MAX] )
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


	if (positionInTheBox(nextPosition) && m_balls.contains((nextPosition[DIM_X] - 2) + (nextPosition[DIM_Y] - 2) * m_columns)) {
		// HIT
		position[DIM_X] = HIT_POSITION;
		position[DIM_Y] = HIT_POSITION;
	} else if (positionInTheBox(nextLeftPosition) && m_balls.contains((nextLeftPosition[DIM_X] - 2) + (nextLeftPosition[DIM_Y] - 2) * m_columns)) {
		// DEVIATION 1
		outgoingDirection[DIM_X] = -incomingDirection[DIM_Y];
		outgoingDirection[DIM_Y] = -incomingDirection[DIM_X];
	} else if (positionInTheBox(nextRightPosition) && m_balls.contains((nextRightPosition[DIM_X] - 2) + (nextRightPosition[DIM_Y] - 2) * m_columns)) {
		// DEVIATION 2
		outgoingDirection[DIM_X] = incomingDirection[DIM_Y];
		outgoingDirection[DIM_Y] = incomingDirection[DIM_X];
	} else {
		//NORMAL
		position[DIM_X] = nextPosition[DIM_X];
		position[DIM_Y] = nextPosition[DIM_Y];
		outgoingDirection[DIM_X] = incomingDirection[DIM_X];
		outgoingDirection[DIM_Y] = incomingDirection[DIM_Y];
	}
	
	
	// Out of the Black box? (loop exit condition)
	if (positionInTheBox(position))
		getOutgoingPosition( position, outgoingDirection );
	return;
}


int KBBBoard::getScore()
{
	return score;
}


QWidget* KBBBoard::getWidget()
{
	return gr;
}


int KBBBoard::getWidgetHeight()
{
	return gr->hHint();
}


int KBBBoard::getWidgetWidth()
{
	return gr->wHint();
}


void KBBBoard::newGame( int balls, int columns, int rows, bool tutorial )
{
	m_columns = columns;
	m_rows = rows;
	
	// Clear
	m_gameReallyStarted = false;
	m_ballsPlaced.clear();
	setScore( 0 );
	gr->newGame(m_columns, m_rows);
	
	// Puts the balls in the black box on random positions.
	m_balls.clear();
	int boxPos;
	for (int i = 0; i < balls; i++) {
		do {
			boxPos = random.getLong(m_columns * m_rows);
		} while (m_balls.contains(boxPos));
		m_balls.append(boxPos);
	}

	
	if (tutorial)
		gr->solve();
	
	// Activate central widget
	gr->setInputAccepted(true);
}


int KBBBoard::numberOfBallsPlaced()
{
	return m_ballsPlaced.count();
}


int KBBBoard::shootRay( int borderPosition )
{
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
	getOutgoingPosition(position, direction);
	
	// 4. Conversion "(absolute) position -> border position"
	int outgoingBorderPosition = absolutePositionToBorderPosition(position);
	
	// 5. Update the score
	if ((outgoingBorderPosition == HIT_POSITION) || (borderPosition == outgoingBorderPosition))
		setScore( score + 1);
	else
		setScore( score + 2);
	m_gameReallyStarted = true;
	emit updateStats();


	return outgoingBorderPosition;
}



//
// Slots
//

void KBBBoard::addPlayerBall( int boxPosition ) {
	if (!m_ballsPlaced.contains(boxPosition))
		m_ballsPlaced.append(boxPosition);
	emit updateStats();
}


bool KBBBoard::containsBall( int boxPosition ) {
	return m_balls.contains(boxPosition);
}


void KBBBoard::removePlayerBall( int boxPosition ) {
	m_ballsPlaced.removeAll(boxPosition);
	emit updateStats();
}



//
// Private
//

bool KBBBoard::positionInTheBox( int position[DIM_MAX] )
{
	return !((position[DIM_X] < 2) || (position[DIM_X] > m_columns + 1) || (position[DIM_Y] < 2) || (position[DIM_Y] > m_rows + 1));
}


void KBBBoard::setScore( int n )
{
	score = n;
	emit updateStats();
}

#include "kbbboard.moc"
