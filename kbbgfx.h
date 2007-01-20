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


class KBBBoard;
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
   Types of the boxes (used f.e.g. in the traceRay() method)
*/
#define OUTERBBT 0
#define INNERBBT 1
#define LASERBBT 2
#define BALLBBT  3


/*
   Ray-tracing results.
*/
#define WRONGSTART -1
#define DETOUR      0
#define REFLECTION  1
#define HIT         2



/*
   Negative numbers are deflected lasers... 
*/

class KBBGraphic : public QWidget
{
  Q_OBJECT
public:
  KBBGraphic( KBBBoard* parent );
  ~KBBGraphic();

  friend class KBBGame;
  
  /**
   * @brief Define the size of the board game
   * 
   * @param w Number of columns of the black box
   * @param h Number of rows of the black box
   * The real size is 4 columns and rows bigger because of the space for "lasers" and an edge...
   */
  void setSize( int w, int h );

  void clear();
  void inputAt( int, int, int );
  void solve();

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
  void endMouseClicked();
  void addPlayerBall ( int );
  void removePlayerBall ( int );

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
  int traceRay( int startX, int startY, int *endX, int *endY );
  RectOnArray *graphicBoard;
  KBBBoard* m_board;
  int curRow;
  int curCol;
  bool inputAccepted;
  int minW;
  int minH;
  int cellW;
  int cellH;
  int numCols;
  int numRows;
  int detourCounter;
  QPixmap **pix;
  QPixmap **pixScaled;
  QPixmap *drawBuffer;
};

#endif // KBBGFX_H
