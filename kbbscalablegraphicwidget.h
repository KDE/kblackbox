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


class QGraphicsScene;
#include <QGraphicsView>
class QResizeEvent;


class KBBBallsOnBoard;
class KBBGameDoc;
class KBBGraphicsItem;
class KBBGraphicsItemBlackBox;
class KBBGraphicsItemCursor;
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
		 * @brief Distance between the black box and the widget border
		 *
		 * Note: The widget is scalable, so it's just an arbitrary default compared with other ranges.
		 * It's the minimal distance as the width/height ratio is constant and the widget may have another width/height ratio.
		 * @see RATIO
		 */
		static int const BORDER_SIZE = 100;

		/**
		 * @brief Width and height of a single square on the black box
		 *
		 * Note: The widget is scalable, so it's just an arbitrary default compared with other ranges.
		 */
		static int const RATIO = 50;

		/**
		 * @brief Every graphic items
		 *
		 * Values are used to define the relative heigths between the displayed graphic items.
		 * @see KBBThemeManager::zValue(const KBBScalableGraphicWidget::itemType itemType);
		 */
		enum itemType {
			background=0,
			blackbox=1,
			blackboxGrid=2,
			tutorialMarker=3,
			markerNothing=4,
			solutionRay=5,
			playerRay=6,
			resultBackground=7,
			resultBackgroundHighlight=8,
			resultReflection=9,
			resultHit=10,
			resultText=11,
			solutionBall=12,
			playerBall=13,
			unsureBall=14,
			wrongPlayerBall=15,
			interactionInfoDeflection=16,
			interactionInfoHit=17,
			interactionInfoNothing=18,
			interactionInfoReflection=19,
			interactionInfoReflectionSym=20,
			laser0=21,
			laser90=22,
			laser180=23,
			laser270=24,
			cursor=25
		};
		
		
		/**
		 * @brief Constructor
		 */
		explicit KBBScalableGraphicWidget(KBBGameDoc* gameDoc, KBBThemeManager* themeManager);


		void addBall(const int boxPosition);
		void addBallUnsure(const int boxPosition);
		void addMarkerNothing(const int boxPosition);
		void drawRay(const int borderPosition);
		void mouseBorderClick(const int borderPosition);
		void mouseBoxClick(const Qt::MouseButton button, const int boxPosition);
		void newGame(const int columns, const int rows);
		void removeAllBalls();
		void removeBall(const int boxPosition);
		void removeRay();
		void resizeEvent(QResizeEvent*);
		QGraphicsScene* scene();
		
		/**
		 * @brief display the solution
		 *
		 * Used at the end of the game and for the sandbox mode.
		 * @param continueGame Sould the game continue after displaying the solution? (Yes for sandbox mode, no for normal game end).
		 */
		void solve(const bool continueGame);


	public slots:
		void cursorAtNewPosition(const int borderPosition);
		void keyboardEnter();
		void keyboardMoveDown();
		void keyboardMoveLeft();
		void keyboardMoveRight();
		void keyboardMoveUp();
		void keyboardSpace();


	protected:
		void drawBackground(QPainter* painter, const QRectF&);


	private:
		/**
		 * @brief Minimum width and height
		 *
		 * Minimum width and minimum height of the widget. The widget needs a minimal site: it is ugly if it is too small.
		 */
		static int const MINIMUM_SIZE = 250;

		void removeMarkerNothing(const int boxPosition);
		void setBallUnsure(const int boxPosition, const bool unsure);
		void setInputAccepted(bool inputAccepted);
		void switchBall();
		void switchMarker();
		void useLaser(const int incomingPosition);
		
		
		// Graphics items
		KBBGraphicsItemBlackBox* m_blackbox;
		KBBGraphicsItemSet* m_balls;
		KBBGraphicsItemCursor* m_cursor;
		KBBGraphicsItemSet* m_markersNothing;
		KBBGraphicsItemSet* m_ballsSolution;
		KBBGraphicsItemSet* m_ballsUnsure;
		KBBGraphicsItemSet* m_lasers;
		KBBGraphicsItemSet* m_rayResults;
		KBBGraphicsItemRay* m_playerRay;
		KBBGraphicsItemRay* m_solutionRay;
		
		
		/**
		 * @brief Position and size of the background in scene coordinates
		 *
		 * Used for drwing the background.
		 * @see drawBackground(QPainter* painter, const QRectF&)
		 */
		QRectF m_rectBackground;
		
		// Various member variables
		KBBGameDoc* m_gameDoc;
		KBBBallsOnBoard* m_boardBalls;
		KBBBallsOnBoard* m_boardBallsPlaced;
		int m_columns;
		bool m_inputAccepted;
		int m_rayNumber;
		
		int m_rows;
		QGraphicsScene* m_scene;
		KBBThemeManager* m_themeManager;
};

#endif // KBBSCALABLEGRAPHICWIDGET_H
