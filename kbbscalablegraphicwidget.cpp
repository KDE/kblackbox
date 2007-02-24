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



#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPalette>
#include <QResizeEvent>


#include <kstandarddirs.h>


#include "kbbballsonboard.h"
#include "kbbboard.h"
#include "kbbgraphicsitemball.h"
#include "kbbgraphicsitemblackbox.h"
#include "kbbgraphicsitemlaser.h"
#include "kbbgraphicsitemray.h"
#include "kbbgraphicsitemrayresult.h"
#include "kbbgraphicsitemset.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBScalableGraphicWidget::KBBScalableGraphicWidget( KBBBoard* parent) : QGraphicsView()
{
	m_board = parent;
	m_columns = 1;
	m_rows = 1;
	m_ray = 0;
	m_raySolution = 0;
	
	m_svgRenderer.load(KStandardDirs::locate("appdata", "pics/kblackbox.svgz"));
	
	m_scene = new QGraphicsScene( 0, 0, 2*BORDER_SIZE, 2*BORDER_SIZE, this );
	m_blackbox = new KBBGraphicsItemBlackBox(this, m_scene);
	
	m_balls = new KBBGraphicsItemSet();
	m_ballsNothing = new KBBGraphicsItemSet();
	m_ballsSolution = new KBBGraphicsItemSet();
	m_ballsUnsure = new KBBGraphicsItemSet();
	m_lasers = new KBBGraphicsItemSet();
	m_rayResults = new KBBGraphicsItemSet();
	
	this->setScene( m_scene );
}



//
// Public
//

void KBBScalableGraphicWidget::addItem(QGraphicsItem* item) 
{
	m_scene->addItem(item);
	m_scene->update();
}


void KBBScalableGraphicWidget::clickAddBall(const int boxPosition)
{
	if (m_inputAccepted && (!m_balls->contains(boxPosition))) {
		m_boardBallsPlaced->add(boxPosition);
		m_balls->insert(new KBBGraphicsItemBall(this, boxPosition, m_columns, m_rows, KBBGraphicsItemBall::blue));
		m_ballsNothing->remove(boxPosition);
	}
}


void KBBScalableGraphicWidget::clickAddBallNothing(const int boxPosition)
{
	if (m_inputAccepted && (!m_ballsNothing->contains(boxPosition))) {
		m_ballsNothing->insert(new KBBGraphicsItemBall(this, boxPosition, m_columns, m_rows, KBBGraphicsItemBall::nothing));
		m_balls->remove(boxPosition);
		m_ballsUnsure->remove(boxPosition);
		m_boardBallsPlaced->remove(boxPosition);
	}
}


void KBBScalableGraphicWidget::clickLaser(const int incomingPosition)
{
	if (m_inputAccepted) {
		const int outgoingPosition = m_board->shootRay(incomingPosition);
		
		KBBGraphicsItemRayResult* inRay;
		KBBGraphicsItemRayResult* outRay;
		
		int rayNumberOrReflection = 0;
		if (outgoingPosition==KBBBoard::HIT_POSITION)
			rayNumberOrReflection = KBBBoard::HIT_POSITION;
		if ((outgoingPosition!=incomingPosition) && (outgoingPosition!=KBBBoard::HIT_POSITION)) {
			m_rayNumber++;
			m_lasers->remove(outgoingPosition);
			m_rayResults->insert(outRay = new KBBGraphicsItemRayResult(this, m_scene,  outgoingPosition, m_columns, m_rows, m_rayNumber));
			rayNumberOrReflection = m_rayNumber;
		}
		m_rayResults->insert(inRay = new KBBGraphicsItemRayResult(this, m_scene, incomingPosition, m_columns, m_rows, rayNumberOrReflection));
		
		if ((outgoingPosition!=incomingPosition) && (outgoingPosition!=KBBBoard::HIT_POSITION)) {
			inRay->setOpposite(outRay);
			outRay->setOpposite(inRay);
		}
		
		m_scene->update();
		m_lasers->remove(incomingPosition);
	}
}


