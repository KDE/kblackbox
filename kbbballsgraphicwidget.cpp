//
// KBlackBox
//
// A simple game inspired by an emacs module
//
/***************************************************************************
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



#include <QGraphicsScene>
#include <QGraphicsView>


#include "kbbballsgraphicwidget.h"
#include "kbbgraphicsitem.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBBallsGraphicWidget::KBBBallsGraphicWidget(KBBThemeManager* themeManager)
{
	m_themeManager = themeManager;
	m_ballsToPlace = 0;
	
	setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
	
	m_scene = new QGraphicsScene(this);
	setScene(m_scene);
	
	setPlacedBalls(0);
}


KBBBallsGraphicWidget::~KBBBallsGraphicWidget()
{
	// This removes all graphic items on the view
	setPlacedBalls(m_ballsToPlace);
}



//
// Public
//

void KBBBallsGraphicWidget::resizeEvent(QResizeEvent*)
{
	fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}


void KBBBallsGraphicWidget::setBallsToPlace(const int ballsToPlace)
{
	if (m_ballsToPlace != ballsToPlace) {
		// 1. remove all balls
		m_ballsToPlace = 0;
		setPlacedBalls(0);
		
		// 2. set new value
		m_ballsToPlace = ballsToPlace;
		
		// 3. Set the scene size
		m_scene->setSceneRect(0, 0, KBBScalableGraphicWidget::RATIO, m_ballsToPlace*KBBScalableGraphicWidget::RATIO);
		resizeEvent(NULL);
	}
}


void KBBBallsGraphicWidget::setPlacedBalls(const int placedBalls)
{
	int ballsLeftToPlace = m_ballsToPlace - placedBalls;
	
	// remove "wrong" player balls
	while (((ballsLeftToPlace>=0) && (m_wrongPlayerBalls.count()>0)) || ((ballsLeftToPlace<0) && (m_wrongPlayerBalls.count()>-ballsLeftToPlace))) {
		delete m_wrongPlayerBalls.last();
		m_wrongPlayerBalls.removeLast();
	}
	
	// remove player balls
	while (((ballsLeftToPlace>=0) && (ballsLeftToPlace<m_playerBalls.count())) || ((ballsLeftToPlace<0) && (m_playerBalls.count()>0))) {
		delete m_playerBalls.last();
		m_playerBalls.removeLast();
	}

	// add balls
	while (ballsLeftToPlace>m_playerBalls.count()) {
		m_playerBalls.append(new KBBGraphicsItem(KBBScalableGraphicWidget::playerBall, m_scene, m_themeManager));
		m_playerBalls.last()->setPos(0, (m_ballsToPlace-m_playerBalls.count())*KBBScalableGraphicWidget::RATIO);
	}
	
	// add "wrong" ball
	while (-ballsLeftToPlace>m_wrongPlayerBalls.count()) {
		m_wrongPlayerBalls.append(new KBBGraphicsItem(KBBScalableGraphicWidget::wrongPlayerBall, m_scene, m_themeManager));
		m_wrongPlayerBalls.last()->setPos(0, (m_ballsToPlace-m_wrongPlayerBalls.count())*KBBScalableGraphicWidget::RATIO);
	}
	
	m_scene->update();
}
