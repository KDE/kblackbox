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



#ifndef KBBGFX_H
#define KBBGFX_H


#include <QWidget>


class KBBBallsOnBoard;
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

/**
 * @brief The classic central graphic widget
 */
class KBBGraphic : public QWidget
{
  Q_OBJECT
public:
  explicit KBBGraphic( KBBBoard* parent );
  ~KBBGraphic();

  friend class KBBGame;
  
  /**
   * @brief Create a new board game
   * 
   * @param w Number of columns of the black box
   * @param h Number of rows of the black box
   * The real size is 4 columns and rows bigger because of the space for "lasers" and an edge...
   */
  void newGame( int w, int h, KBBBallsOnBoard* balls, KBBBallsOnBoard* ballsPlaced);

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
  void slotUp();
  void slotDown();
  void slotLeft();
  void slotRight();
  void slotInput();

signals:
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
  int traceRay( int startX, int startY, int *endX, int *endY );
  void updateElement( int col, int row );
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
  KBBBallsOnBoard* m_balls;
  KBBBallsOnBoard* m_ballsPlaced;
};

#endif // KBBGFX_H
