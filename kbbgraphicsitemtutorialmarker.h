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

#ifndef KBBGRAPHICSITEMTUTORIALMARKER_H
#define KBBGRAPHICSITEMTUTORIALMARKER_H



#include "kbbgraphicsitem.h"
#include "kbbgraphicsitemborder.h"
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief Marker used by the tutorial
 */
class KBBGraphicsItemTutorialMarker : public KBBGraphicsItemBorder, public KBBGraphicsItem
{
	public:
		static const int NO_POSITION = -1;


		KBBGraphicsItemTutorialMarker(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, const int columns, const int rows);

		void setBorderPosition(const int borderPosition);
};

#endif // KBBGRAPHICSITEMTUTORIALMARKER_H
