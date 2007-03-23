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



#ifndef KBBGRAPHICSITEMINTERACTIONINFO_H
#define KBBGRAPHICSITEMINTERACTIONINFO_H


#include <QString>


#include "kbbgraphicsitem.h"
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief Interaction information around a ball on the scalable graphic widget
 *
 * Some signs around the ball the player is pointing on are been displayed to show how a laser ray interacts with the ball.
 */
class KBBGraphicsItemInteractionInfo : public KBBGraphicsItem
{
	public:
		/**
		 * @brief Constructor
		 */
		KBBGraphicsItemInteractionInfo(KBBScalableGraphicWidget* widget, KBBThemeManager* themeManager, KBBScalableGraphicWidget::itemType type, const qreal x, const qreal y, const int rotation);
		
		void setType(KBBScalableGraphicWidget::itemType type);


	private:
		QString m_elementIdBase;
		KBBThemeManager* m_themeManager;
};

#endif // KBBGRAPHICSITEMINTERACTIONINFO_H
