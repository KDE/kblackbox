/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/



#ifndef KBBBALLSGRAPHICWIDGET_H
#define KBBBALLSGRAPHICWIDGET_H


class QGraphicsScene;
#include <QGraphicsView>
#include <QList>


class KBBGraphicsItem;
class KBBThemeManager;



/**
 * @brief Widget displaying the number of balls to place on the black box
 *
 * This is always the number of balls remaining to be placed. If this number is negative because the player placed too many balls, the widget displays how many balls the player should remove from the black box.
 */
class KBBBallsGraphicWidget : public QGraphicsView
{
	public:
		/**
		 * @brief Constructor
		 *
		 * @param themeManager Pointer to the theme manager to display the right graphic items.
		 */
		explicit KBBBallsGraphicWidget(KBBThemeManager* themeManager);
		
		/**
		 * @brief Destructor
		 * Remove all items displayed on the scene.
		 */
		~KBBBallsGraphicWidget();
		
		
		/**
		 * @brief Event: widget has been resized
		 * This happens for instance when the main window has been resized.
		 */
		void resizeEvent(QResizeEvent*) override;
		
		/**
		 * @brief Define the number of balls to place on the black box
		 * 
		 * This defines also the maximum number of balls (or items) to display on this widget.
		 * @param ballsToPlace Number of balls to place
		 */
		void setBallsToPlace(const int ballsToPlace);
		
		/**
		 * @brief Define the number of balls the player already placed on the board
		 *
		 * So the number of balls to display on this widget is "Number of balls to place" - this. (if >=0).
		 * @param placedBalls Number of balls placed on the black box.
		 */
		void setPlacedBalls(const int placedBalls);


	private:
		int m_ballsToPlace;
		QList<KBBGraphicsItem*> m_playerBalls;
		QGraphicsScene* m_scene;
		KBBThemeManager* m_themeManager;
		QList<KBBGraphicsItem*> m_wrongPlayerBalls;
};

#endif // KBBBALLSGRAPHICWIDGET_H
