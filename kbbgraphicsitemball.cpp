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
	setPos( KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition % columns), KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition / columns));
	switch (type) {
		case blue:
			setZValue(2);
			setElementId("blueball");
			break;
		case red:
			setZValue(1);
			setElementId("redball");
			break;
		case cross:
			setZValue(3);
			setElementId("cross");
			break;
	}
	scene->addItem(this);
	scene->update();
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

void KBBGraphicsItemBall::mousePressEvent (QGraphicsSceneMouseEvent*)
{
	m_widget->clickBlackBox(boxPosition());
}


#include "kbbgraphicsitemball.moc"
