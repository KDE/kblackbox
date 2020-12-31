/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kbbgamedoc.h"





#include "kbbballsonboard.h"
#include "kbbtutorial.h"



//
// Constructor / Destructor
//

KBBGameDoc::KBBGameDoc(KBBMainWindow *parent, KBBTutorial* tutorial)
    : QObject(parent)
    , m_random(QRandomGenerator::global()->generate())
{
	setRunning(false);
	m_columns = 1;
	m_rows = 1;
	m_tutorial = tutorial;
	
	m_balls = new KBBBallsOnBoard(this, m_columns, m_rows);
	m_ballsPlaced = new KBBBallsOnBoard(this, m_columns, m_rows);
    connect(m_ballsPlaced, &KBBBallsOnBoard::changes, parent, &KBBMainWindow::updateStats);
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
			boxPos = m_random.bounded(m_columns * m_rows);
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
	Q_EMIT updateStats();

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
	Q_EMIT isRunning(r);
}


void KBBGameDoc::setScore( int n )
{
	if (n<1000)
		m_score = n;
	else
		m_score = 999;
	Q_EMIT updateStats();
}


