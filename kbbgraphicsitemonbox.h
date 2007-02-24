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



#ifndef KBBGRAPHICSITEMONBOX_H
#define KBBGRAPHICSITEMONBOX_H


class QGraphicsScene;


#include "kbbgraphicsitem.h"
class KBBScalableGraphicWidget;



/**
 * @brief Item on the box on the scalable graphic widget
 *
 * The item is general and can be a ball or other markers.
 */
class KBBGraphicsItemOnBox : public KBBGraphicsItem
{
	public:
		enum itemType { blue, blueUnsure, red, cross, nothing };
		
		KBBGraphicsItemOnBox(KBBScalableGraphicWidget* parent, const int boxPosition, const int columns, const int rows, itemType type);
		
		const int position();


	protected:
		KBBScalableGraphicWidget* m_widget;
		int m_boxPosition;
		int m_columns;
		int m_rows;


	private:
		void mousePressEvent (QGraphicsSceneMouseEvent*);
		
		itemType m_itemType;
};

#endif // KBBGRAPHICSITEMONBOX_H
