//
//
// KBlackBox
//
// A simple game inspired by an emacs module
// 
// File: util.cpp
//
// The implementation of the RectOnArray class
// 

#include "util.h"

RectOnArray::RectOnArray( int w, int h ) 
{
  array = new ArrayType[w*h];
  this->w = w;
  this->h = h;
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
