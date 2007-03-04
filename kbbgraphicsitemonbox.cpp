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


#include "kbbgraphicsitem.h"
#include "kbbgraphicsitemonbox.h"
#include "kbbgraphicsiteminteractioninfo.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemOnBox::KBBGraphicsItemOnBox( KBBScalableGraphicWidget* parent, const int boxPosition, const int columns, const int rows, itemType type) : KBBGraphicsItem()
{
	m_widget = parent;
	m_boxPosition = boxPosition;
	m_columns = columns;
	m_rows = rows;
	m_itemType = type;
	
	setSharedRenderer(m_widget->svgRenderer());
	setPos(KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition % columns), KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition / columns));
	
	switch (m_itemType) {
		case ball:
			// Do nothing. It's implemented in the sub-class KBBGraphicsItemBall
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
	
	m_widget->addItem(this);
}



//
// Public
//

const int KBBGraphicsItemOnBox::position ()
{
	return m_boxPosition;
}



//
// Private
//

void KBBGraphicsItemOnBox::mousePressEvent (QGraphicsSceneMouseEvent* event)
{
	if (m_itemType==nothing) {
		if (event->buttons()==Qt::LeftButton)
			m_widget->clickAddBall(position());
		else
			m_widget->clickRemoveBallNothing(position());
	}
}
