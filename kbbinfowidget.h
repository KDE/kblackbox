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


class KGameLCD;


class KBBBallsGraphicWidget;
class KBBThemeManager;



/**
 * @brief Widget displaying game information
 *
 * Displayed information: 
 * - the number of balls left to be placed on the black box
 * - the score
 */
class KBBInfoWidget : public QWidget
{
	public:
		/**
		 * @brief Constructor
		 */
		KBBInfoWidget(KBBThemeManager* themeManager);
		
		
		void setGameParameters(const int ballsToPlace, const int scoreLimit);
		void setPlacedBalls(const int placedBalls);
		void setScore(const int score);


	private:
		static const int BALL_SIZE = 50;
		
		QLabel* m_ballsTitle;
		int m_ballsToPlace;
		KBBBallsGraphicWidget* m_ballsWidget;
		KGameLCD* m_score;
		int m_scoreLimit;

		/**
		 * @brief Set if the variable m_scoreLimit has been defined
		 *
		 * If no score limit is defined, the widget don't highlight the score value in the LCD.
		 * @see m_scoreLimit
		 */
		bool m_scoreLimitDefinied;
};

#endif // KBBINFOWIDGET_H
