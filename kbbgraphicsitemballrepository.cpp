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

#include "kbbgraphicsitemballrepository.h"



#include <QGraphicsScene>


#include "kbbgraphicsitemonbox.h"
#include "kbbgraphicsitemset.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemBallRepository::KBBGraphicsItemBallRepository(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager) : KBBGraphicsItem(KBBScalableGraphicWidget::informationBackground, parent->scene(), themeManager)
{
	m_parent = parent;
	m_themeManager = themeManager;
	m_width = 1;
	m_height = 1;
	m_ballToPlace = 0;

	m_random.setSeed(0);

	m_ballsOutside = new KBBGraphicsItemSet(parent->scene());
}


KBBGraphicsItemBallRepository::~KBBGraphicsItemBallRepository()
{
	delete m_ballsOutside;
}



//
// Public
//

int KBBGraphicsItemBallRepository::ballToTake() const
{
	return m_ballsOutside->anyItemPosition();
}


void KBBGraphicsItemBallRepository::fillBallsOutside(int placed)
{
	int i = m_columns*m_rows;
	while ((m_ballsOutside->count()+placed)<m_ballToPlace) {
		if (!(m_ballsOutside->containsVisible(i))) {
			KBBGraphicsItemOnBox* b = new KBBGraphicsItemOnBox(KBBScalableGraphicWidget::playerBall, m_parent, m_themeManager, i, m_columns, m_rows);
			m_ballsOutside->insert(b);
			b->setPos(x() + ((i - m_columns*m_rows) / m_height)*KBBScalableGraphicWidget::RATIO, y() + ((i - m_columns*m_rows) % m_height)*KBBScalableGraphicWidget::RATIO);
		}
		i++;
	}
}


void KBBGraphicsItemBallRepository::newGame(int columns, int rows, int balls)
{
	m_columns = columns;
	m_rows = rows;

	m_ballsOutside->clear();

	m_ballToPlace = balls;
	scale(1./m_width, 1./m_height);


	m_height = (rows/2);
	if (rows % 2 > 0)
		m_height++;
	m_width = balls/m_height;
	if (balls % m_height > 0)
		m_width++;

	scale(m_width/1., m_height/1.);

	setPos((-(m_width+1)*KBBScalableGraphicWidget::RATIO), KBBScalableGraphicWidget::RATIO);

	fillBallsOutside(0);
}


void KBBGraphicsItemBallRepository::removeBall(int outsidePosition)
{
	m_ballsOutside->remove(outsidePosition);
}

#include "kbbgraphicsitemballrepository.moc"
