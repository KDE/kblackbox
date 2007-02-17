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



#include <QColor>
#include <QGraphicsScene>
#include <QList>
#include <QPainterPath>
#include <QPen>


#include "kbbboard.h"
#include "kbbgraphicsitemborder.h"
#include "kbbgraphicsitemray.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemRay::KBBGraphicsItemRay( QGraphicsScene* scene, const int borderPosition, KBBBallsOnBoard* ballsOnBoard, rayType type ) : KBBGraphicsItemBorder( borderPosition, ballsOnBoard->columns(), ballsOnBoard->rows(), KBBScalableGraphicWidget::BORDER_SIZE/2), QGraphicsPathItem ( 0, scene )
{
	QList<int> points;
	const int oppositeBorderPosition = ballsOnBoard->oppositeBorderPositionWithPoints(borderPosition, points);
	
	QPainterPath path;
	path.moveTo(m_centerX, m_centerY);

	const int columns = ballsOnBoard->columns();
	const int rows = ballsOnBoard->rows();
	const int b = KBBScalableGraphicWidget::BORDER_SIZE;
	const int r = KBBScalableGraphicWidget::RATIO;
	int x;
	int y;
	for (int i=0; i<points.count(); i++) {
		x = b - r/2 + r*((points[i] % columns) + 1);
		y = b - r/2 + r*((points[i] / columns) + 1);
		path.lineTo(x,y);
	}
	
	if (oppositeBorderPosition!=KBBBoard::HIT_POSITION) {
		int x1;
		int y1;
		centerCoordinate(oppositeBorderPosition, x1, y1, b/2);
		path.lineTo(x1,y1);
	}
	
	QPen pen;
	switch(type) {
		case playerRay:
			pen.setColor(QColor(0,237,255));
			pen.setStyle(Qt::DotLine);
			pen.setWidth(r/10);
			setZValue(KBBScalableGraphicWidget::ZVALUE_PLAYER_RAY);
			break;
		case playerSolutionRay:
			pen.setColor(QColor(0,237,255));
			pen.setStyle(Qt::DotLine);
			pen.setWidth(r/15);
			setZValue(KBBScalableGraphicWidget::ZVALUE_PLAYER_RAY);
			break;
		case solutionRay:
			pen.setColor(Qt::red);
			pen.setStyle(Qt::SolidLine);
			pen.setWidth(r/7);
			setZValue(KBBScalableGraphicWidget::ZVALUE_SOLUTION_RAY);
			break;
	}
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setCapStyle(Qt::RoundCap);
	setPen(pen);
	
	setPath(path);
}
