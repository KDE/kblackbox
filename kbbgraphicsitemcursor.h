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



#ifndef KBBGRAPHICSITEMCURSOR_H
#define KBBGRAPHICSITEMCURSOR_H


#include "kbbgraphicsitem.h"
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief "Cursor" on the scalable graphic widget
 *
 * The "cursor" is used to play with the keyboard.
 */
class KBBGraphicsItemCursor : public KBBGraphicsItem
{
	Q_OBJECT


	public:
		static const int NO_POSITION = -1;
		
		
		KBBGraphicsItemCursor(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager);
		
		
		int borderPosition();
		int boxPosition();
		
		void moveDown();
		void moveLeft();
		void moveRight();
		void moveUp();
		
		void setBoardSize(const int columns, const int rows);
		void setBorderPosition(const int borderPosition);
		void setBoxPosition(const int boxPosition);


	signals:
		void cursorAtNewPosition(const int borderPosition);


	private:
		void updatePositions();
		
		int m_boxPosition;
		int m_borderPosition;
		int m_columns;
		int m_rows;
};

#endif // KBBGRAPHICSITEMCURSOR_H
