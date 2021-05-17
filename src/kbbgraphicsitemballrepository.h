/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBBGRAPHICSITEMBALLREPOSITORY_H
#define KBBGRAPHICSITEMBALLREPOSITORY_H

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
		~KBBGraphicsItemBallRepository();


		int ballToTake() const;
		void fillBallsOutside(int placed);
		void newGame(int columns, int rows, int balls);
		void removeBall(int outsidePosition);


	private:
		KBBGraphicsItemSet* m_ballsOutside;
		KBBScalableGraphicWidget* m_parent;
		KBBThemeManager* m_themeManager;

		int m_columns;
		int m_rows;
		int m_width;
		int m_height;
		int m_ballToPlace;
};

#endif // KBBGRAPHICSITEMBALLREPOSITORY_H
