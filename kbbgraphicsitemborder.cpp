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
#include <QRectF>
#include <QSvgRenderer>


#include "kbbgraphicsitemborder.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBorder::KBBGraphicsItemBorder( KBBScalableGraphicWidget* parent, QGraphicsScene* scene, QSvgRenderer* svgRenderer, const QString svgId, const int borderPosition, const int columns, const int rows, const int offset) : QGraphicsSvgItem ( )
{
	m_widget = parent;
	m_borderPosition = borderPosition;
	
	int x;
	int y;
	if (m_borderPosition<columns) {
		x = m_borderPosition*KBBScalableGraphicWidget::RATIO + KBBScalableGraphicWidget::BORDER_SIZE;
		y = offset;
		m_rotation = 0;
	} else if (m_borderPosition<columns + rows) {
		x = (columns)*KBBScalableGraphicWidget::RATIO + KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::BORDER_SIZE/2 - offset;
		y = (m_borderPosition - columns)*KBBScalableGraphicWidget::RATIO + KBBScalableGraphicWidget::BORDER_SIZE;
		m_rotation = 90;
	} else if (m_borderPosition<2*columns + rows) {
		x = (2*columns + rows - m_borderPosition)*KBBScalableGraphicWidget::RATIO + KBBScalableGraphicWidget::BORDER_SIZE/2;
		y = (rows)*KBBScalableGraphicWidget::RATIO + 3*KBBScalableGraphicWidget::BORDER_SIZE/2 - offset;
		m_rotation = 180;
	} else {
		x = offset;
		y = (2*columns + 2*rows - m_borderPosition)*KBBScalableGraphicWidget::RATIO + KBBScalableGraphicWidget::BORDER_SIZE/2;
		m_rotation = 270;
	}
	
	m_centerX = x + KBBScalableGraphicWidget::RATIO/2;
	m_centerY = y + KBBScalableGraphicWidget::RATIO/2;

	setSharedRenderer(svgRenderer);
	setElementId(svgId);
	scene->addItem(this);
	setZValue(1);
	scene->update();
}



//
// Public
//

int KBBGraphicsItemBorder::borderPosition () const
{
	return m_borderPosition;
}
