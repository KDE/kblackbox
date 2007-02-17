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



#include <QFont>
#include <QGraphicsScene>


#include "kbbboard.h"
#include "kbbgraphicsitem.h"
#include "kbbgraphicsitemborder.h"
#include "kbbgraphicsitemrayresult.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemRayResult::KBBGraphicsItemRayResult( KBBScalableGraphicWidget* parent, QGraphicsScene* scene, const int borderPosition, const int columns, const int rows, const int rayNumber) : KBBGraphicsItemBorder( borderPosition, columns, rows, KBBScalableGraphicWidget::BORDER_SIZE/2), KBBGraphicsItem()
{
	m_widget = parent;
	
	int centerRadius = 3*KBBScalableGraphicWidget::RATIO/8;
	int radius = KBBScalableGraphicWidget::BORDER_SIZE/4;

	m_opposite = this;
	
	setSharedRenderer(parent->svgRenderer());
	setElementId("result");
	setPos(m_centerX - radius, m_centerY - radius);
	parent->addItem(this);
	setZValue(KBBScalableGraphicWidget::ZVALUE_RAY_RESULT_BACKGROUND);
	
	
	// R for "Reflection". TODO: Draw a sign to symbolise it. Better for i18n and nicer anyway...
	QString text("R");
	if (rayNumber>0)
		text.setNum(rayNumber);
	if (rayNumber==KBBBoard::HIT_POSITION)
		// H for "Hit". TODO: Draw a sign to symbolise it. Better for i18n and nicer anyway...
		text = "H";
	m_number = new QGraphicsSimpleTextItem ( text, this, scene);
	QFont font;
	font.setStyleHint(QFont::SansSerif);
	font.setWeight(QFont::DemiBold);
	int offset;
	if (rayNumber<10) {
		font.setPixelSize(3*centerRadius/2);
		offset = 0;
	} else {
		font.setPixelSize(5*centerRadius/4);
		offset = 1*centerRadius/6;
	}
	m_number->setFont(font);
	m_number->setPos(radius - centerRadius/2 - 2*offset, radius - centerRadius + offset);
	m_number->setZValue(KBBScalableGraphicWidget::ZVALUE_RAY_RESULT_TEXT);
	
	setAcceptsHoverEvents(true);
}



//
// Public
//

void KBBGraphicsItemRayResult::highlight(bool state)
{
	if (state)
		setElementId("resulthighlight");
	else
		setElementId("result");
}



//
// Public
//

const int KBBGraphicsItemRayResult::position ()
{
	return m_borderPosition;
}


void KBBGraphicsItemRayResult::setOpposite(KBBGraphicsItemRayResult* opposite)
{
	m_opposite = opposite;
}


//
// Private
//

void KBBGraphicsItemRayResult::hoverEnterEvent (QGraphicsSceneHoverEvent*)
{
	m_opposite->highlight(true);
	highlight(true);
	m_widget->drawRay(position());
}


void KBBGraphicsItemRayResult::hoverLeaveEvent (QGraphicsSceneHoverEvent*)
{
	m_opposite->highlight(false);
	highlight(false);
	m_widget->removeRay();
}
