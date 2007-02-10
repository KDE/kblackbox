//
// KBlackbox
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



#ifndef KBBSCALABLEGRAPHICWIDGET_H
#define KBBSCALABLEGRAPHICWIDGET_H


#include <QGraphicsView>
class QGraphicsScene;
#include <QMap>
class QResizeEvent;
class QSvgRenderer;


class KBBBallsOnBoard;
class KBBBoard;
class KBBGraphicsItemBall;
class KBBGraphicsItemBlackBox;
class KBBGraphicsItemLaser;
class KBBGraphicsItemRay;
class KBBGraphicsItemRayResult;



/**
 * @brief Scalable graphic central widget for KBlackBox
 *
 */
class KBBScalableGraphicWidget : public QGraphicsView
{
	Q_OBJECT

	public:
		static int const RATIO = 50;
		static int const BORDER_SIZE = 100;
		
		static int const ZVALUE_BACKGROUND = 10;
		static int const ZVALUE_BLACKBOX = 20;
		static int const ZVALUE_BLACKBOX_GRID = 30;
		static int const ZVALUE_BLACKBOX_BACKGROUND = 40;
		static int const ZVALUE_MARKER_NOTHING = 50;
		static int const ZVALUE_SOLUTION_RAY = 60;
		static int const ZVALUE_PLAYER_RAY = 70;
		static int const ZVALUE_RAY_RESULT_BACKGROUND = 80;
		static int const ZVALUE_RAY_RESULT_TEXT = 90;
		static int const ZVALUE_BALL_RED = 100;
		static int const ZVALUE_BALL_BLUE = 110;
		static int const ZVALUE_BALL_CROSS = 120;
		static int const ZVALUE_LASER = 130;

		
		explicit KBBScalableGraphicWidget(KBBBoard* parent);
		
		void clickAddBall(const int boxPosition);
		void clickAddBallNothing(const int boxPosition);
		void clickLaser(KBBGraphicsItemLaser* laser);
		void clickRemoveBall(const int boxPosition);
		void clickRemoveBallNothing(const int boxPosition);
		void drawRay(const int borderPosition);
		int hHint() const;
		void newGame(const int columns, const int rows, KBBBallsOnBoard* balls, KBBBallsOnBoard* ballsPlaced);
		void removeRay();
		void resizeEvent(QResizeEvent*);
		void solve();
		int wHint() const;


	public slots:
		void setInputAccepted(bool inputAccepted);
		void slotDown();
		void slotInput();
		void slotLeft();
		void slotRight();
		void slotUp();


	signals:
		void endMouseClicked();


	private:
		void removeBall(KBBGraphicsItemBall* ball);
		void removeBallNothing(KBBGraphicsItemBall* ball);
		void removeBallSolution(KBBGraphicsItemBall* ball);
		void removeLaser( KBBGraphicsItemLaser* laser);
		void removeRayResult( KBBGraphicsItemRayResult* rayResult);
		
		// Graphics Items
		QMap<int, KBBGraphicsItemBall*> m_balls;
		QMap<int, KBBGraphicsItemBall*> m_ballsNothing;
		QMap<int, KBBGraphicsItemBall*> m_ballsSolution;
		KBBGraphicsItemBlackBox* m_blackbox;
		QMap<int, KBBGraphicsItemLaser*> m_lasers;
		QMap<int, KBBGraphicsItemRayResult*> m_rayResults;
		
		// Various member variables
		KBBBallsOnBoard* m_boardBalls;
		KBBBallsOnBoard* m_boardBallsPlaced;
		QSvgRenderer* m_ballSvg;
		KBBBoard* m_board;
		int m_columns;
		bool m_inputAccepted;
		int m_rows;
		KBBGraphicsItemRay* m_ray;
		KBBGraphicsItemRay* m_raySolution;
		int m_rayNumber;
		QGraphicsScene* m_scene;
};

#endif // KBBSCALABLEGRAPHICWIDGET_H
