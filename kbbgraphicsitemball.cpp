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



#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>


#include "kbbgraphicsitemball.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBall::KBBGraphicsItemBall( KBBScalableGraphicWidget* parent, QGraphicsScene* scene, const int boxPosition, const int columns, QSvgRenderer* svgRenderer, ballType type) : QGraphicsSvgItem ( 0 )
{
	m_widget = parent;
	m_boxPosition = boxPosition;
	setSharedRenderer(svgRenderer);
	setPos(KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition % columns), KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition / columns));
	scene->addItem(this);
	setBallType(type);
}



//
// Public
//

int KBBGraphicsItemBall::boxPosition () const
{
	return m_boxPosition;
}



//
// Private
//

void KBBGraphicsItemBall::mousePressEvent (QGraphicsSceneMouseEvent* event)
{
	if (event->buttons()==Qt::LeftButton) {
		switch(m_ballType) {
			case red:
				m_widget->clickAddBall(boxPosition());
				break;
			case blue:
				m_widget->clickRemoveBall(boxPosition());
				break;
			case blueUnsure:
				setBallType(blue);
				break;
			case nothing:
				m_widget->clickAddBall(boxPosition());
				break;
			case cross:
				break;
		}
	} else {
		switch(m_ballType) {
			case red:
				m_widget->clickAddBallNothing(boxPosition());
				break;
			case blue:
				setBallType(blueUnsure);
				break;
			case blueUnsure:
				m_widget->clickAddBallNothing(boxPosition());
				break;
			case nothing:
				m_widget->clickRemoveBallNothing(boxPosition());
				break;
			case cross:
				break;
		}
	}
}


void KBBGraphicsItemBall::setBallType(ballType type)
{
	m_ballType = type;
	switch (m_ballType) {
		case blue:
			setZValue(KBBScalableGraphicWidget::ZVALUE_BALL_BLUE);
			setElementId("blueball");
			break;
		case blueUnsure:
			setZValue(KBBScalableGraphicWidget::ZVALUE_BALL_BLUE);
			setElementId("blueballunsure");
			break;
		case red:
			setZValue(KBBScalableGraphicWidget::ZVALUE_BALL_RED);
			setElementId("redball");
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
}

#include "kbbgraphicsitemball.moc"
