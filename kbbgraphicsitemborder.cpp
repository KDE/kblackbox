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

KBBGraphicsItemBorder::KBBGraphicsItemBorder( const int borderPosition, const int columns, const int rows, const int offset)
{
	m_borderPosition = borderPosition;
	centerCoordinate(m_borderPosition, m_centerX, m_centerY, offset, columns, rows);
}



//
// Private
//

void KBBGraphicsItemBorder::centerCoordinate(const int borderPosition, int &centerX, int &centerY, const int offset, const int columns, const int rows)
{
	const int b = KBBScalableGraphicWidget::BORDER_SIZE;
	const int r = KBBScalableGraphicWidget::RATIO;
	int x;
	int y;
	if (borderPosition<columns) {
		x = borderPosition*r + b;
		y = offset;
	} else if (borderPosition<columns + rows) {
		x = (columns)*r + b + b/2 - offset;
		y = (borderPosition - columns)*r + b;
	} else if (borderPosition<2*columns + rows) {
		x = (2*columns + rows - borderPosition)*r + b/2;
		y = (rows)*r + 3*b/2 - offset;
	} else {
		x = offset;
		y = (2*columns + 2*rows - borderPosition)*r + b/2;
	}
	
	centerX = x + r/2;
	centerY = y + r/2;
}
