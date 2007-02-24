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



#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>


#include "kbbgraphicsitem.h"
#include "kbbgraphicsitemball.h"
#include "kbbgraphicsiteminteractioninfo.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBall::KBBGraphicsItemBall( KBBScalableGraphicWidget* parent, const int boxPosition, const int columns, const int rows, ballType type) : KBBGraphicsItem()
{
	m_widget = parent;
	m_boxPosition = boxPosition;
	m_columns = columns;
	m_rows = rows;
	m_ballType = type;
	m_timer = NULL;
	
	setSharedRenderer(m_widget->svgRenderer());
	setPos(KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition % columns), KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition / columns));
	m_widget->addItem(this);
	
	switch (m_ballType) {
		case blue:
			setZValue(KBBScalableGraphicWidget::ZVALUE_BALL_BLUE);
			setElementId("blueball");
			setAcceptsHoverEvents(true);
			break;
		case blueUnsure:
			setZValue(KBBScalableGraphicWidget::ZVALUE_BALL_BLUE);
			setElementId("blueballunsure");
			setAcceptsHoverEvents(true);
			break;
		case red:
			setZValue(KBBScalableGraphicWidget::ZVALUE_BALL_RED);
			setElementId("redball");
			setAcceptsHoverEvents(true);
			break;
		case cross:
			setZValue(KBBScalableGraphicWidget::ZVALUE_BALL_CROSS);
			setElementId("cross");
			break;
		case nothing:
			setZValue(KBBScalableGraphicWidget::ZVALUE_MARKER_NOTHING);
			setElementId("nothing");
			break;
	}
	
	for (int i=0; i<8;i++)
		m_interactionInfos[i] = NULL;
}



//
// Public
//

const int KBBGraphicsItemBall::position ()
{
	return m_boxPosition;
}



//
// Private slots
//

void KBBGraphicsItemBall::showInteractions()
{
	delete m_timer;
	m_timer = NULL;
	
	const int offsetX[8] = {0, 1, 2, 2, 2, 1, 0, 0};
	const int offsetY[8] = {0, 0, 0, 1, 2, 2, 2, 1};
	qreal posX;
	qreal posY;
	
	// General interactions for every balls:
	for (int i=0; i<8;i++) {
		posX = x() - KBBScalableGraphicWidget::RATIO/2 + offsetX[i]*KBBScalableGraphicWidget::RATIO;
		posY = y() - KBBScalableGraphicWidget::RATIO/2 + offsetY[i]*KBBScalableGraphicWidget::RATIO;
		KBBGraphicsItemInteractionInfo::interactionType type = KBBGraphicsItemInteractionInfo::deflection;
		if (i%2 == 1)
			type = KBBGraphicsItemInteractionInfo::hit;
		m_interactionInfos[i] = new KBBGraphicsItemInteractionInfo(m_widget, type, posX, posY, i*45 );
	}
	
	// If the ball is on a border:
	const KBBGraphicsItemInteractionInfo::interactionType r = KBBGraphicsItemInteractionInfo::reflection;
	const KBBGraphicsItemInteractionInfo::interactionType rS = KBBGraphicsItemInteractionInfo::reflectionSym;
	if (m_boxPosition<m_columns) {
		m_interactionInfos[0]->setType(r);
		m_interactionInfos[2]->setType(rS);
	}
	if (m_boxPosition>=m_columns*(m_rows-1)) {
		m_interactionInfos[4]->setType(r);
		m_interactionInfos[6]->setType(rS);
	}
	if (m_boxPosition%m_columns == 0) {
		m_interactionInfos[6]->setType(r);
		m_interactionInfos[0]->setType(rS);
	}
	if (m_boxPosition%m_columns == (m_columns-1)) {
		m_interactionInfos[2]->setType(r);
		m_interactionInfos[4]->setType(rS);
	}
	
	// If the ball is on a corner:
	const KBBGraphicsItemInteractionInfo::interactionType n = KBBGraphicsItemInteractionInfo::nothing;
	if (m_boxPosition==0)
		m_interactionInfos[0]->setType(n);
	if (m_boxPosition==m_columns-1)
		m_interactionInfos[2]->setType(n);
	if (m_boxPosition==m_rows*m_columns-1)
		m_interactionInfos[4]->setType(n);
	if (m_boxPosition==(m_rows-1)*m_columns)
		m_interactionInfos[6]->setType(n);
}



//
// Private
//

void KBBGraphicsItemBall::hoverEnterEvent (QGraphicsSceneHoverEvent*)
{
	if (m_timer==NULL) {
		m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(showInteractions()));
		m_timer->start(TIME_TO_WAIT_BEFORE_SHOWING_INTERACTIONS);
	}
}


void KBBGraphicsItemBall::hoverLeaveEvent (QGraphicsSceneHoverEvent*)
{
	delete m_timer;
	m_timer = NULL;
	removeInteractionInfos();
}


void KBBGraphicsItemBall::mousePressEvent (QGraphicsSceneMouseEvent* event)
{
	if (event->buttons()==Qt::LeftButton) {
		switch(m_ballType) {
			case red:
				m_widget->clickAddBall(position());
				break;
			case blue:
				m_widget->clickRemoveBall(position());
				break;
			case blueUnsure:
				m_widget->clickSetBallUnsure(position(), false);
				break;
			case nothing:
				m_widget->clickAddBall(position());
				break;
			case cross:
				break;
		}
	} else {
		switch(m_ballType) {
			case red:
				m_widget->clickAddBallNothing(position());
				break;
			case blue:
				m_widget->clickSetBallUnsure(position(), true);
				break;
			case blueUnsure:
				m_widget->clickAddBallNothing(position());
				break;
			case nothing:
				m_widget->clickRemoveBallNothing(position());
				break;
			case cross:
				break;
		}
	}
}


void KBBGraphicsItemBall::removeInteractionInfos()
{
	for (int i=0; i<8;i++) {
		delete m_interactionInfos[i];
		m_interactionInfos[i] = NULL;
	}
}



#include "kbbgraphicsitemball.moc"
