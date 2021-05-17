/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/



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
		/**
		 * @brief Invalid border and box position
		 */
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
		void updatePositions();

	Q_SIGNALS:
		void cursorAtNewPosition(const int borderPosition);


	private:
		
		int m_boxPosition;
		int m_borderPosition;
		int m_columns;
		int m_rows;
};

#endif // KBBGRAPHICSITEMCURSOR_H
