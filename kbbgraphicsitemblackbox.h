//
// KBlackbox
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
class QGraphicsSvgItem;
#include <QList>
class QSvgRenderer;


#include "kbbscalablegraphicwidget.h"



/**
 * @brief Black box in the scalable graphic widget of KBlackBox
 *
 */
class KBBGraphicsItemBlackBox : public QGraphicsRectItem
{
	public:
		KBBGraphicsItemBlackBox(KBBScalableGraphicWidget* parent, QGraphicsScene* scene, QSvgRenderer* svgRenderer);
		~KBBGraphicsItemBlackBox();
		
		void setSize(const int columns, const int rows);


	private:
		void mousePressEvent (QGraphicsSceneMouseEvent* event);
		
		QGraphicsSvgItem* m_background;
		int m_columns;
		QList<QGraphicsLineItem*> m_lines;
		int m_rows;
		QGraphicsScene* m_scene;
		KBBScalableGraphicWidget* m_widget;
};

#endif // KBBGRAPHICSITEMBLACKBOX_H
