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



#ifndef KBBINFOWIDGET_H
#define KBBINFOWIDGET_H


class QLabel;
#include <QWidget>


class KBBBallsGraphicWidget;
class KBBThemeManager;



/**
 * @brief Widget for the status bar displaying the number of balls left to be placed on the black box
 *
 * TODO / i18n issue: This widget cannot manage bidirectional languages: only "left to right" languages are supported. This limitation should be fixed once it is possible to fix it, by changing the way the class QHboxLayout orders the widget positions depending on the locale...
 */
class KBBInfoWidget : public QWidget
{
	public:
		/**
		 * @brief Constructor
		 */
		KBBInfoWidget(KBBThemeManager* themeManager);
		
		
		void setBalls(const int placedBalls, const int ballsToPlace);


	private:
		static const int BALL_SIZE = 50;
		
		KBBBallsGraphicWidget* m_ballsWidget;
		QLabel* m_title;
};

#endif // KBBINFOWIDGET_H
