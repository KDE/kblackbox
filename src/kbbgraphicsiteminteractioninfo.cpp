/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/



#include "kbbgraphicsiteminteractioninfo.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBGraphicsItemInteractionInfo::KBBGraphicsItemInteractionInfo( KBBScalableGraphicWidget* widget, KBBThemeManager* themeManager, KBBScalableGraphicWidget::itemType type, const qreal x, const qreal y, const int rotation) : KBBGraphicsItem(KBBScalableGraphicWidget::interactionInfoNothing, widget->scene(), themeManager)
{
	m_themeManager = themeManager;
	setPos(x, y);
	setTransform(QTransform().rotate(rotation), true);
	setType(type);
}



//
// Public
//

void KBBGraphicsItemInteractionInfo::setType(KBBScalableGraphicWidget::itemType type)
{
	setElementId(m_themeManager->elementId(type));
}
