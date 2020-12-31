/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kbbgraphicsitemball.h"



#include <QGraphicsSceneHoverEvent>
#include <QTimer>


#include "kbbgraphicsiteminteractioninfo.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBall::KBBGraphicsItemBall(KBBScalableGraphicWidget::itemType itemType, KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, int boxPosition, int columns, int rows) : KBBGraphicsItemOnBox( itemType, parent, themeManager, boxPosition, columns, rows)
{
	m_timer = nullptr;
	m_ballType = itemType;
	m_themeManager = themeManager;
	
	setAcceptHoverEvents(true);
	
	for (int i=0; i<8;i++)
		m_interactionInfos[i] = nullptr;
}


KBBGraphicsItemBall::~KBBGraphicsItemBall()
{
	removeInteractionInfos();
}



//
// Private slots
//

void KBBGraphicsItemBall::showInteractions()
{
	delete m_timer;
	m_timer = nullptr;
	
	const int offsetX[8] = {0, 1, 2, 2, 2, 1, 0, 0};
	const int offsetY[8] = {0, 0, 0, 1, 2, 2, 2, 1};
	qreal posX;
	qreal posY;
	
	// General interactions for every balls:
	for (int i=0; i<8;i++) {
		posX = x() - KBBScalableGraphicWidget::RATIO/2 + offsetX[i]*KBBScalableGraphicWidget::RATIO;
		posY = y() - KBBScalableGraphicWidget::RATIO/2 + offsetY[i]*KBBScalableGraphicWidget::RATIO;
		KBBScalableGraphicWidget::itemType type = KBBScalableGraphicWidget::interactionInfoDeflection;
		if (i%2 == 1)
			type = KBBScalableGraphicWidget::interactionInfoHit;
		m_interactionInfos[i] = new KBBGraphicsItemInteractionInfo(m_widget, m_themeManager, type, posX, posY, i*45 );
	}
	
	// If the ball is on a border:
	const KBBScalableGraphicWidget::itemType r = KBBScalableGraphicWidget::interactionInfoReflection;
	const KBBScalableGraphicWidget::itemType rS = KBBScalableGraphicWidget::interactionInfoReflectionSym;
	if (position()<m_columns) {
		m_interactionInfos[0]->setType(r);
		m_interactionInfos[2]->setType(rS);
	}
	if (position()>=m_columns*(m_rows-1)) {
		m_interactionInfos[4]->setType(r);
		m_interactionInfos[6]->setType(rS);
	}
	if (position()%m_columns == 0) {
		m_interactionInfos[6]->setType(r);
		m_interactionInfos[0]->setType(rS);
	}
	if (position()%m_columns == (m_columns-1)) {
		m_interactionInfos[2]->setType(r);
		m_interactionInfos[4]->setType(rS);
	}
	
	// If the ball is on a corner:
	const KBBScalableGraphicWidget::itemType n = KBBScalableGraphicWidget::interactionInfoNothing;
	if (position()==0)
		m_interactionInfos[0]->setType(n);
	if (position()==m_columns-1)
		m_interactionInfos[2]->setType(n);
	if (position()==m_rows*m_columns-1)
		m_interactionInfos[4]->setType(n);
	if (position()==(m_rows-1)*m_columns)
		m_interactionInfos[6]->setType(n);
}



//
// Private
//

void KBBGraphicsItemBall::hoverEnterEvent (QGraphicsSceneHoverEvent*)
{
	if (m_timer==nullptr) {
		m_timer = new QTimer(this);
		connect(m_timer, &QTimer::timeout, this, &KBBGraphicsItemBall::showInteractions);
		m_timer->start(TIME_TO_WAIT_BEFORE_SHOWING_INTERACTIONS);
	}

	m_widget->hoverMovePosition(position());
}


void KBBGraphicsItemBall::hoverLeaveEvent (QGraphicsSceneHoverEvent*)
{
	delete m_timer;
	m_timer = nullptr;
	removeInteractionInfos();
}


void KBBGraphicsItemBall::removeInteractionInfos()
{
	for (int i=0; i<8;i++) {
		delete m_interactionInfos[i];
		m_interactionInfos[i] = nullptr;
	}
}


