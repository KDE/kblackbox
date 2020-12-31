/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBBGRAPHICSITEM_H
#define KBBGRAPHICSITEM_H



#include <QGraphicsScene>
#include <QGraphicsSvgItem>


#include "kbbscalablegraphicwidget.h"
class KBBThemeManager;



/**
 * @brief Graphic item of the scalable graphic widget
 */
class KBBGraphicsItem : public QGraphicsSvgItem
{
	public:
		/**
		 * @brief Constructor
		 */
		KBBGraphicsItem(KBBScalableGraphicWidget::itemType itemType, QGraphicsScene* scene, KBBThemeManager* themeManager);
};

#endif // KBBGRAPHICSITEM_H
