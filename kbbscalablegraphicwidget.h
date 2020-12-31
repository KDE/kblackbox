/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/



#ifndef KBBSCALABLEGRAPHICWIDGET_H
#define KBBSCALABLEGRAPHICWIDGET_H


class QAction;
class QGraphicsScene;
#include <QGraphicsView>
class QLCDNumber;
class QResizeEvent;


class KGamePopupItem;
class QPushButton;


class KBBBallsOnBoard;
class KBBGameDoc;
class KBBGraphicsItem;
class KBBGraphicsItemBallRepository;
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
		static int const BORDER_SIZE = 50;

		/**
		 * @brief Width and height of a single square on the black box
		 *
		 * Note: The widget is scalable, so it's just an arbitrary default compared with other ranges.
		 */
		static int const RATIO = 25;

		/**
		 * @brief Every graphic items
		 *
		 * Values are used to define the relative heights between the displayed graphic items.
		 * @see KBBThemeManager::zValue(const KBBScalableGraphicWidget::itemType itemType);
		 */
		enum itemType {
			background=0,
			informationBackground=1,
			blackbox=2,
			blackboxGrid=3,
			tutorialMarker=4,
			markerNothing=5,
			solutionRay=6,
			playerRay=7,
			resultBackground=8,
			resultBackgroundHighlight=9,
			resultReflection=10,
			resultHit=11,
			resultText=12,
			solutionBall=13,
			playerBall=14,
			unsureBall=15,
			wrongPlayerBall=16,
			rightPlayerBall=17,
			interactionInfoDeflection=18,
			interactionInfoHit=19,
			interactionInfoNothing=20,
			interactionInfoReflection=21,
			interactionInfoReflectionSym=22,
			laser0=23,
			laser90=24,
			laser180=25,
			laser270=26,
			cursor=27
		};
		
		
		/**
		 * @brief Constructor
		 */
		explicit KBBScalableGraphicWidget(KBBGameDoc* gameDoc, KBBThemeManager* themeManager, QAction* done);
		~KBBScalableGraphicWidget();


		void addBall(int boxPosition);
		void addBall(int boxPosition, int outsidePosition);
		void addBallUnsure(const int boxPosition);
		void addMarkerNothing(const int boxPosition);
		void drawRay(const int borderPosition);
		void mouseBorderClick(const int borderPosition);
		void mouseBoxClick(const Qt::MouseButton button, int boxPosition);
		int moveBall(const int boxPositionFrom, const int boxPositionTo);
		int moveMarkerNothing(const int boxPositionFrom, const int boxPositionTo);
		void newGame(int columns, int rows, int ballNumber);

		/**
		 * @brief Message to display
		 *
		 * @param text Message. Attention: Message should not be too wide.
		 * @param time Time (in ms) the message remains displayed. 0 = forever.
		 */
		void popupText(const QString& text, int time = 5000);

		int positionAfterMovingBall(const int boxPositionFrom, const int boxPositionTo) const;

		void setPause(bool state);
		void removeAllBalls();
		void removeBall(const int boxPosition);
		void removeRay();
		void resizeEvent(QResizeEvent*) override;
		QGraphicsScene* scene();
		void setScore(int score);

		/**
		 * @brief display the solution
		 *
		 * Used at the end of the game and for the sandbox mode.
		 * @param continueGame Sould the game continue after displaying the solution? (Yes for sandbox mode, no for normal game end).
		 */
		void solve(const bool continueGame);

		void toggleCursor();

	public Q_SLOTS:
	        void cursorOff();
		void hoverMovePosition(int newPosition);
		void cursorAtNewPosition(int borderPosition);
		void keyboardEnter();
		void keyboardMoveDown();
		void keyboardMoveLeft();
		void keyboardMoveRight();
		void keyboardMoveUp();
		void keyboardSpace();


	protected:
		void drawBackground(QPainter* painter, const QRectF&) override;


	private:
		/**
		 * @brief Minimum width and height
		 *
		 * Minimum width and minimum height of the widget. The widget needs a minimal site: it is ugly if it is too small.
		 */
		static int const MINIMUM_SIZE = 250;

		static int const OFFSET_DONE_BUTTON = 12;

		void fillBallsOutside();
		void removeMarkerNothing(const int boxPosition);
		void setBallUnsure(const int boxPosition, const bool unsure);
		void setInputAccepted(bool inputAccepted);
		void switchBall();
		void switchMarker();
		void updateDoneButton();
		void useLaser(const int incomingPosition);
		
		
		// Graphics items
		KBBGraphicsItemBlackBox* m_blackbox;
		KBBGraphicsItemSet* m_balls;
		KBBGraphicsItemSet* m_ballsSolution;
		KBBGraphicsItemSet* m_ballsUnsure;
		KBBGraphicsItemCursor* m_cursor;
		KBBGraphicsItemBallRepository* m_ballRepository;
		KBBGraphicsItemSet* m_lasers;
		KBBGraphicsItemSet* m_markersNothing;
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
		int m_ballNumber;
		KBBBallsOnBoard* m_boardBalls;
		KBBBallsOnBoard* m_boardBallsPlaced;
		int m_columns;
		QAction* m_doneAction;
		QPushButton* m_doneButton;
		KBBGameDoc* m_gameDoc;
		KGamePopupItem* m_infoScore;
		bool m_inputAccepted;
		bool m_pause;
		int m_rayNumber;
		
		int m_rows;
		QGraphicsScene* m_scene; //TODO: Remove it because scene() already gives it back.
		QLCDNumber* m_score;
		KBBThemeManager* m_themeManager;
		bool m_cursorFollowsMouse; //enable cursor following
};

#endif // KBBSCALABLEGRAPHICWIDGET_H
