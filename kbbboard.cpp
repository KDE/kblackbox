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



#include <QWidget>


#include <krandomsequence.h>


#include "kbbballsonboard.h"
#include "kbbboard.h"



//
// Constructor / Destructor
//

KBBBoard::KBBBoard(KBBMainWindow *parent) : QObject(parent)
{
	m_gameReallyStarted = false;
	m_columns = 1;
	m_rows = 1;
	
	random.setSeed(0);
		
	m_balls = new KBBBallsOnBoard(this, m_columns, m_rows);
	m_ballsPlaced = new KBBBallsOnBoard(this, m_columns, m_rows);
	connect(m_ballsPlaced, SIGNAL(changes()), parent, SLOT(updateStats()));
}



//
// Public
//

void KBBBoard::gameOver()
{
	// Clear
	m_gameReallyStarted = false;
	
	// Compute final score
	setScore( score + 5*m_ballsPlaced->numberOfBallsNotIn(m_balls) );
}


bool KBBBoard::gameReallyStarted()
{
	return m_gameReallyStarted;
}


int KBBBoard::getScore()
{
	return score;
}


void KBBBoard::newGame( int balls, int columns, int rows, bool tutorial )
{
	m_columns = columns;
	m_rows = rows;
	
	// Clear
	m_gameReallyStarted = false;
	m_ballsPlaced->newBoard(m_columns, m_rows);
	setScore( 0 );
	
	// Puts the balls in the black box on random positions.
	m_balls->newBoard(m_columns, m_rows);
	int boxPos;
	for (int i = 0; i < balls; i++) {
		do {
			boxPos = random.getLong(m_columns * m_rows);
		} while (m_balls->contains(boxPos));
		m_balls->add(boxPos);
	}
}


int KBBBoard::numberOfBallsPlaced()
{
	return m_ballsPlaced->count();
}


int KBBBoard::shootRay( int borderPosition )
{
	int outgoingBorderPosition = m_balls->oppositeBorderPosition(borderPosition);
	
	if ((outgoingBorderPosition == HIT_POSITION) || (borderPosition == outgoingBorderPosition))
		setScore( score + 1);
	else
		setScore( score + 2);
	m_gameReallyStarted = true;
	emit updateStats();

	return outgoingBorderPosition;
}



//
// Private
//

void KBBBoard::setScore( int n )
{
	score = n;
	emit updateStats();
}

#include "kbbboard.moc"
