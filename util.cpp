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



#include "util.h"

RectOnArray::RectOnArray( int newWidth, int newHeight ) 
{
  w = newWidth;
  h = newHeight;
  array = new ArrayType[w*h];
}

RectOnArray::~RectOnArray() 
{
  delete[] array;
}

/*
   Size info...
*/

int RectOnArray::width() { return w; }
int RectOnArray::height() { return h; }

/*
   Utility function for mapping from 2D table to 1D array
*/

int RectOnArray::indexOf( int col, int row ) const
{
    return (row * w) + col;
}

/*
   Return content of cell
*/

ArrayType RectOnArray::get( int col, int row )
{
  return array[indexOf( col, row )];
}

/*
   Set content of cell
*/

void RectOnArray::set( int col, int row, ArrayType type )
{
  array[indexOf( col, row )] = type;
}

/*
   Fill all cells witj type
*/

void RectOnArray::fill( ArrayType type )
{
  int i;
  for (i = 0; i < w*h; i++) array[i] = type;
}
