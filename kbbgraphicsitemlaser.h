/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/



#ifndef KBBGRAPHICSITEMLASER_H
#define KBBGRAPHICSITEMLASER_H


#include "kbbgraphicsitem.h"
#include "kbbgraphicsitemborder.h"
#include "kbbitemwithposition.h"
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief Laser element of the scalable graphic widget
 *
 * The laser is used to shoot a laser ray into the black box.
 *
 * Note: The laser size should be so defined: width = width of a single box of the black box (RATIO), height = twice the size of single box of the black box (2*RATIO). The laser size is independent of the board border size.
 */
class KBBGraphicsItemLaser : public KBBGraphicsItemBorder, public KBBGraphicsItem, public KBBItemWithPosition
{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param parent Scalable graphic widget
		 * @param themeManager Theme manager
		 * @param borderPosition Position of the laser around the border. Each laser has a different border position.
		 * @param columns Width of the Black Box
		 * @param rows Height of the Black Box
		 */
		KBBGraphicsItemLaser(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, const int borderPosition, const int columns, const int rows);

		/**
		 * @brief Get the border position
		 */
		int position() override;


	private:
		void hoverEnterEvent (QGraphicsSceneHoverEvent*) override;
		void hoverLeaveEvent (QGraphicsSceneHoverEvent*) override;
		void mousePressEvent (QGraphicsSceneMouseEvent* event) override;
		
		KBBScalableGraphicWidget* m_widget;
};

#endif // KBBGRAPHICSITEMLASER_H
