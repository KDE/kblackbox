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



#include <QFont>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>


#include <klocale.h>


#include "kbbballsgraphicwidget.h"
#include "kbbgraphicsitem.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBBallsGraphicWidget::KBBBallsGraphicWidget(const int ballSize, KBBThemeManager* themeManager)
{
	m_ballSize = ballSize;
	m_themeManager = themeManager;
	m_ballsToPlace = 0;
	m_ballWrong = NULL;
	
	setMaximumHeight(m_ballSize);
	setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	
	m_scene = new QGraphicsScene(this);
	setScene(m_scene);
	
	m_text = new QGraphicsTextItem(NULL, m_scene);
	m_text->setFont(QFont("Helvetica", 2*KBBScalableGraphicWidget::RATIO/3));
	
	setPlacedBalls(0);
}


KBBBallsGraphicWidget::~KBBBallsGraphicWidget()
{
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
		m_ballsToPlace = ballsToPlace;
		
		//setMaximumWidth(m_ballSize*m_ballsToPlace);
		m_scene->setSceneRect(0, 0, m_ballsToPlace*KBBScalableGraphicWidget::RATIO, KBBScalableGraphicWidget::RATIO);
		resizeEvent(NULL);
	}
}


void KBBBallsGraphicWidget::setPlacedBalls(const int placedBalls)
{
	int ballsLeftToPlace = m_ballsToPlace - placedBalls;
	
	// remove "wrong" ball
	if ((ballsLeftToPlace>=0) && (m_ballWrong!=NULL)) {
		delete m_ballWrong;
		m_scene->update();
		m_ballWrong = NULL;
	}
	
	// remove balls
	while ((ballsLeftToPlace>=0) && (ballsLeftToPlace<m_balls.count())) {
		delete m_balls.last();
		m_scene->update();
		m_balls.removeLast();
	}

	// add balls
	while (ballsLeftToPlace>m_balls.count()) {
		m_balls.append(new KBBGraphicsItem(KBBScalableGraphicWidget::playerBall, m_scene, m_themeManager));
		m_balls.last()->setPos((m_balls.count()-1)*KBBScalableGraphicWidget::RATIO,0);
	}
	
	// add "wrong" ball
	while ((ballsLeftToPlace<0) && (m_ballWrong==NULL)) {
		m_ballWrong = new KBBGraphicsItem(KBBScalableGraphicWidget::wrongPlayerBall, m_scene, m_themeManager);
	}
	if (ballsLeftToPlace<0) {
		m_text->setPlainText(i18n("x %1 too much!", -ballsLeftToPlace));
		m_text->setPos(KBBScalableGraphicWidget::RATIO,0);
	}

	if (ballsLeftToPlace==0) {
		m_text->setPlainText(i18n("You're done!"));
		m_text->setPos(0,0);
	}
	
	if (ballsLeftToPlace>0)
		m_text->setPlainText("");

}
