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

#ifndef KBBGRAPHICSITEMBALLREPOSITORY_H
#define KBBGRAPHICSITEMBALLREPOSITORY_H



#include <krandomsequence.h>


#include "kbbgraphicsitem.h"
class KBBGraphicsItemSet;
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief The balls the player has to place
 */
class KBBGraphicsItemBallRepository : public KBBGraphicsItem
{
	Q_OBJECT


	public:
		KBBGraphicsItemBallRepository(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager);


		int ballToTake() const;
		void fillBallsOutside(int placed);
		void newGame(int columns, int rows, int balls);
		void removeBall(int outsidePosition);


	private:
		KBBGraphicsItemSet* m_ballsOutside;
		KBBScalableGraphicWidget* m_parent;
		KBBThemeManager* m_themeManager;

		KRandomSequence m_random;

		int m_columns;
		int m_rows;
		int m_width;
		int m_height;
		int m_ballToPlace;
};

#endif // KBBGRAPHICSITEMBALLREPOSITORY_H
