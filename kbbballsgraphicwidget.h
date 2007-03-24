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



#ifndef KBBBALLSGRAPHICWIDGET_H
#define KBBBALLSGRAPHICWIDGET_H


class QGraphicsScene;
#include <QGraphicsView>
#include <QList>


class KBBBallsGraphicWidget;
class KBBGraphicsItem;
class KBBThemeManager;



/**
 * @brief Widget displaying the number of balls left to be placed on the black box
 */
class KBBBallsGraphicWidget : public QGraphicsView
{
	public:
		/**
		 * @brief Constructor
		 */
		KBBBallsGraphicWidget(const int ballSize, KBBThemeManager* themeManager);
		
		~KBBBallsGraphicWidget();
		
		
		void resizeEvent(QResizeEvent*);
		void setBallsToPlace(const int ballsToPlace);
		void setPlacedBalls(const int placedBalls);


	private:
		int m_ballSize;
		QList<KBBGraphicsItem*> m_balls;
		int m_ballsToPlace;
		QGraphicsScene* m_scene;
		KBBThemeManager* m_themeManager;
};

#endif // KBBBALLSGRAPHICWIDGET_H
