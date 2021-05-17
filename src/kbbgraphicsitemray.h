/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/



#ifndef KBBGRAPHICSITEMRAY_H
#define KBBGRAPHICSITEMRAY_H


class QColor;
#include <QGraphicsPathItem>
class QGraphicsScene;


class KBBBallsOnBoard;
#include "kbbgraphicsitemborder.h"
#include "kbbscalablegraphicwidget.h"
class KBBThemeManager;



/**
 * @brief Ray on the scalable graphic widget of KBlackBox
 *
 */
class KBBGraphicsItemRay : public KBBGraphicsItemBorder, public QGraphicsPathItem
{
	public:
		//KBBGraphicsItemRay(QGraphicsScene* scene, QColor color, const qreal width, Qt::PenStyle style, const int zValue);
		/**
		 * @brief Constructor
		 */
		KBBGraphicsItemRay(KBBScalableGraphicWidget::itemType itemType, QGraphicsScene* scene, KBBThemeManager* themeManager);
		
		
		void draw(KBBBallsOnBoard* ballsOnBoard, const int borderPosition);
		void hide();
};

#endif // KBBGRAPHICSITEMRAY_H
