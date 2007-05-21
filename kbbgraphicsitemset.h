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


class KBBItemWithPosition;
#include <QMap>



/**
 * @brief Set of graphic items
 */
class KBBGraphicsItemSet
{
	public:
		KBBGraphicsItemSet(QGraphicsScene* scene);
		~KBBGraphicsItemSet();
		
		void clear();

		/**
		 * If an element is not visible, it is not contained.
		 * @return false if the element is not contained or contained but not visible.
		 */
		bool contains(int position);

		void insert(KBBItemWithPosition* item);
		void remove(int position);

		/**
		 * @brief Change the visibility of an element
		 */
		void setVisible(const int position, const bool visible) const;


	private:
		QGraphicsScene* m_scene;
		QMap<int, KBBItemWithPosition*> m_items;
};

#endif // KBBGRAPHICSITEMSET_H
