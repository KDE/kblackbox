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



#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>


#include "kbbgraphicsitemblackbox.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBlackBox::KBBGraphicsItemBlackBox( KBBScalableGraphicWidget* parent, QGraphicsScene* scene) : QGraphicsRectItem (0, scene )
{
	m_widget = parent;

	setPen(QPen(Qt::black));
	setBrush(QColor(35, 35, 35));
}



//
// Public
//

void KBBGraphicsItemBlackBox::setSize(const int columns, const int rows)
{
	m_columns = columns;
	m_rows = rows;
	
	setRect(KBBScalableGraphicWidget::BORDER_SIZE, KBBScalableGraphicWidget::BORDER_SIZE, m_columns*KBBScalableGraphicWidget::RATIO, m_rows*KBBScalableGraphicWidget::RATIO);

	//TODO: Improve the black box with a grid a nicer look.
}



//
// Private
//

void KBBGraphicsItemBlackBox::mousePressEvent (QGraphicsSceneMouseEvent* event)
{
	int x = (static_cast<int>(event->pos().x()) - KBBScalableGraphicWidget::BORDER_SIZE);
	int y = (static_cast<int>(event->pos().y()) - KBBScalableGraphicWidget::BORDER_SIZE);
	x /= KBBScalableGraphicWidget::RATIO;
	y /= KBBScalableGraphicWidget::RATIO;
	m_widget->clickBlackBox(x + y*m_columns);
}
