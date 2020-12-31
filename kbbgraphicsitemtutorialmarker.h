/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBBGRAPHICSITEMTUTORIALMARKER_H
#define KBBGRAPHICSITEMTUTORIALMARKER_H



#include "kbbgraphicsitem.h"
#include "kbbgraphicsitemborder.h"
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief Marker used by the tutorial
 */
class KBBGraphicsItemTutorialMarker : public KBBGraphicsItemBorder, public KBBGraphicsItem
{
	public:
		static const int NO_POSITION = -1;


		KBBGraphicsItemTutorialMarker(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, const int columns, const int rows);

		void setBorderPosition(const int borderPosition);
};

#endif // KBBGRAPHICSITEMTUTORIALMARKER_H
