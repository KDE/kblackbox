/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "kbbgraphicsitem.h"



#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBGraphicsItem::KBBGraphicsItem(KBBScalableGraphicWidget::itemType itemType, QGraphicsScene* scene, KBBThemeManager* themeManager) : QGraphicsSvgItem()
{
	setSharedRenderer(themeManager->svgRenderer());
	setElementId(themeManager->elementId(itemType));
	setZValue(themeManager->zValue(itemType));
	
	scene->addItem(this);
}
