/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBBGRAPHICSITEMRAYRESULT_H
#define KBBGRAPHICSITEMRAYRESULT_H



class QGraphicsScene;
class QGraphicsSvgItem;


#include "kbbgraphicsitem.h"
#include "kbbgraphicsitemborder.h"
#include "kbbitemwithposition.h"
class KBBScalableGraphicWidget;
class KBBThemeManager;



/**
 * @brief Result of a laser ray shoot in the black box
 *
 * There 3 kinds of ray results:
 *   - HIT: if the laser ray hits a ball in the black box by entering at the considered position.
 *   - REFLECTION: if the laser ray exits the black box at the entrance point.
 *   - A number to display both positions where the laser ray entered and exited the black box. Both positions are opposite positions of each other and belong together.
 */
class KBBGraphicsItemRayResult : public KBBGraphicsItemBorder, public KBBGraphicsItem, public KBBItemWithPosition
{
	public:
		KBBGraphicsItemRayResult(KBBScalableGraphicWidget* parent, KBBThemeManager* themeManager, QGraphicsScene* scene, const int borderPosition, const int columns, const int rows, const int rayNumber);


		/**
		 * @brief Destructor of the class and dependent object
		 * Call this methode to destroy simultaneously this QGraphicsItem and all other dependent QGraphicsItems: The number or the sign.
		 */
		void cleanDelete() override;

		/**
		 * @brief Highlight the item
		 * 
		 * Usually used when the mouse goes over the item or over the opposite item.
		 */
		void highlight(bool state) override;

		/**
		 * @brief Highlight the item and the opposite one
		 */
		void highlightBoth(bool state) override;

		/**
		 * @brief Get the border position
		 */
		int position() override;

		/**
		 * @brief Define the opposite "ray result" item
		 * 
		 * Used if the result is not a HIT and not a REFLECTION. In the other case, the opoosite is the item itself and this method doesn't need to be called.
		 * @param opposite Correspondent item with the same "number".
		 */
		void setOpposite(KBBGraphicsItemRayResult* opposite);

		/**
		 * @brief Set the pause state
		 */
		void setPause(bool state) override;


	private:
		void hoverEnterEvent (QGraphicsSceneHoverEvent*) override;
		void hoverLeaveEvent (QGraphicsSceneHoverEvent*) override;
		
		QString m_elementIdResultBackground;
		QString m_elementIdResultBackgroundHighlight;
		QGraphicsSvgItem* m_notNumber;
		QGraphicsSimpleTextItem* m_number;
		KBBGraphicsItemRayResult* m_opposite;
		bool m_pause;
		QGraphicsScene* m_scene;
		KBBScalableGraphicWidget* m_widget;
};

#endif // KBBGRAPHICSITEMRAYRESULT_H
