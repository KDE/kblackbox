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


class KBBBallsOnBoard;
class KBBBoard;
class KBBGraphicsItemBlackBox;
class KBBGraphicsItemRay;
class KBBGraphicsItemSet;
class KBBThemeManager;



/**
 * @brief Scalable graphic central widget for KBlackBox
 */
class KBBScalableGraphicWidget : public QGraphicsView
{
	Q_OBJECT

	public:
		/**
		 * @brief Width and height of a single square on the black box
		 *
		 * Note: The widget is scalable, so it's just an arbitrary default compared with other ranges.
		 */
		static int const RATIO = 50;
		
		/**
		 * @brief Distance between the black box and the widget border
		 *
		 * Note: The widget is scalable, so it's just an arbitrary default compared with other ranges.
		 * It's the minimal distance as the width/height ratio is constant and the widget may have another width/height ratio.
		 * @see RATIO
		 */
		static int const BORDER_SIZE = 100;
		
		/**
		 * @brief Every graphic items
		 *
		 * Values are used to define the relative heigths between the displayed graphic items.
		 * @see KBBThemeManager::zValue(const KBBScalableGraphicWidget::itemType itemType);
		 */
		enum itemType { blackbox=10, blackboxGrid=20, markerNothing=30, solutionRay=40, playerRay=50, resultBackground=60, resultReflection=70, resultHit=80, resultText=90, solutionBall=100, playerBall=110, unsureBall=120, wrongPlayerBall=130, interactionInfo=140, laser=150 };
		
		
		/**
		 * @brief Constructor
		 */
		explicit KBBScalableGraphicWidget(KBBBoard* parent);
		
		
		void addItem(QGraphicsItem* item);
		void clickAddBall(const int boxPosition);
		void clickAddBallNothing(const int boxPosition);
		void clickLaser(const int incomingPosition);
		void clickRemoveBall(const int boxPosition);
		void clickRemoveBallNothing(const int boxPosition);
		void clickSetBallUnsure(const int boxPosition, const bool unsure);
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


	protected:
		void drawBackground(QPainter* painter, const QRectF&);


	private:
		// Graphics items
		KBBGraphicsItemBlackBox* m_blackbox;
		KBBGraphicsItemSet* m_balls;
		KBBGraphicsItemSet* m_ballsNothing;
		KBBGraphicsItemSet* m_ballsSolution;
		KBBGraphicsItemSet* m_ballsUnsure;
		KBBGraphicsItemSet* m_lasers;
		KBBGraphicsItemSet* m_rayResults;
		KBBGraphicsItemRay* m_playerRay;
		KBBGraphicsItemRay* m_solutionRay;
		
		
		// Various member variables
		KBBBoard* m_board;
		KBBBallsOnBoard* m_boardBalls;
		KBBBallsOnBoard* m_boardBallsPlaced;
		int m_columns;
		bool m_inputAccepted;
		int m_rayNumber;
		
		/**
		 * @brief Position and size of the background in scene coordinates
		 *
		 * Used for drwing the background.
		 * @see drawBackground(QPainter* painter, const QRectF&)
		 */
		QRectF m_rectBackground;
		
		int m_rows;
		QGraphicsScene* m_scene;
		KBBThemeManager* m_themeManager;
};

#endif // KBBSCALABLEGRAPHICWIDGET_H
