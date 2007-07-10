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
		virtual const int position() = 0;

		/**
		 * @brief Destructor for dependent QGraphicsItem
		 * Some QGraphicsItem s needs to delete other dependent QGraphicsItemo s before being deleted. In this case, this methode should be reimplemented.
		 * If the object is not a QGraphicsItem, you probably don't need to reimplement it and can just reimplement the normal destructor in a child class.
		 *
		 * It's different from the normal destructor that only destroys this QGraphicsItem. By exiting KBlackBox, the QGraphicsScene is been destroyed and call in an arbitrary order the destructors of all QGraphicsItem.s. So the dependent items may be destroy before this one: That's why we cannot always destroy them in the normal destructor of this class.
		 */
		virtual void cleanDelete();
};

#endif // KBBITEMWITHPOSITION_H
