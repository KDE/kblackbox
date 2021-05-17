/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kbbgraphicsitemblackbox.h"



#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>


#include "kbbgraphicsitem.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBlackBox::KBBGraphicsItemBlackBox(QGraphicsView* parent, QGraphicsScene* scene, KBBThemeManager* themeManager, bool isPreview) : QGraphicsRectItem (nullptr)
{
    Q_UNUSED(parent); // TODO: check why not passing to QGraphicsRectItem?
    scene->addItem(this);
	m_columns = 1;
	m_rows = 1;
	m_widget = nullptr;
	m_scene = scene;

	m_background = new KBBGraphicsItem(KBBScalableGraphicWidget::blackbox, m_scene, themeManager);
	
	//Grid
	const KBBScalableGraphicWidget::itemType g = KBBScalableGraphicWidget::blackboxGrid;
	m_zValueLines = themeManager->zValue(g);
	m_penLines.setColor(themeManager->color(g));
	m_penLines.setStyle(themeManager->style(g));
	m_penLines.setWidthF(themeManager->width(g));
	//accept hover events unless the central widget is a preview (crashes the program)
	if (!isPreview)
		setAcceptHoverEvents(true);
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
	
	if (m_widget!=nullptr)
		m_widget->mouseBoxClick(event->button(), x + y*m_columns);
}

void KBBGraphicsItemBlackBox::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
	m_widget->cursorOff();
}

void KBBGraphicsItemBlackBox::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	int x = (int)(event->pos().x() - KBBScalableGraphicWidget::BORDER_SIZE)/KBBScalableGraphicWidget::RATIO;
	int y = (int)(event->pos().y() - KBBScalableGraphicWidget::BORDER_SIZE)/KBBScalableGraphicWidget::RATIO;
	Q_EMIT hoverMoved(x + y*m_columns);
}
