//
// KBlackBox
//
// A simple game inspired by an emacs module
//
/***************************************************************************
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



#ifndef KBBTHEMEMANAGER_H
#define KBBTHEMEMANAGER_H


#include <QDomElement>
class QString;


#include <ksvgrenderer.h>


#include "kbbscalablegraphicwidget.h"



/**
 * @brief Theme manager of the scalable graphic widget
 *
 * This class manages the graphic theme of the central widget.
 * Graphics elements of the game are mainly SVG items. Some dynamic elements (like laser rays) are not SVG items but their theme attributes are nonetheless saved in the SVG file.
 * @see value;
 */
class KBBThemeManager
{
	public:
		/**
		 * @brief Constructor
		 *
		 * The constructor opens the SVGZ file, creates the SVG renderer and gets the DOM root node.
		 * @param &svgzFileName Path to the SVGZ file
		 */
		KBBThemeManager(const QString &svgzFileName);
		
		
		/**
		 * @brief Get the color of the item
		 *
		 * @param &itemType Type of the considered item
		 * @return The color
		 */
		QColor color(const KBBScalableGraphicWidget::itemType itemType);
		
		/**
		 * @brief Get the XML "id" value of the item
		 *
		 * In the SVG file, each graphic element has an ID that is saved in the attribute "id" of every XML node.
		 * This methode maps the item type with its XML "id" value.
		 *
		 * @param &itemType Type of the considered item
		 * @return The XML attribute "id" of the item
		 */
		QString elementId(const KBBScalableGraphicWidget::itemType itemType);
		
		/**
		 * @brief Get the pen style of the item
		 *
		 * @param &itemType Type of the considered item
		 * @return Qt::DotLine except if the style value "stroke-dasharray" is "none". In that case: Qt::SolidLine
		 */
		Qt::PenStyle style(const KBBScalableGraphicWidget::itemType itemType);
		
		/**
		 * @brief Get the shared SVG renderer
		 * 
		 * @return The SVG renderer
		 */
		KSvgRenderer* svgRenderer();
		
		/**
		 * @brief Get the width of the item
		 *
		 * @param &itemType Type of the considered item
		 * @return The stroke width
		 */
		qreal width(const KBBScalableGraphicWidget::itemType itemType);
		
		/**
		 * @brief Get the relative height of the item
		 *
		 * Items are stacked horizontally. It's important to know what item is over or unter what other item.
		 *
		 * @param &itemType Type of the considered item
		 * @return item relative height
		 */
		int zValue(const KBBScalableGraphicWidget::itemType itemType);


	private:
		/**
		 * @brief Get the value of the style attribute in the SVG file for a non SVG item
		 *
		 * This methode is useful for laser rays or for the grid of the black box.
		 * Laser rays (for instance) inherit QGraphicsPathItem and not KBBGraphicsItem (i.e. not QGraphicsSvgItem). But we want to be able to configure their attributes (color, width and style) to suit the graphic theme. All other items are saved as SVG items in a single theme file. To make it easy to manage themes, the laser ray attributes are also saved in the same SVG file. And as SVG is a XML format, <b>this methode of this class parses the XML of the SVG file</b>.
		 *
		 * Prerequisites: The SVG file must follow a strict structure.
		 * - Laser ray elements must be defined in a XML node that is a direct child of the root SVG node. It won't work for instance if there is a "layer" node between the XML node of the laser ray and the root node.
		 * - Just the attribute "style" of the laser ray node is used. The color is the value of "stroke:", the width is the value of "stroke-width:", and the style is the value of "stroke-dasharray:". All other values are ignored.
		 * - The id attribute of the node is important, but the node tag is not. ("g" or "path" or whatever is OK).
		 *
		 * @param &itemType Type of the considered item
		 * @param &styleElement Element of the attribute "style" to consider
		 * @return Value of the given element in the attribute "style" of the give XML node
		 * @see KBBGraphicsItemRay
		 */
		QString value(const KBBScalableGraphicWidget::itemType itemType, const QString &styleElement);
		
		/**
		 * @brief Root element of the DOM tree of the XML theme file
		 */
		QDomElement m_root;
		
		/**
		 * @brief SVG Renderer
		 *
		 * The SVG renderer is shared by all SVG graphic items.
		 */
		KSvgRenderer m_svgRenderer;
};

#endif // KBBTHEMEMANAGER_H
