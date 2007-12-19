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

#include "kbbgamedoc.h"





#include "kbbballsonboard.h"
#include "kbbtutorial.h"



//
// Constructor / Destructor
//

KBBGameDoc::KBBGameDoc(KBBMainWindow *parent, KBBTutorial* tutorial) : QObject(parent)
{
	setRunning(false);
	m_columns = 1;
	m_rows = 1;
	m_tutorial = tutorial;
	
	m_random.setSeed(0);
	
	m_balls = new KBBBallsOnBoard(this, m_columns, m_rows);
	m_ballsPlaced = new KBBBallsOnBoard(this, m_columns, m_rows);
	connect(m_ballsPlaced, SIGNAL(changes()), parent, SLOT(updateStats()));
}



//
// Public
//

int KBBGameDoc::columns() const
{
	return m_columns;
}


void KBBGameDoc::gameOver()
{
	// Clear
	setRunning(false);
	
	// Compute final score
	if (m_ballsPlaced->numberOfBallsNotIn(m_balls)>0)
		setScore(SCORE_LOST);
}


bool KBBGameDoc::gameReallyStarted()
{
	return m_gameReallyStarted;
}


bool KBBGameDoc::mayShootRay(const int incomingPosition) const
{
	if (m_tutorial->isVisible() && !m_tutorial->mayShootRay(incomingPosition))
		return false;
	else
		return true;
}


void KBBGameDoc::newGame(int balls, int columns, int rows)
{
	clean(columns, rows);

	// Puts the balls in the black box on random positions.
	int boxPos;
	for (int i = 0; i < balls; i++) {
		do {
			boxPos = m_random.getLong(m_columns * m_rows);
		} while (m_balls->contains(boxPos));
		m_balls->add(boxPos);
	}
}


int KBBGameDoc::numberOfBallsPlaced()
{
	return m_ballsPlaced->count();
}


int KBBGameDoc::numberOfBallsToPlace()
{
	return m_balls->count();
}


int KBBGameDoc::rows() const
{
	return m_rows;
}


int KBBGameDoc::score()
{
	return m_score;
}


int KBBGameDoc::shootRay( int borderPosition )
{
	int outgoingBorderPosition = m_balls->oppositeBorderPosition(borderPosition);
	
	if ((outgoingBorderPosition == HIT_POSITION) || (borderPosition == outgoingBorderPosition))
		setScore(m_score + SCORE_ONE);
	else
		setScore(m_score + SCORE_TWO);
	
	if (!m_tutorial->isVisible())
		setRunning(true);
	emit updateStats();

	return outgoingBorderPosition;
}


void KBBGameDoc::startTutorial()
{
	clean(KBBTutorial::COLUMNS, KBBTutorial::ROWS);
	m_balls->add(16);
	m_balls->add(21);
	m_balls->add(33);
	m_tutorial->setStep(1);
	m_tutorial->start();
}


void KBBGameDoc::timeChanged()
{
	setScore(m_score+1);
}



//
// Private
//

void KBBGameDoc::clean(const int columns, const int rows)
{
	m_columns = columns;
	m_rows = rows;

	// Clear
	setRunning(false);
	m_ballsPlaced->newBoard(m_columns, m_rows);
	setScore(-1); // -1 because a signal "timeChanged" will be send at the beginning and the score will be set to 0.

	m_balls->newBoard(m_columns, m_rows);
}


void KBBGameDoc::setRunning(const bool r)
{
	m_gameReallyStarted = r;
	emit isRunning(r);
}


void KBBGameDoc::setScore( int n )
{
	if (n<1000)
		m_score = n;
	else
		m_score = 999;
	emit updateStats();
}

#include "kbbgamedoc.moc"
