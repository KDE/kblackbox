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



#ifndef KBBSCALABLEGRAPHICWIDGETNONSVGITEMS_H
#define KBBSCALABLEGRAPHICWIDGETNONSVGITEMS_H


#include <QDomElement>
#include <QString>



/**
 * @brief Get some attributes in the SVG file for items that are not SVG items
 *
 * Useful for laser ray.
 * @see KBBGraphicsItemRay
 */
class KBBScalableGraphicWidgetNonSvgItems
{
	public:
		KBBScalableGraphicWidgetNonSvgItems(QString svgzFile);
		
		
		QColor color(QString elementId);
		Qt::PenStyle style(QString elementId);
		qreal width(QString elementId);


	private:
		QString value(QString elementId, QString styleElement);
		
		QDomElement m_root;
};

#endif // KBBSCALABLEGRAPHICWIDGETNONSVGITEMS_H
