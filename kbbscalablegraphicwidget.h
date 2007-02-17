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



#ifndef KBBSCALABLEGRAPHICWIDGET_H
#define KBBSCALABLEGRAPHICWIDGET_H


class QGraphicsItem;
class QGraphicsScene;
#include <QGraphicsView>
class QResizeEvent;


#include <ksvgrenderer.h>


class KBBBallsOnBoard;
class KBBBoard;
class KBBGraphicsItemBlackBox;
class KBBGraphicsItemRay;
class KBBGraphicsItemSet;



/**
 * @brief Scalable graphic central widget for KBlackBox
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
		
		void addItem(QGraphicsItem* item);
		void clickAddBall(const int boxPosition);
		void clickAddBallNothing(const int boxPosition);
		void clickLaser(const int incomingPosition);
		void clickRemoveBall(const int boxPosition);
		void clickRemoveBallNothing(const int boxPosition);
		void drawRay(const int borderPosition);
		int hHint() const;
		void newGame(const int columns, const int rows, KBBBallsOnBoard* balls, KBBBallsOnBoard* ballsPlaced);
		void removeRay();
		void resizeEvent(QResizeEvent*);
		void solve();
		KSvgRenderer* svgRenderer();
		int wHint() const;


	public slots:
		void setInputAccepted(bool inputAccepted);
		void slotDown();
		void slotInput();
		void slotLeft();
		void slotRight();
		void slotUp();


	private:
		// Graphics items
		KBBGraphicsItemSet* m_balls;
		KBBGraphicsItemSet* m_ballsNothing;
		KBBGraphicsItemSet* m_ballsSolution;
		KBBGraphicsItemSet* m_lasers;
		KBBGraphicsItemSet* m_rayResults;
		KBBGraphicsItemBlackBox* m_blackbox;
		KBBGraphicsItemRay* m_ray;
		KBBGraphicsItemRay* m_raySolution;
		
		// Various member variables
		KBBBoard* m_board;
		KBBBallsOnBoard* m_boardBalls;
		KBBBallsOnBoard* m_boardBallsPlaced;
		int m_columns;
		bool m_inputAccepted;
		int m_rayNumber;
		int m_rows;
		QGraphicsScene* m_scene;
		//QSvgRenderer* m_svgRenderer;
		KSvgRenderer m_svgRenderer;
};

#endif // KBBSCALABLEGRAPHICWIDGET_H
