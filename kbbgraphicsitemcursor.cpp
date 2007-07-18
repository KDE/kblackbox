//
// KBlackBox
//
// A simple game inspired by an emacs module
//
/***************************************************************************
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

#include "kbbgraphicsitemcursor.h"



//
// Constructor / Destructor
//

KBBGraphicsItemCursor::KBBGraphicsItemCursor(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager) : KBBGraphicsItem(KBBScalableGraphicWidget::cursor, parent->scene(), themeManager)
{
	setBoardSize(1,1);
}



//
// Public
//

int KBBGraphicsItemCursor::borderPosition()
{
	return m_borderPosition;
}


int KBBGraphicsItemCursor::boxPosition()
{
	return m_boxPosition;
}


void KBBGraphicsItemCursor::moveDown()
{
	if (m_borderPosition!=NO_POSITION) {
		if (m_borderPosition<m_columns) {
			m_boxPosition = m_borderPosition;
			m_borderPosition = NO_POSITION;
		} else if ((m_borderPosition>=m_columns) && (m_borderPosition<m_columns+m_rows-1))
			m_borderPosition++;
		else if ((m_borderPosition>=2*m_columns+m_rows+1) && (m_borderPosition<2*m_columns+2*m_rows))
			m_borderPosition--;
	} else if (m_boxPosition!=NO_POSITION) {
		if ((m_boxPosition+m_columns)<(m_columns*m_rows))
			m_boxPosition += m_columns;
		else {
			m_borderPosition = 2*m_columns+m_rows-(m_boxPosition+m_columns-m_columns*m_rows)-1;
			m_boxPosition = NO_POSITION;
		}
	}
	
	updatePositions();
}


void KBBGraphicsItemCursor::moveLeft()
{
	if (m_borderPosition!=NO_POSITION) {
		if ((m_borderPosition>0) && (m_borderPosition<m_columns))
			m_borderPosition--;
		else if ((m_borderPosition>=m_columns) && (m_borderPosition<m_columns+m_rows)) {
			m_boxPosition = m_columns*(m_borderPosition-m_columns+1)-1;
			m_borderPosition = NO_POSITION;
		} else if ((m_borderPosition>=m_columns+m_rows) && (m_borderPosition<2*m_columns+m_rows-1))
			m_borderPosition++;
	} else if (m_boxPosition!=NO_POSITION) {
		if ((m_boxPosition % m_columns)!=0)
			m_boxPosition--;
		else {
			m_borderPosition = 2*m_columns + 2*m_rows - m_boxPosition/m_columns -1;
			m_boxPosition = NO_POSITION;
		}
	}
	
	updatePositions();
}


void KBBGraphicsItemCursor::moveRight()
{
	if (m_borderPosition!=NO_POSITION) {
		if (m_borderPosition<m_columns-1)
			m_borderPosition++;
		else if ((m_borderPosition>=m_columns+m_rows+1) && (m_borderPosition<2*m_columns+m_rows))
			m_borderPosition--;
		else if (m_borderPosition>=2*m_columns+m_rows) {
			m_boxPosition = m_columns*(2*m_columns+2*m_rows-m_borderPosition-1);
			m_borderPosition = NO_POSITION;
		}
	} else if (m_boxPosition!=NO_POSITION) {
		if (((m_boxPosition+1) % m_columns)!=0)
			m_boxPosition++;
		else {
			m_borderPosition = m_columns + m_boxPosition/m_columns;
			m_boxPosition = NO_POSITION;
		}
	}
	
	updatePositions();
}


void KBBGraphicsItemCursor::moveUp()
{
	if (m_borderPosition!=NO_POSITION) {
		if ((m_borderPosition>=m_columns+1) && (m_borderPosition<m_columns+m_rows))
			m_borderPosition--;
		else if ((m_borderPosition>=m_columns+m_rows) && (m_borderPosition<2*m_columns+m_rows)) {
			m_boxPosition = m_columns*m_rows-(m_borderPosition-m_columns-m_rows)-1;
			m_borderPosition = NO_POSITION;
		} else if ((m_borderPosition>=2*m_columns+m_rows) && (m_borderPosition<2*m_columns+2*m_rows-1))
			m_borderPosition++;
	} else if (m_boxPosition!=NO_POSITION) {
		if ((m_boxPosition-m_columns)>=0)
			m_boxPosition -= m_columns;
		else {
			m_borderPosition = m_boxPosition;
			m_boxPosition = NO_POSITION;
		}
	}
	
	updatePositions();
}


void KBBGraphicsItemCursor::setBoardSize(const int columns, const int rows)
{
	m_columns = columns;
	m_rows = rows;
	
	m_boxPosition = 0;
	m_borderPosition = NO_POSITION;
	
	updatePositions();
	hide();
}


void KBBGraphicsItemCursor::setBorderPosition(const int borderPosition)
{
	if (borderPosition!=NO_POSITION) {
		m_borderPosition = borderPosition;
		m_boxPosition = NO_POSITION;
		
		const int b = KBBScalableGraphicWidget::BORDER_SIZE;
		const int r = KBBScalableGraphicWidget::RATIO;
		const int offset = 0;
		int x;
		int y;
		if (m_borderPosition<m_columns) {
			x = borderPosition*r + b;
			y = offset;
		} else if (borderPosition<m_columns + m_rows) {
			x = m_columns*r + b + b/2 - offset;
			y = (borderPosition - m_columns)*r + b;
		} else if (borderPosition<2*m_columns + m_rows) {
			x = (2*m_columns + m_rows - borderPosition)*r + b/2;
			y = m_rows*r + 3*b/2 - offset;
		} else {
			x = offset;
			y = (2*m_columns + 2*m_rows - borderPosition)*r + b/2;
		}
		setPos(x, y);
	}
}


void KBBGraphicsItemCursor::setBoxPosition(const int boxPosition)
{
	if (boxPosition!=NO_POSITION) {
		m_borderPosition = NO_POSITION;
		m_boxPosition = boxPosition;
		
		setPos(KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(m_boxPosition % m_columns), KBBScalableGraphicWidget::BORDER_SIZE + KBBScalableGraphicWidget::RATIO*(m_boxPosition / m_columns));
	}
}


//
// Private
//

void KBBGraphicsItemCursor::updatePositions()
{
	setBoxPosition(m_boxPosition);
	setBorderPosition(m_borderPosition);
	
	emit cursorAtNewPosition(m_borderPosition);
}

#include "kbbgraphicsitemcursor.moc"
