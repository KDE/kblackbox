/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/



#ifndef KBBGRAPHICSITEMBLACKBOX_H
#define KBBGRAPHICSITEMBLACKBOX_H


class QGraphicsLineItem;
#include <QGraphicsRectItem>
class QGraphicsScene;
class QGraphicsView;
#include <QList>
#include <QPen>


class KBBGraphicsItem;
#include "kbbscalablegraphicwidget.h"
#include <QObject>


/**
 * @brief The black box in the scalable graphic widget
 */
class KBBGraphicsItemBlackBox : public QObject, public QGraphicsRectItem
{
Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 */
		KBBGraphicsItemBlackBox(QGraphicsView* parent, QGraphicsScene* scene, KBBThemeManager* themeManager, bool isPreview);


		/**
		 * @brief Set the KBBScalableGraphicWidget
		 */
		void setKBBScalableGraphicWidget(KBBScalableGraphicWidget* w);

		/**
		 * @brief Define the (new) size of the black box
		 *
		 * Used to "recycle" the black box for a new game.
		 */
		void setSize(const int columns, const int rows);


	private:
		void mousePressEvent (QGraphicsSceneMouseEvent* event) override;
		void hoverMoveEvent (QGraphicsSceneHoverEvent* event) override;
		void hoverLeaveEvent (QGraphicsSceneHoverEvent* event) override;

		KBBGraphicsItem* m_background;
		int m_columns;
		QList<QGraphicsLineItem*> m_lines;
		QPen m_penLines;
		int m_rows;
		QGraphicsScene* m_scene;
		KBBScalableGraphicWidget* m_widget;
		int m_zValueLines;
 public:
	  Q_SIGNALS:
		void hoverMoved(int);
		void hoverExit();
};

#endif // KBBGRAPHICSITEMBLACKBOX_H
