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



#include <QGraphicsSvgItem>


#include "kbbgraphicsiteminteractioninfo.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBGraphicsItemInteractionInfo::KBBGraphicsItemInteractionInfo( KBBScalableGraphicWidget* widget, interactionType type, const qreal x, const qreal y, const int rotation) : QGraphicsSvgItem()
{
	setSharedRenderer(widget->svgRenderer());
	setPos(x, y);
	setType(type);
	rotate(rotation);
	setZValue(KBBScalableGraphicWidget::ZVALUE_INTERACTION_INFO);
	widget->addItem(this);
}



//
// Public
//

void KBBGraphicsItemInteractionInfo::setType(interactionType type)
{
	switch (type) {
		case deflection:
			setElementId("infod");
			break;
		case reflection:
			setElementId("infor");
			break;
		case reflectionSym:
			setElementId("infos");
			break;
		case hit:
			setElementId("infoh");
			break;
		case nothing:
			setElementId("infon");
			break;
	}
}
