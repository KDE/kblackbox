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

int KBBGraphicsItemSet::anyItemPosition()
{
        if (!m_items.isEmpty())
		return m_items.last()->position();
	else
		return NO_INDEX;
}


int KBBGraphicsItemSet::count() const
{
	return m_items.count();
}


void KBBGraphicsItemSet::clear()
{
	while (m_items.count()>0) {
		remove(m_items.last()->position());
	}
}


bool KBBGraphicsItemSet::contains(int position)
{
	return (indexOf(position)!=NO_INDEX);
}


bool KBBGraphicsItemSet::containsVisible(int position)
{
	int i = indexOf(position);

	if (i!=NO_INDEX) {
		if(dynamic_cast<KBBGraphicsItem*>(m_items[i]))
			return ((dynamic_cast<KBBGraphicsItem*>(m_items[i]))->isVisible());
		else
			return true;
	} else
		return false;
}


void KBBGraphicsItemSet::insert(KBBItemWithPosition* item)
{
	if (contains(item->position()))
		// We want to avoid duplicated item on a given position.
		item->cleanDelete();
	else
		m_items.append(item);
}


KBBItemWithPosition* KBBGraphicsItemSet::item(int position)
{
	KBBItemWithPosition* r = nullptr;

	for (int i=0;i<m_items.count();i++)
		if (m_items[i]->position()==position)
			r = m_items[i];
	return r;
}


void KBBGraphicsItemSet::remove(int position)
{
	int i = indexOf(position);

	if (i!=NO_INDEX) {
		m_items[i]->cleanDelete();
		m_scene->update();
		m_items.removeAt(i);
	}
}


void KBBGraphicsItemSet::setVisible(const int position, const bool visible)
{
	int i = indexOf(position);

	if (i!=NO_INDEX) {
		if(dynamic_cast<KBBGraphicsItem*>(m_items[i]))
			(dynamic_cast<KBBGraphicsItem*>(m_items[i]))->setVisible(visible);
	}
}



//
// Private
//

int KBBGraphicsItemSet::indexOf(int position)
{
	for(int i=0;i<m_items.count();i++)
		if (m_items[i]->position()==position)
			return i;

	return NO_INDEX;
}
