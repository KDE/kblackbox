//
//
// KBlackBox
//
// A simple game inspired by an emacs module
// 
// File: util.h
//
// The definition of the RectOnArray class
// 

#ifndef UTIL_H
#define UTIL_H

/*
   This is used for gameBoard and graphicBoard fields
*/

#define ArrayType int

class RectOnArray
{
public:
  RectOnArray( int w, int h );
  ~RectOnArray();

  int get( int col, int row );
  void set( int col, int row, ArrayType type );
  void fill( ArrayType type );

  int width();
  int height();

private:
  int indexOf( int col, int row ) const;

  int w;
  int h;
  ArrayType *array;
};

#endif // UTIL_H
