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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/



#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <QResizeEvent>
#include <QSvgRenderer>


#include <kstandarddirs.h>


#include "kbbboard.h"
#include "kbbgraphicsitemball.h"
#include "kbbgraphicsitemblackbox.h"
#include "kbbgraphicsitemlaser.h"
#include "kbbgraphicsitemrayresult.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBScalableGraphicWidget::KBBScalableGraphicWidget( KBBBoard *parent) : QGraphicsView( )
{
	m_board = parent;
	m_columns = 1;
	m_rows = 1;
	
	m_scene = new QGraphicsScene( 0, 0, 2*BORDER_SIZE, 2*BORDER_SIZE, this );
	m_scene->setBackgroundBrush(QColor(220,220,185));
	m_blackbox = new KBBGraphicsItemBlackBox(this, m_scene);
	m_ballSvg = new QSvgRenderer(KStandardDirs::locate("appdata", "pics/balls.svg"));
	
	this->setScene( m_scene );
}



//
// Public
//

void KBBScalableGraphicWidget::clickBlackBox(const int boxPosition)
{
	if (m_inputAccepted) {
		if (!m_balls.contains(boxPosition)) {
			m_board->addPlayerBall(boxPosition);
			m_balls.insert(boxPosition, new KBBGraphicsItemBall(this, m_scene, boxPosition, m_columns, m_ballSvg, KBBGraphicsItemBall::blue));
		} else {
			removeBall(m_balls[boxPosition]);
		}
	}
}


void KBBScalableGraphicWidget::clickLaser( KBBGraphicsItemLaser* laser )
{
	if (m_inputAccepted) {
		int incomingPosition = laser->borderPosition();
		int outgoingPosition = m_board->shootRay(incomingPosition);
	
		if (outgoingPosition != KBBBoard::HIT_POSITION) {
			int rayNumberOrReflection = 0;
			if (outgoingPosition!=incomingPosition) {
				m_rayNumber++;
				removeLaser(m_lasers[outgoingPosition]);
				m_rayResults.insert(outgoingPosition, new KBBGraphicsItemRayResult(this, m_scene, outgoingPosition, m_columns, m_rows, m_rayNumber));
				rayNumberOrReflection = m_rayNumber;
			}
			m_rayResults.insert(incomingPosition, new KBBGraphicsItemRayResult(this, m_scene, incomingPosition, m_columns, m_rows, rayNumberOrReflection));
			m_scene->update();
		}
		removeLaser(laser);
	}
}


int KBBScalableGraphicWidget::hHint() const
{
	return (m_rows*RATIO + 2*BORDER_SIZE)/2;
}


void KBBScalableGraphicWidget::newGame( const int columns, const int rows )
{
	m_rayNumber = 0;
	
	// remove old lasers and old ray results
	for (int i=0; i<2*(m_columns + m_rows); i++) {
		if (m_lasers.contains(i))
			removeLaser(m_lasers[i]);
		if (m_rayResults.contains(i))
			removeRayResult(m_rayResults[i]);
	}

	// remove all placed balls ans all solutions
	for (int i=0; i<(m_columns * m_rows); i++) {
		if (m_balls.contains(i)) 
			removeBall(m_balls[i]);
		if (m_ballsSolution.contains(i)) 
			removeBallSolution(m_ballsSolution[i]);
	}

	// set the new size
	m_columns = columns;
	m_rows = rows;
	m_blackbox->setSize(m_columns, m_rows);
	m_scene->setSceneRect(0, 0, m_columns*RATIO + 2*BORDER_SIZE, m_rows*RATIO + 2*BORDER_SIZE);

	// Place new lasers
	for (int i=0; i<2*(m_columns + m_rows); i++)
		m_lasers.insert(i, new KBBGraphicsItemLaser(this, m_scene, i, m_columns, m_rows));

}


void KBBScalableGraphicWidget::resizeEvent(  QResizeEvent* )
{
	this->fitInView(0, 0, m_columns*RATIO + 2*BORDER_SIZE, m_rows*RATIO + 2*BORDER_SIZE, Qt::KeepAspectRatio);
}


void KBBScalableGraphicWidget::solve()
{
	for (int i=0; i<(m_columns * m_rows); i++) {
		if (m_balls.contains(i) && !m_board->containsBall(i)) 
			m_ballsSolution.insert(i, new KBBGraphicsItemBall(this, m_scene, i, m_columns, m_ballSvg, KBBGraphicsItemBall::cross));
		if (!m_balls.contains(i) && m_board->containsBall(i)) 
			m_ballsSolution.insert(i, new KBBGraphicsItemBall(this, m_scene, i, m_columns, m_ballSvg, KBBGraphicsItemBall::red));
	}
}


int KBBScalableGraphicWidget::wHint() const
{
	return (m_columns*RATIO + 2*BORDER_SIZE)/2;
}



//
// Slots
//

void KBBScalableGraphicWidget::setInputAccepted( bool inputAccepted )
{
	m_inputAccepted = inputAccepted;
	if (m_inputAccepted) {
		setFocusPolicy( Qt::StrongFocus );
		setFocus();
	} else {
		setFocusPolicy( Qt::NoFocus );
		clearFocus();
	}
}


void KBBScalableGraphicWidget::slotDown()
{
	//TODO: Manage keyboard input
}


void KBBScalableGraphicWidget::slotInput()
{
	//TODO: Manage keyboard input
}


void KBBScalableGraphicWidget::slotLeft()
{
	//TODO: Manage keyboard input
}


void KBBScalableGraphicWidget::slotRight()
{
	//TODO: Manage keyboard input
}


void KBBScalableGraphicWidget::slotUp()
{
	//TODO: Manage keyboard input
}



//
// Private
//

void KBBScalableGraphicWidget::removeBall( KBBGraphicsItemBall* ball)
{
	m_scene->removeItem(ball);
	m_balls.remove(ball->boxPosition());
	m_board->removePlayerBall(ball->boxPosition());
	delete ball;
}


void KBBScalableGraphicWidget::removeBallSolution( KBBGraphicsItemBall* ball)
{
	m_scene->removeItem(ball);
	m_ballsSolution.remove(ball->boxPosition());
	delete ball;
}


void KBBScalableGraphicWidget::removeLaser( KBBGraphicsItemLaser* laser)
{
	m_scene->removeItem(laser);
	m_lasers.remove(laser->borderPosition());
	delete laser;
}


void KBBScalableGraphicWidget::removeRayResult( KBBGraphicsItemRayResult* rayResult)
{
	m_scene->removeItem(rayResult);
	m_rayResults.remove(rayResult->borderPosition());
	delete rayResult;
}


#include "kbbscalablegraphicwidget.moc"
