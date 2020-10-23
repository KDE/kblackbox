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


#include "kbbgraphicsitemlaser.h"

#include <QGraphicsSceneMouseEvent>



#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBGraphicsItemLaser::KBBGraphicsItemLaser(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, const int borderPosition, const int columns, const int rows) : KBBGraphicsItemBorder(borderPosition, columns, rows, 0), KBBGraphicsItem(((borderPosition<columns) ? KBBScalableGraphicWidget::laser0 : ((borderPosition<columns + rows) ? KBBScalableGraphicWidget::laser90 : ((borderPosition<2*columns + rows) ? KBBScalableGraphicWidget::laser180 : KBBScalableGraphicWidget::laser270))) , parent->scene(), themeManager), KBBItemWithPosition()
{
	m_widget = parent;
	
	const int radius = KBBScalableGraphicWidget::RATIO/2;
	if (rotationAngle()==90) {
		setPos(m_centerX - 3*radius, m_centerY - radius);
	} else if (rotationAngle()==180) {
		setPos(m_centerX - radius, m_centerY - 3*radius);
	} else {
		setPos(m_centerX - radius, m_centerY - radius);
	}
	
	setAcceptHoverEvents(true);
}



//
// Public
//

int KBBGraphicsItemLaser::position ()
{
	return m_borderPosition;
}



//
// Private
//

void KBBGraphicsItemLaser::hoverEnterEvent (QGraphicsSceneHoverEvent*)
{
	m_widget->drawRay(position());
}


void KBBGraphicsItemLaser::hoverLeaveEvent (QGraphicsSceneHoverEvent*)
{
	m_widget->removeRay();
}


void KBBGraphicsItemLaser::mousePressEvent (QGraphicsSceneMouseEvent* event)
{
	if (event->buttons()==Qt::LeftButton) {
		m_widget->mouseBorderClick(position());
	}
}
