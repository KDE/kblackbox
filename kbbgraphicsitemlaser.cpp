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
#include <QGraphicsSvgItem>
#include <QSvgRenderer>


#include "kbbgraphicsitemborder.h"
#include "kbbgraphicsitemlaser.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemLaser::KBBGraphicsItemLaser( KBBScalableGraphicWidget* parent, QGraphicsScene* scene, const int borderPosition, const int columns, const int rows) : KBBGraphicsItemBorder( borderPosition, columns, rows, 0), QGraphicsSvgItem ( )
{
	m_widget = parent;

	setSharedRenderer(parent->svgRenderer());
	setElementId("laser");
	parent->addItem(this);
	setZValue(KBBScalableGraphicWidget::ZVALUE_LASER);

	int rotation;
	if (borderPosition<columns) {
		rotation = 0;
	} else if (borderPosition<columns + rows) {
		rotation = 90;
	} else if (borderPosition<2*columns + rows) {
		rotation = 180;
	} else {
		rotation = 270;
	}

	int radius = KBBScalableGraphicWidget::BORDER_SIZE/4;
	setPos(m_centerX - radius, m_centerY - radius);
	translate(radius,radius);
	rotate(rotation);
	translate(-radius,-radius);

	setAcceptsHoverEvents(true);
}



//
// Private
//


void KBBGraphicsItemLaser::hoverEnterEvent (QGraphicsSceneHoverEvent*)
{
	m_widget->drawRay(borderPosition());
}


void KBBGraphicsItemLaser::hoverLeaveEvent (QGraphicsSceneHoverEvent*)
{
	m_widget->removeRay();
}


void KBBGraphicsItemLaser::mousePressEvent (QGraphicsSceneMouseEvent* )
{
	m_widget->clickLaser(this);
	m_widget->removeRay();
}
