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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/



#ifndef KBBGRAPHICSITEMRAYRESULT_H
#define KBBGRAPHICSITEMRAYRESULT_H


class QGraphicsScene;


#include "kbbgraphicsitemborder.h"



/**
 * @brief RayResult als element of the scalable graphic widget of KBlackBox
 *
 */
class KBBGraphicsItemRayResult : public KBBGraphicsItemBorder
{
	public:
		KBBGraphicsItemRayResult(KBBScalableGraphicWidget* parent, QGraphicsScene* scene, const int borderPosition, const int columns, const int rows, const int rayNumber);


	private:
		//void hoverEnterEvent (QGraphicsSceneHoverEvent* event);
		//void hoverLeaveEvent (QGraphicsSceneHoverEvent* event);
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*);
		
		int m_rayNumber;
};

#endif // KBBGRAPHICSITEMRAYRESULT_H
