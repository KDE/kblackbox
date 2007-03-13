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
 * This class is useful for laser rays.
 * Laser rays inherit QGraphicsPathItem and not KBBGraphicsItem (i.e. not QGraphicsSvgItem). But we want to be able to configure their attributes (color, width and style) to suit the graphic theme. All other items are saved as SVG items in a single theme file. To make it easy to manage themes, the laser ray attributes are also saved in the same SVG file. And as SVG is a XML format, <b>this class parses the XML of the SVG file</b>.
 *
 * Prerequisites: The SVG file must follow a strict structure.
 * - Laser ray elements must be defined in a XML node that is a direct child of the root SVG node. It won't work for instance if there is a "layer" node between the XML node of the laser ray and the root node.
 * - Just the attribute "style" of the laser ray node is used. The color is the value of "stroke:", the width is the value of "stroke-width:", and the style is the value of "stroke-dasharray:". All other values are ignored.
 * - The id attribute of the node is important, but the node tag is not. ("g" or "path" or whatever is OK).
 *
 * @see KBBGraphicsItemRay
 */
class KBBScalableGraphicWidgetNonSvgItems
{
	public:
		/**
		 * @brief Constructor
		 *
		 * The constructor opens the SVGZ file and get the DOM root node.
		 * @param &svgzFile Path to the SVGZ file
		 */
		KBBScalableGraphicWidgetNonSvgItems(const QString &svgzFile);
		
		
		/**
		 * @brief Get the color of the item
		 *
		 * @param &elementId Value of the attribute "id" of the XML node of the considered item.
		 * @return The color
		 */
		QColor color(const QString &elementId);
		
		/**
		 * @brief Get the pen style of the item
		 *
		 * @param &elementId Value of the attribute "id" of the XML node of the considered item.
		 * @return Qt::DotLine except if the style value "stroke-dasharray" is "none". In that case: Qt::SolidLine
		 */
		Qt::PenStyle style(const QString &elementId);
		
		/**
		 * @brief Get the width of the item
		 *
		 * @param &elementId Value of the attribute "id" of the XML node of the considered item.
		 * @return The stroke width
		 */
		qreal width(const QString &elementId);


	private:
		/**
		 * @brief Get the value of the style attribute
		 *
		 * @param &elementId Value of the attribute "id" of the XML node of the considered item.
		 * @param &styleElement Element of the attribute "style" to consider
		 * @return Value of the given element in the attribute "style" of the give XML node
		 */
		QString value(const QString &elementId, const QString &styleElement);
		
		/**
		 * @brief Root element of the DOM tree of the XML theme file
		 */
		QDomElement m_root;
};

#endif // KBBSCALABLEGRAPHICWIDGETNONSVGITEMS_H
