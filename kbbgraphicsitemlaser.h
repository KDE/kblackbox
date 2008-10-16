//
// KBlackBox
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
		int position();


	private:
		void hoverEnterEvent (QGraphicsSceneHoverEvent*);
		void hoverLeaveEvent (QGraphicsSceneHoverEvent*);
		void mousePressEvent (QGraphicsSceneMouseEvent* event);
		
		KBBScalableGraphicWidget* m_widget;
};

#endif // KBBGRAPHICSITEMLASER_H
