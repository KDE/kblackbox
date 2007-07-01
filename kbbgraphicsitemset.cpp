//
// KBlackbox
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

#include "kbbgraphicsitemset.h"



#include <QGraphicsScene>


#include "kbbgraphicsitem.h"
#include "kbbitemwithposition.h"



//
// Constructor / Destructor
//

KBBGraphicsItemSet::KBBGraphicsItemSet(QGraphicsScene* scene)
{
        m_scene = scene;
}


KBBGraphicsItemSet::~KBBGraphicsItemSet()
{
	clear();
}



//
// Public
//

void KBBGraphicsItemSet::clear()
{
	while (m_items.count()>0) {
		remove(m_items[m_items.keys().last()]->position());
	}
}


bool KBBGraphicsItemSet::contains(int position)
{
	if (m_items.contains(position)) {
		if(dynamic_cast<KBBGraphicsItem*>(m_items[position]))
			return (dynamic_cast<KBBGraphicsItem*>(m_items[position]))->isVisible();
		else
			return true;
	} else
		return false;

	return m_items.contains(position);
}


void KBBGraphicsItemSet::insert(KBBItemWithPosition* item)
{
	if (m_items.contains(item->position()))
		// We want to avoid duplicated item on a given position.
		item->cleanDelete();
	else
		m_items.insert(item->position(), item);
}


void KBBGraphicsItemSet::remove(int position)
{
	if (m_items.contains(position)) {
		m_items[position]->cleanDelete();
		m_scene->update();
		m_items.remove(position);
	}
}


void KBBGraphicsItemSet::setVisible(int position, const bool visible) const
{
	if (m_items.contains(position)) {
		if(dynamic_cast<KBBGraphicsItem*>(m_items[position]))
			(dynamic_cast<KBBGraphicsItem*>(m_items[position]))->setVisible(visible);
	}
}
