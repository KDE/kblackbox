/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBBGRAPHICSITEMSET_H
#define KBBGRAPHICSITEMSET_H



class QGraphicsScene;
#include <QList>


class KBBItemWithPosition;



/**
 * @brief Set of graphic items with positions
 */
class KBBGraphicsItemSet
{
	public:
		explicit KBBGraphicsItemSet(QGraphicsScene* scene);
		~KBBGraphicsItemSet();


		static const int NO_INDEX = -1;

		/**
		 * @brief A position of an item (anyone of them)
		 */
		int anyItemPosition();

		/**
		 * @brief Number of items
		 */
		int count() const;

		/**
		 * @brief Remove all items
		 */
		void clear();

		/**
		 * If an element is not visible, it is not contained.
		 * @return false if the element is not contained or contained but not visible.
		 */
		bool containsVisible(int position);

		/**
		 * @brief Insert an item in the list
		 *
		 * @param item Item to insert. It must have a position: a box position or a border position.
		 */
		void insert(KBBItemWithPosition* item);

		/**
		 * @brief Return the item at the given position
		 *
		 * @param position Position of the item.
		 */
		KBBItemWithPosition* item(int position);

		/**
		 * @brief Remove item at given position
		 *
		 * @param position Position of the item to be removed.
		 */
		void remove(int position);

		/**
		 * @brief Change the visibility of an element
		 */
		void setVisible(const int position, const bool visible);


	private:
		bool contains(int position);
		int indexOf(int position);

		QGraphicsScene* m_scene;
		QList<KBBItemWithPosition*> m_items;
};

#endif // KBBGRAPHICSITEMSET_H
