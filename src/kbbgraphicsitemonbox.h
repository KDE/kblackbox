/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBBGRAPHICSITEMONBOX_H
#define KBBGRAPHICSITEMONBOX_H



class QGraphicsScene;


#include "kbbgraphicsitem.h"
#include "kbbitemwithposition.h"
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief Item on the box on the scalable graphic widget
 *
 * The item is general and can be a ball or other markers.
 */
class KBBGraphicsItemOnBox : public KBBGraphicsItem, public KBBItemWithPosition
{
	public:
		/**
		 * @brief Invalid box position
		 */
		static const int NO_POSITION = -1;


		KBBGraphicsItemOnBox(KBBScalableGraphicWidget::itemType itemType, KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, const int boxPosition, const int columns, const int rows);
		
		int position() override;


	protected:
		virtual void removeInteractionInfos();

		KBBScalableGraphicWidget* m_widget;
		int m_columns;
		int m_rows;


	private:
		int boxPosition(qreal x, qreal y);
		bool isMovable();
		void mousePressEvent (QGraphicsSceneMouseEvent* event) override;
		void mouseReleaseEvent (QGraphicsSceneMouseEvent* event) override;
		void setBoxPosition(int boxPosition);
		
		int m_boxPosition;
		qreal m_dragXPos;
		qreal m_dragYPos;
		qreal m_dragX;
		qreal m_dragY;
		KBBScalableGraphicWidget::itemType m_itemType;
};

#endif // KBBGRAPHICSITEMONBOX_H
