/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kbbitemwithposition.h"



//
// Constructor / Destructor
//

KBBItemWithPosition::~KBBItemWithPosition()
{
}



//
// Public
//

void KBBItemWithPosition::cleanDelete()
{
	delete this;
}


void KBBItemWithPosition::highlight(bool)
{
}


void KBBItemWithPosition::highlightBoth(bool)
{
}


void KBBItemWithPosition::setPause(bool)
{
}
