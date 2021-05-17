/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/



#ifndef KBBGRAPHICSITEMINTERACTIONINFO_H
#define KBBGRAPHICSITEMINTERACTIONINFO_H





#include "kbbgraphicsitem.h"
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief Interaction information around a ball on the scalable graphic widget
 *
 * Some signs around the ball the player is pointing on are been displayed to show how a laser ray interacts with the ball.
 */
class KBBGraphicsItemInteractionInfo : public KBBGraphicsItem
{
	public:
		/**
		 * @brief Constructor
		 */
		KBBGraphicsItemInteractionInfo(KBBScalableGraphicWidget* widget, KBBThemeManager* themeManager, KBBScalableGraphicWidget::itemType type, const qreal x, const qreal y, const int rotation);
		
		void setType(KBBScalableGraphicWidget::itemType type);


	private:
		QString m_elementIdBase;
		KBBThemeManager* m_themeManager;
};

#endif // KBBGRAPHICSITEMINTERACTIONINFO_H
