//
//
// KBlackBox
//
// A simple game inspired by an emacs module
// 
// File: kbbgfx.h
//
// The definition of the KBBGraphic widget
// 

#ifndef KBBGFX_H
#define KBBGFX_H

#include <qtablevw.h>
#include <qpixmap.h>

#include "util.h"

/*
   Default size of a cell
*/

#define CELLW 24
#define CELLH 24

/*
   Graphical types of the boxes
*/

#define OUTERBBG  0  
#define INNERBBG  1
#define LASERBBG  2
#define FBALLBBG  3
#define TBALLBBG  4
#define WBALLBBG  5
#define MARK1BBG  6 
#define LFIREBBG  7

/*
  These have the same pixmaps as some of those above...
*/

#define RLASERBBG 8
#define HLASERBBG 9

#define NROFTYPES 8

/*
   Negative numbers are deflected lasers... 
*/

class KBBGraphic : public QTableView
{
  Q_OBJECT
public:
  KBBGraphic( int w, int h, QPixmap** p=0,
	      QWidget* parent=0, const char* name=0 );
  KBBGraphic(  QPixmap** p=0, QWidget* parent=0, const char* name=0 );
  ~KBBGraphic();

  friend class KBBGame;
  
  void setSize( int w, int h );
  RectOnArray *getGraphicBoard();
  int numC();
  int numR();
  int width();
  int height();
  int wHint();
  int hHint();

public slots:
  void setInputAccepted( bool b );
  void updateElement( int col, int row );
  void setTopLeft( int col, int row );

signals:
  void sizeChanged();
  void inputAt( int, int, int );
  void endMouseClicked();

protected:
  void paintEvent( QPaintEvent* e );
  void paintCell( QPainter*, int row, int col );
  void mousePressEvent( QMouseEvent* );
  void mouseMoveEvent( QMouseEvent* );
  void keyPressEvent( QKeyEvent* );
  void focusInEvent( QFocusEvent* );
  void focusOutEvent( QFocusEvent* );
  void resizeEvent( QResizeEvent* e );

private:
  void paintCellDefault( QPainter*, int row, int col );
  void paintCellPixmap( QPainter*, int row, int col );
  void scalePixmaps( int w, int h );
  RectOnArray *graphicBoard;
  int curRow;
  int curCol;
  bool inputAccepted;
  int minW;
  int minH;
  QPixmap** pix;
  QPixmap** pixScaled;
};

#endif // KBBGFX_H
