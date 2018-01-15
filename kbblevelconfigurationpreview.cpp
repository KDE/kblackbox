/*
Copyright (c) 2006, 2007, Nicolas Roffet, <nicolas-kde@roffet.com>

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU Library General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*/

#include "kbblevelconfigurationpreview.h"





#include <krandomsequence.h>


#include "kbbgraphicsitem.h"
#include "kbbgraphicsitemblackbox.h"
#include "kbbgraphicsitemset.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



KBBLevelConfigurationPreview::KBBLevelConfigurationPreview(QWidget *parent, KBBThemeManager* themeManager) : QGraphicsView(parent)
{
	setFrameStyle(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setScene(new QGraphicsScene(0, 0, 2*KBBScalableGraphicWidget::BORDER_SIZE, 2*KBBScalableGraphicWidget::BORDER_SIZE, this));
	m_blackbox = new KBBGraphicsItemBlackBox(this, scene(), themeManager, true);
	m_themeManager = themeManager;
}


void KBBLevelConfigurationPreview::preview(int balls, int columns, int rows)
{
	m_blackbox->setSize(columns, rows);
	scene()->setSceneRect(0, 0, columns*KBBScalableGraphicWidget::RATIO + 2*KBBScalableGraphicWidget::BORDER_SIZE, rows*KBBScalableGraphicWidget::RATIO + 2*KBBScalableGraphicWidget::BORDER_SIZE);

	// Show balls on the black box
	while (m_balls.count()>0) {
		delete m_balls.last();
		m_balls.removeLast();
	}
	QList<int> ballPos;
	KRandomSequence random;
	random.setSeed(0);
	int boxPos;
	KBBGraphicsItem* item;
	for (int i=0;i<balls;i++) {
		do {
			boxPos = random.getLong(columns*rows);
		} while (ballPos.contains(boxPos));
		item = new KBBGraphicsItem(KBBScalableGraphicWidget::playerBall, scene(), m_themeManager);
		item->setPos(KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPos % columns), KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPos / columns));
		ballPos.append(boxPos);
		m_balls.append(item);
	}

	resizeEvent(0);
}


void KBBLevelConfigurationPreview::drawBackground(QPainter* painter, const QRectF&)
{
	QRectF rectBackground;

	// TODO: This is duplication of code from the class KBBScalableGraphicWidget. Try to fix this.
	const qreal sW = scene()->width();
	const qreal sH = scene()->height();
	const qreal wW = width();
	const qreal wH = height();
	const qreal offset = (sH+sW)/100 ;
	if (sH*wW > sW*wH) {
		// The widget is larger than the scene
		qreal w =  wW*sH/wH;
		rectBackground.setRect((sW-w)/2-offset, -offset, w + 2*offset, sH + 2*offset);
	} else {
		// The scene is larger than the widget (or as large)
		qreal h =  wH*sW/wW;
		rectBackground.setRect(-offset, (sH-h)/2-offset, sW + 2*offset, h + 2*offset);
	}

	m_themeManager->svgRenderer()->render(painter, m_themeManager->elementId(KBBScalableGraphicWidget::background), rectBackground);
}


void KBBLevelConfigurationPreview::resizeEvent( QResizeEvent* )
{
	fitInView(0.5*KBBScalableGraphicWidget::BORDER_SIZE, 0.5*KBBScalableGraphicWidget::BORDER_SIZE, scene()->width() - 1.5*KBBScalableGraphicWidget::BORDER_SIZE, scene()->height() - 1.5*KBBScalableGraphicWidget::BORDER_SIZE, Qt::KeepAspectRatio);
}



