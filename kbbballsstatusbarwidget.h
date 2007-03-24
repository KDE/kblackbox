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



#ifndef KBBBALLSSTATUSBARWIDGET_H
#define KBBBALLSSTATUSBARWIDGET_H


class QLabel;
#include <QWidget>


class KBBBallsGraphicWidget;
class KBBThemeManager;



/**
 * @brief Widget for the status bar displaying the number of balls left to be placed on the black box
 */
class KBBBallsStatusBarWidget : public QWidget
{
	public:
		/**
		 * @brief Constructor
		 */
		KBBBallsStatusBarWidget(KBBThemeManager* themeManager);
		
		
		void setBalls(const int placedBalls, const int ballsToPlace);


	private:
		static const int BALL_SIZE = 50;
		
		KBBBallsGraphicWidget* m_ballsWidget;
		QLabel* m_title;
};

#endif // KBBBALLSSTATUSBARWIDGET_H
