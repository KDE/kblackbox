/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "kbbgraphicsitemray.h"

#include <QGraphicsScene>
#include <QList>
#include <QPainterPath>
#include <QPen>


#include "kbbballsonboard.h"
#include "kbbgamedoc.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBGraphicsItemRay::KBBGraphicsItemRay(KBBScalableGraphicWidget::itemType itemType, QGraphicsScene* scene, KBBThemeManager* themeManager) : KBBGraphicsItemBorder(0, 1, 1, KBBScalableGraphicWidget::BORDER_SIZE/2), QGraphicsPathItem (nullptr)
{
    scene->addItem(this);
    QPen pen;

    pen.setColor(themeManager->color(itemType));
    pen.setStyle(themeManager->style(itemType));
    pen.setWidthF(themeManager->width(itemType));
    setZValue(themeManager->zValue(itemType));

    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);

    setPen(pen);
}



//
// Public
//

void KBBGraphicsItemRay::draw(KBBBallsOnBoard* ballsOnBoard, const int borderPosition)
{
	const int columns = ballsOnBoard->columns();
	const int rows = ballsOnBoard->rows();
	
	QList<int> points;
	const int oppositeBorderPosition = ballsOnBoard->oppositeBorderPositionWithPoints(borderPosition, points);
	
	QPainterPath path;
	setSize(borderPosition, columns, rows);
	path.moveTo(m_centerX, m_centerY);

	const float b = (float) KBBScalableGraphicWidget::BORDER_SIZE;
	const float r = (float) KBBScalableGraphicWidget::RATIO;
	float x;
	float y;
	for (int i=0; i<points.count(); i++) {
		x = b - r/2 + r*((points[i] % columns) + 1);
		y = b - r/2 + r*((points[i] / columns) + 1);
		path.lineTo(x,y);
	}
	
	if (oppositeBorderPosition!=KBBGameDoc::HIT_POSITION) {
		float x1;
		float y1;
		centerCoordinate(oppositeBorderPosition, x1, y1, b/2.);
		path.lineTo(x1,y1);
	}
	
	setPath(path);
}


void KBBGraphicsItemRay::hide()
{
	setPath(QPainterPath());
}
