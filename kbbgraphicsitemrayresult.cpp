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

#include "kbbgraphicsitemrayresult.h"



#include <QFont>
#include <QGraphicsScene>


#include "kbbgraphicsitem.h"
#include "kbbgraphicsitemborder.h"
#include "kbbitemwithposition.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBGraphicsItemRayResult::KBBGraphicsItemRayResult( KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, QGraphicsScene* scene, const int borderPosition, const int columns, const int rows, const int rayNumber) : KBBGraphicsItemBorder( borderPosition, columns, rows, KBBScalableGraphicWidget::BORDER_SIZE/2), KBBGraphicsItem(KBBScalableGraphicWidget::resultBackground, scene, themeManager), KBBItemWithPosition()
{
	m_widget = parent;
	m_scene = scene;
	m_elementIdResultBackground = themeManager->elementId(KBBScalableGraphicWidget::resultBackground);
	m_elementIdResultBackgroundHighlight = themeManager->elementId(KBBScalableGraphicWidget::resultBackgroundHighlight);
	m_number = nullptr;
	m_notNumber = nullptr;
	m_pause = false;
	
	float centerRadius = 3*KBBScalableGraphicWidget::RATIO/8.;
	float radius = KBBScalableGraphicWidget::BORDER_SIZE/4.;

	m_opposite = this;
	
	setPos(m_centerX - radius, m_centerY - radius);
	
	if(rayNumber<=0) {

		if (rayNumber==0)
			m_notNumber = new KBBGraphicsItem(KBBScalableGraphicWidget::resultReflection, m_scene, themeManager);
		else
			m_notNumber = new KBBGraphicsItem(KBBScalableGraphicWidget::resultHit, m_scene, themeManager);
		m_notNumber->setTransform(QTransform::fromTranslate(radius,radius), true);
		m_notNumber->setTransform(QTransform().rotate(rotationAngle()), true);
		m_notNumber->setTransform(QTransform::fromTranslate(-radius,-radius), true);
		m_notNumber->setPos(m_centerX - radius, m_centerY - radius);
	} else {
		QString text;
		text.setNum(rayNumber);

        m_number = new QGraphicsSimpleTextItem ( text, this);
		QFont font;
		font.setStyleHint(QFont::SansSerif);
		font.setWeight(QFont::DemiBold);
		float offset;
		if (rayNumber<10) {
			font.setPixelSize((int)(3*centerRadius/2));
			offset = 0.;
		} else {
			font.setPixelSize((int)(5*centerRadius/4));
			offset = 1.*centerRadius/6;
		}
		m_number->setFont(font);
		m_number->setPos(radius - centerRadius/2 - 2*offset, radius - centerRadius + offset);
		m_number->setZValue(themeManager->zValue(KBBScalableGraphicWidget::resultText));
	}
	setAcceptHoverEvents(true);
}



//
// Public
//

void KBBGraphicsItemRayResult::cleanDelete()
{
	delete m_notNumber;
	delete m_number;
	delete this;
}


void KBBGraphicsItemRayResult::highlight(bool state)
{
	if (state && !m_pause)
		setElementId(m_elementIdResultBackgroundHighlight);
	else
		setElementId(m_elementIdResultBackground);
}


void KBBGraphicsItemRayResult::highlightBoth(bool state)
{
	m_opposite->highlight(state);
	highlight(state);
}


int KBBGraphicsItemRayResult::position ()
{
	return m_borderPosition;
}


void KBBGraphicsItemRayResult::setOpposite(KBBGraphicsItemRayResult* opposite)
{
	m_opposite = opposite;
}


void KBBGraphicsItemRayResult::setPause(bool state)
{
	if (m_number!=nullptr)
		m_number->setVisible(!state);
	if (m_notNumber!=nullptr)
		m_notNumber->setVisible(!state);

	m_pause = state;
}



//
// Private
//

void KBBGraphicsItemRayResult::hoverEnterEvent (QGraphicsSceneHoverEvent*)
{
	highlightBoth(true);
	m_widget->drawRay(position());
}


void KBBGraphicsItemRayResult::hoverLeaveEvent (QGraphicsSceneHoverEvent*)
{
	highlightBoth(false);
	m_widget->removeRay();
}
