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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <QFont>
#include <QGraphicsScene>


#include <kdebug.h>


#include "kbbgraphicsitemborder.h"
#include "kbbgraphicsitemrayresult.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemRayResult::KBBGraphicsItemRayResult( KBBScalableGraphicWidget* parent, QGraphicsScene* scene, const int borderPosition, const int columns, const int rows, const int rayNumber) : KBBGraphicsItemBorder( parent, scene, borderPosition, columns, rows, KBBScalableGraphicWidget::BORDER_SIZE/2)
{
	m_rayNumber = rayNumber;
	setAcceptsHoverEvents(true);
}


//
// Private
//

/*
// TODO: Nice effects to show the outgoing border position of the current position.

void KBBGraphicsItemRayResult::hoverEnterEvent (QGraphicsSceneHoverEvent* event)
{
	kDebug() << "Enter " << m_borderPosition << "!!\n";
}


void KBBGraphicsItemRayResult::hoverLeaveEvent (QGraphicsSceneHoverEvent* event)
{
	kDebug() << "Leave " << m_borderPosition << "!!\n";
}
*/

void KBBGraphicsItemRayResult::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget*)
{
	painter->setPen(QPen(Qt::green));
	painter->setBrush(Qt::Dense5Pattern);
	painter->setBrush(Qt::green);
	painter->drawEllipse(m_centerX - m_centerRadius, m_centerY - m_centerRadius, 2*m_centerRadius, 2*m_centerRadius);
	
	QString text("R"); // R for "Reflection". TODO: Draw a sign to symbolise it. Better for i18n and nicer anyway...
	if (m_rayNumber>0)
		text.setNum(m_rayNumber);
	painter->setPen(QPen(Qt::black));

	QFont font;
	font.setStyleHint(QFont::SansSerif);
	font.setWeight(QFont::DemiBold);
	font.setPixelSize(3*m_centerRadius/2);
	painter->setFont(font);
	painter->drawText(m_centerX - m_centerRadius, m_centerY - m_centerRadius, 2*m_centerRadius, 2*m_centerRadius, Qt::AlignCenter, text);
}
