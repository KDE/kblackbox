/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBBITEMWITHPOSITION_H
#define KBBITEMWITHPOSITION_H



/**
 * @brief Item with position
 *
 * Items with position can be managed by the class KBBGraphicsItemSet.
 */
class KBBItemWithPosition
{
	public:
		virtual ~KBBItemWithPosition();
		virtual int position() = 0;

		/**
		 * @brief Destructor for dependent QGraphicsItem
		 * Some QGraphicsItem s needs to delete other dependent QGraphicsItemo s before being deleted. In this case, this methode should be reimplemented.
		 * If the object is not a QGraphicsItem, you probably don't need to reimplement it and can just reimplement the normal destructor in a child class.
		 *
		 * It's different from the normal destructor that only destroys this QGraphicsItem. By exiting KBlackBox, the QGraphicsScene is been destroyed and call in an arbitrary order the destructors of all QGraphicsItem.s. So the dependent items may be destroy before this one: That's why we cannot always destroy them in the normal destructor of this class.
		 */
		virtual void cleanDelete();


		virtual void highlight(bool);
		virtual void highlightBoth(bool);

		/**
		 * @brief Should the element changes if the game is paused?
		 * Do nothing.
		 * Reimplement this methode to do something if the game is paused.
		 */
		virtual void setPause(bool state);
};

#endif // KBBITEMWITHPOSITION_H
