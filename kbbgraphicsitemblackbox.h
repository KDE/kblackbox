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



#ifndef KBBGRAPHICSITEMBLACKBOX_H
#define KBBGRAPHICSITEMBLACKBOX_H


class QGraphicsLineItem;
#include <QGraphicsRectItem>
class QGraphicsScene;
class QGraphicsView;
#include <QList>
#include <QPen>


class KBBGraphicsItem;
#include "kbbscalablegraphicwidget.h"
#include <QObject>


/**
 * @brief The black box in the scalable graphic widget
 */
class KBBGraphicsItemBlackBox : public QObject, public QGraphicsRectItem
{
Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 */
		KBBGraphicsItemBlackBox(QGraphicsView* parent, QGraphicsScene* scene, KBBThemeManager* themeManager, bool isPreview);


		/**
		 * @brief Set the KBBScalableGraphicWidget
		 */
		void setKBBScalableGraphicWidget(KBBScalableGraphicWidget* w);

		/**
		 * @brief Define the (new) size of the black box
		 *
		 * Used to "recycle" the black box for a new game.
		 */
		void setSize(const int columns, const int rows);


	private:
		void mousePressEvent (QGraphicsSceneMouseEvent* event) override;
		void hoverMoveEvent (QGraphicsSceneHoverEvent* event) override;
		void hoverLeaveEvent (QGraphicsSceneHoverEvent* event) override;

		KBBGraphicsItem* m_background;
		int m_columns;
		QList<QGraphicsLineItem*> m_lines;
		QPen m_penLines;
		int m_rows;
		QGraphicsScene* m_scene;
		KBBScalableGraphicWidget* m_widget;
		int m_zValueLines;
 public:
	  Q_SIGNALS:
		void hoverMoved(int);
		void hoverExit();
};

#endif // KBBGRAPHICSITEMBLACKBOX_H
