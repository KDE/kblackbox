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

#include "kbbgraphicsitemblackbox.h"



#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>


#include "kbbgraphicsitem.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBlackBox::KBBGraphicsItemBlackBox(QGraphicsView* parent, QGraphicsScene* scene, KBBThemeManager* themeManager) : QGraphicsRectItem (0)
{
    scene->addItem(this);
	m_columns = 1;
	m_rows = 1;
	m_widget = 0;
	m_scene = scene;

	m_background = new KBBGraphicsItem(KBBScalableGraphicWidget::blackbox, m_scene, themeManager);
	
	//Grid
	const KBBScalableGraphicWidget::itemType g = KBBScalableGraphicWidget::blackboxGrid;
	m_zValueLines = themeManager->zValue(g);
	m_penLines.setColor(themeManager->color(g));
	m_penLines.setStyle(themeManager->style(g));
	m_penLines.setWidthF(themeManager->width(g));
}



//
// Public
//

void KBBGraphicsItemBlackBox::setSize(const int columns, const int rows)
{
	m_background->setTransform(QTransform::fromScale(1./m_columns, 1./m_rows), true);

	if ((m_columns!=columns) || (m_rows!=rows)) {
		m_columns = columns;
		m_rows = rows;
		
		const int b = KBBScalableGraphicWidget::BORDER_SIZE;
		const int r = KBBScalableGraphicWidget::RATIO;
		
		setRect(b, b, m_columns*r, m_rows*r);
	
		//remove old lines
		for (int i=0; i<m_lines.count(); i++)
			delete m_lines[i];
		m_lines.clear();
	
		// add new lines
        for (int i=0; i<m_columns+1; i++) {
            QGraphicsLineItem *item = new QGraphicsLineItem( b + i*r, b, b + i*r, b + m_rows*r, this);
            m_lines.append(item);
        }
        for (int i=0; i<m_rows+1; i++) {
            QGraphicsLineItem *item = new QGraphicsLineItem(  b, b + i*r, b + m_columns*r,  b + i*r, this);
            m_lines.append(item);
        }
		
		// set line style
		for (int i=0; i<m_lines.count(); i++) {
			m_lines[i]->setPen(m_penLines);
			m_lines[i]->setZValue(m_zValueLines);
		}
		m_background->setPos(b, b);
	}
	
	m_background->setTransform(QTransform::fromScale(m_columns/1., m_rows/1.), true);
}


void KBBGraphicsItemBlackBox::setKBBScalableGraphicWidget(KBBScalableGraphicWidget* w)
{
	m_widget = w;
}



//
// Private
//

void KBBGraphicsItemBlackBox::mousePressEvent (QGraphicsSceneMouseEvent* event)
{
	int x = (int)(event->pos().x() - KBBScalableGraphicWidget::BORDER_SIZE)/KBBScalableGraphicWidget::RATIO;
	int y = (int)(event->pos().y() - KBBScalableGraphicWidget::BORDER_SIZE)/KBBScalableGraphicWidget::RATIO;
	
	if (m_widget!=0)
		m_widget->mouseBoxClick(event->button(), x + y*m_columns);
}
