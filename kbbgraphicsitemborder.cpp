/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kbbgraphicsitemborder.h"



#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBorder::KBBGraphicsItemBorder(int borderPosition, int columns, int rows, float offset)
{
	m_offset = offset;
	
	setSize(borderPosition, columns, rows);
}



//
// Public
//

void KBBGraphicsItemBorder::setSize(int borderPosition, int columns, int rows)
{
	m_borderPosition = borderPosition;
	m_columns = columns;
	m_rows = rows;
	
	centerCoordinate(m_borderPosition, m_centerX, m_centerY, m_offset);
}



//
// Protected
//

void KBBGraphicsItemBorder::setBorderPosition(int borderPosition)
{
	setSize(borderPosition, m_columns, m_rows);
}


void KBBGraphicsItemBorder::centerCoordinate(int borderPosition, float &centerX, float &centerY, float offset)
{
	const float b = (float) KBBScalableGraphicWidget::BORDER_SIZE;
	const float r = (float) KBBScalableGraphicWidget::RATIO;
	float x;
	float y;
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


float KBBGraphicsItemBorder::centerX() const
{
	return m_centerX;
}


float KBBGraphicsItemBorder::centerY() const
{
	return m_centerY;
}


int KBBGraphicsItemBorder::rotationAngle()
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
