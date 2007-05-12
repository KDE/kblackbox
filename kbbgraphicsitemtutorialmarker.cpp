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

#include "kbbgraphicsitemtutorialmarker.h"



//
// Constructor / Destructor
//

KBBGraphicsItemTutorialMarker::KBBGraphicsItemTutorialMarker(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, const int columns, const int rows) : KBBGraphicsItemBorder(NO_POSITION, columns, rows, 0), KBBGraphicsItem(KBBScalableGraphicWidget::tutorialMarker, parent->scene(), themeManager)
{
	m_borderPosition = NO_POSITION;
	setBorderPosition(m_borderPosition);
}



//
// Public
//

void KBBGraphicsItemTutorialMarker::setBorderPosition(const int borderPosition)
{
	KBBGraphicsItemBorder::setBorderPosition(borderPosition);
	setPos(centerX() - KBBScalableGraphicWidget::RATIO/2, centerY() - KBBScalableGraphicWidget::RATIO/2);
}
