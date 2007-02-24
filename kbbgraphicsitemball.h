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



#ifndef KBBGRAPHICSITEMBALL_H
#define KBBGRAPHICSITEMBALL_H


class QGraphicsScene;
class QTimer;


#include "kbbgraphicsitem.h"
class KBBGraphicsItemInteractionInfo;
#include "kbbscalablegraphicwidget.h"



/**
 * @brief Ball (or similar) on the scalable graphic widget
 *
 */
class KBBGraphicsItemBall : public KBBGraphicsItem
{
	Q_OBJECT
	
	public:
		static const int TIME_TO_WAIT_BEFORE_SHOWING_INTERACTIONS = 1500;
		
		enum ballType { blue, blueUnsure, red, cross, nothing };
		
		KBBGraphicsItemBall(KBBScalableGraphicWidget* parent, const int boxPosition, const int columns, const int rows, ballType type);
		
		const int position();


	private slots:
		void showInteractions();


	private:
		void hoverEnterEvent (QGraphicsSceneHoverEvent*);
		void hoverLeaveEvent (QGraphicsSceneHoverEvent*);
		void mousePressEvent (QGraphicsSceneMouseEvent*);
		void removeInteractionInfos();
		
		ballType m_ballType;
		int m_boxPosition;
		int m_columns;
		KBBGraphicsItemInteractionInfo* m_interactionInfos[8];
		int m_rows;
		QTimer* m_timer;
		KBBScalableGraphicWidget* m_widget;
};

#endif // KBBGRAPHICSITEMBALL_H
