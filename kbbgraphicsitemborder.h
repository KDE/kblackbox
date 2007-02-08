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



#ifndef KBBGRAPHICSITEMBORDER_H
#define KBBGRAPHICSITEMBORDER_H


#include <QGraphicsSvgItem>
class QGraphicsScene;
class QRectF;
class QSvgRenderer;


#include "kbbscalablegraphicwidget.h"



/**
 * @brief Border element of the scalable graphic widget of KBlackBox
 *
 */
class KBBGraphicsItemBorder : public QGraphicsSvgItem
{
	public:
		KBBGraphicsItemBorder(KBBScalableGraphicWidget* parent, QGraphicsScene* scene, QSvgRenderer* svgRenderer, const QString svgId, const int borderPosition, const int columns, const int rows, const int offset);
	
		int borderPosition() const;


	protected:
		KBBScalableGraphicWidget* m_widget;
		int m_borderPosition;
		int m_centerX;
		int m_centerY;
		int m_rotation;
};

#endif // KBBGRAPHICSITEMBORDER_H
