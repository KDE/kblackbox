/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBBGRAPHICSITEMBALL_H
#define KBBGRAPHICSITEMBALL_H



class QTimer;


#include "kbbgraphicsitemonbox.h"
class KBBGraphicsItemInteractionInfo;
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief Ball on the scalable graphic widget
 *
 * A ball can be gray, red or gray with a question mark.
 */
class KBBGraphicsItemBall : public KBBGraphicsItemOnBox
{
	Q_OBJECT
	
	public:
		static const int TIME_TO_WAIT_BEFORE_SHOWING_INTERACTIONS = 1500;
		
		/**
		 * @brief Constructor
		 */
		KBBGraphicsItemBall(KBBScalableGraphicWidget::itemType itemType, KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, int boxPosition, int columns, int rows);


		~KBBGraphicsItemBall() override;


	private Q_SLOTS:
		void showInteractions();


	private:
		void hoverEnterEvent (QGraphicsSceneHoverEvent*) override;
		void hoverLeaveEvent (QGraphicsSceneHoverEvent*) override;
		void removeInteractionInfos() override;
		
		KBBGraphicsItemInteractionInfo* m_interactionInfos[8];
		KBBThemeManager* m_themeManager;
		QTimer* m_timer;
		
		KBBScalableGraphicWidget::itemType m_ballType;
};

#endif // KBBGRAPHICSITEMBALL_H
