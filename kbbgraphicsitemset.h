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