void KBBScalableGraphicWidget::clickRemoveBall(const int boxPosition)
{
	if (m_inputAccepted) {
		m_balls->remove(boxPosition);
		m_ballsUnsure->remove(boxPosition);
		m_boardBallsPlaced->remove(boxPosition);
	}
}


void KBBScalableGraphicWidget::clickRemoveBallNothing(const int boxPosition)
{
	if (m_inputAccepted) {
		m_ballsNothing->remove(boxPosition);
	}
}


void KBBScalableGraphicWidget::clickSetBallUnsure(const int boxPosition, const bool unsure)
{
	if (m_inputAccepted) {
		if (unsure) {
			m_balls->remove(boxPosition);
			m_ballsUnsure->insert(new KBBGraphicsItemBall(this, boxPosition, m_columns, m_rows, KBBGraphicsItemBall::blueUnsure));
		} else {
			m_ballsUnsure->remove(boxPosition);
			m_balls->insert(new KBBGraphicsItemBall(this, boxPosition, m_columns, m_rows, KBBGraphicsItemBall::blue));
		}
	}
}


void KBBScalableGraphicWidget::drawRay(const int borderPosition)
{
	if (!m_inputAccepted) {
		m_raySolution = new KBBGraphicsItemRay(m_scene, borderPosition, m_boardBalls, KBBGraphicsItemRay::solutionRay);
		m_ray = new KBBGraphicsItemRay(m_scene, borderPosition, m_boardBallsPlaced, KBBGraphicsItemRay::playerSolutionRay);
	} else
		m_ray = new KBBGraphicsItemRay(m_scene, borderPosition, m_boardBallsPlaced, KBBGraphicsItemRay::playerRay);
}


int KBBScalableGraphicWidget::hHint() const
{
	return (m_rows*RATIO + 2*BORDER_SIZE)/2;
}


void KBBScalableGraphicWidget::newGame( const int columns, const int rows, KBBBallsOnBoard* balls, KBBBallsOnBoard* ballsPlaced )
{
	m_rayNumber = 0;
	m_boardBalls = balls;
	m_boardBallsPlaced = ballsPlaced;
	
	// remove old lasers, old ray results, all placed balls, all markers "nothing" and all solutions
	m_lasers->clear();
	m_rayResults->clear();
	m_balls->clear();
	m_ballsUnsure->clear();
	m_ballsNothing->clear();
	m_ballsSolution->clear();

	// set the new size
	m_columns = columns;
	m_rows = rows;
	m_blackbox->setSize(m_columns, m_rows);
	m_scene->setSceneRect(0, 0, m_columns*RATIO + 2*BORDER_SIZE, m_rows*RATIO + 2*BORDER_SIZE);
	resizeEvent(0);

	// Place new lasers
	for (int i=0; i<2*(m_columns + m_rows); i++)
		m_lasers->insert(new KBBGraphicsItemLaser(this, i, m_columns, m_rows));
}


void KBBScalableGraphicWidget::resizeEvent(  QResizeEvent* )
{
	this->fitInView(0, 0, m_columns*RATIO + 2*BORDER_SIZE, m_rows*RATIO + 2*BORDER_SIZE, Qt::KeepAspectRatio);
}


void KBBScalableGraphicWidget::removeRay()
{
	delete m_ray;
	m_ray = 0;
	delete m_raySolution;
	m_raySolution = 0;
}


void KBBScalableGraphicWidget::solve()
{
	for (int i=0; i<(m_columns * m_rows); i++) {
		if (m_balls->contains(i) && !m_boardBalls->contains(i))
			m_ballsSolution->insert(new KBBGraphicsItemBall(this, i, m_columns, m_rows, KBBGraphicsItemBall::cross));
		if (!m_balls->contains(i) && m_boardBalls->contains(i)) 
			m_ballsSolution->insert(new KBBGraphicsItemBall(this, i, m_columns, m_rows, KBBGraphicsItemBall::red));
	}
}


KSvgRenderer* KBBScalableGraphicWidget::svgRenderer() {
	return &m_svgRenderer;
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

#include "kbbscalablegraphicwidget.moc"
