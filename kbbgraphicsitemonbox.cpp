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

#include "kbbgraphicsitemonbox.h"



#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


#include "kbbgraphicsitem.h"
#include "kbbgraphicsiteminteractioninfo.h"
#include "kbbitemwithposition.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBGraphicsItemOnBox::KBBGraphicsItemOnBox(KBBScalableGraphicWidget::itemType itemType, KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, const int boxPosition, const int columns, const int rows) : KBBGraphicsItem(itemType, parent->scene(), themeManager), KBBItemWithPosition()
{
	m_widget = parent;
	m_columns = columns;
	m_rows = rows;
	m_itemType = itemType;
	
	setBoxPosition(boxPosition);

	if (isMovable()) {
		setAcceptDrops(true);
		setFlag(QGraphicsItem::ItemIsMovable);
	}
}



//
// Public
//

const int KBBGraphicsItemOnBox::position ()
{
	return m_boxPosition;
}



//
// Protected
//

void KBBGraphicsItemOnBox::removeInteractionInfos()
{
}



//
// Private
//

int KBBGraphicsItemOnBox::boxPosition(qreal x, qreal y)
{
	int r = (int)((y - KBBScalableGraphicWidget::BORDER_SIZE)/KBBScalableGraphicWidget::RATIO);
	int c = (int)((x - KBBScalableGraphicWidget::BORDER_SIZE)/KBBScalableGraphicWidget::RATIO);

	if ((r<0) || (r>=m_rows) || (c<0) || (c>=m_columns)) {
		if (m_boxPosition>=m_columns*m_rows)
			// Item is outside of the board
			return m_boxPosition;
		else
			return NO_POSITION;
	} else
		return c+r*m_columns;
}


bool KBBGraphicsItemOnBox::isMovable()
{
	return ((m_itemType==KBBScalableGraphicWidget::playerBall) || (m_itemType==KBBScalableGraphicWidget::unsureBall) || (m_itemType==KBBScalableGraphicWidget::markerNothing));
}


void KBBGraphicsItemOnBox::mousePressEvent (QGraphicsSceneMouseEvent* event)
{
	m_dragXPos = x();
	m_dragYPos = y();
	m_dragX = event->scenePos().x();
	m_dragY = event->scenePos().y();

	if (isMovable()) {
		setCursor(Qt::ClosedHandCursor);
		removeInteractionInfos();
	}
}


void KBBGraphicsItemOnBox::mouseReleaseEvent (QGraphicsSceneMouseEvent* event)
{
	qreal dropX = event->scenePos().x();
	qreal dropY = event->scenePos().y();

	if ((dropX==m_dragX) && (dropY==m_dragY)) {
		setCursor(Qt::ArrowCursor);
		if ((position()!=NO_POSITION) && (position()<(m_columns*m_rows)))
			m_widget->mouseBoxClick(event->button(), position());
	} else if (isMovable()) {
		setCursor(Qt::ArrowCursor);

		if ((boxPosition(dropX, dropY)==NO_POSITION) || (boxPosition(dropX, dropY)==boxPosition(m_dragX, m_dragY)) || (boxPosition(dropX, dropY)>=m_columns*m_rows))
			// Cancel move
			setPos(m_dragXPos, m_dragYPos);
		else {
			if (m_itemType==KBBScalableGraphicWidget::markerNothing)
				setBoxPosition(m_widget->moveMarkerNothing(boxPosition(m_dragX, m_dragY), boxPosition(dropX, dropY)));
			else {
				int newPos = m_widget->positionAfterMovingBall(boxPosition(m_dragX, m_dragY), boxPosition(dropX, dropY));

				// if we do move from outside the board. Because in this case and if the move is OK, we will destroy ourself to put a "real" new ball over the board... So we cannot do anything else more after calling m_widget->moveBall()...
				if ((m_boxPosition==NO_POSITION) || (m_boxPosition>=(m_columns*m_rows))) {
					if (newPos==m_boxPosition)
						setPos(m_dragXPos, m_dragYPos);
					else
						m_widget->moveBall(boxPosition(m_dragX, m_dragY), boxPosition(dropX, dropY));
				} else
					setBoxPosition(m_widget->moveBall(boxPosition(m_dragX, m_dragY), boxPosition(dropX, dropY)));
			}
		}
	}
}


void KBBGraphicsItemOnBox::setBoxPosition(int boxPosition)
{
	m_boxPosition = boxPosition;

	if ((boxPosition!=NO_POSITION) && (boxPosition<m_columns*m_rows)) {
		QPointF p((qreal) (KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition % m_columns)), (qreal) (KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(boxPosition / m_columns)));
		setPos(p);
	}
}
