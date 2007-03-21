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
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA               *
 ***************************************************************************/



#include "kbbgraphicsitemborder.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBorder::KBBGraphicsItemBorder(const int borderPosition, const int columns, const int rows, const int offset)
{
	m_offset = offset;
	
	setSize(borderPosition, columns, rows);
}



//
// Public
//

void KBBGraphicsItemBorder::setSize(const int borderPosition, const int columns, const int rows)
{
	m_borderPosition = borderPosition;
	m_columns = columns;
	m_rows = rows;
	
	centerCoordinate(m_borderPosition, m_centerX, m_centerY, m_offset);
}



//
// Protected
//

void KBBGraphicsItemBorder::centerCoordinate(const int borderPosition, int &centerX, int &centerY, const int offset)
{
	const int b = KBBScalableGraphicWidget::BORDER_SIZE;
	const int r = KBBScalableGraphicWidget::RATIO;
	int x;
	int y;
	if (borderPosition<m_columns) {
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
	
	centerX = x + r/2;
	centerY = y + r/2;
}


const int KBBGraphicsItemBorder::rotation()
{
	if (m_borderPosition<m_columns) {
		return 0;
	} else if (m_borderPosition<m_columns + m_rows) {
		return 90;
	} else if (m_borderPosition<2*m_columns + m_rows) {
		return 180;
	} else {
		return 270;
	}
}
