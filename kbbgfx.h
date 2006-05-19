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

#include <QWidget>

#include "util.h"

/*
   Default size of a cell
*/

#define CELLW 32
#define CELLH 32

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

class KBBGraphic : public QWidget
{
  Q_OBJECT
public:
  KBBGraphic(  QPixmap** p=0, QWidget* parent=0 );
  ~KBBGraphic();

  friend class KBBGame;
  
  void setSize( int w, int h );
  RectOnArray *getGraphicBoard();
  int numC();
  int numR();
  int width();
  int height();
  int wHint() const;
  int hHint() const;
  void setCellWidth( int w );
  void setCellHeight( int h );
  void setNumRows( int rows );
  void setNumCols( int cols );

public slots:
  void setInputAccepted( bool b );
  void updateElement( int col, int row );
  void slotUp();
  void slotDown();
  void slotLeft();
  void slotRight();
  void slotInput();

signals:
  void sizeChanged();
  void inputAt( int, int, int );
  void endMouseClicked();

protected:
  virtual QSize sizeHint() const;
  void paintEvent( QPaintEvent* );
  void mousePressEvent( QMouseEvent* );
  void mouseMoveEvent( QMouseEvent* );
  void focusInEvent( QFocusEvent* );
  void focusOutEvent( QFocusEvent* );
  void resizeEvent( QResizeEvent* e );

  void moveSelection(int drow, int dcol);

private:
  void paintCell( QPainter* p, int row, int col );
  void paintCellDefault( QPainter*, int row, int col );
  void paintCellPixmap( QPainter*, int row, int col );
  void scalePixmaps( int w, int h );
  RectOnArray *graphicBoard;
  int curRow;
  int curCol;
  bool inputAccepted;
  int minW;
  int minH;
  int cellW;
  int cellH;
  int numCols;
  int numRows;
  QPixmap **pix;
  QPixmap **pixScaled;
  QPixmap *drawBuffer;
};

#endif // KBBGFX_H
