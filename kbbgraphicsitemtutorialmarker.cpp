/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
