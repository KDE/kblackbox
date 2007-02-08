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



#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QMouseEvent>
#include <QPaintEvent>


#include <kdebug.h>
#include <kicon.h>
#include <kiconloader.h>


#include "kbbgfx.h"
#include "kbbboard.h"
#include "util.h"



/*
  Names of pixmap files.
*/

const char *pFNames[NROFTYPES] = {
  "white",
  "gray",
  "green",
  "red",
  "blue",
  "cyan",
  "brown",
  "green2"
};


/*
   Constructs a KBBGraphic widget.
*/

KBBGraphic::KBBGraphic( KBBBoard* parent )
    : QWidget( )
{
  m_board = parent;

  int i;

  // Load icon files
  
//  QPixmap **pix = new QPixmap * [NROFTYPES];
  pix = new QPixmap * [NROFTYPES];
  pix[0] = new QPixmap();
  *pix[0] = BarIcon( pFNames[0] );
  if (!pix[0]->isNull()) {
    kDebug(12009) << "Pixmap \"" << pFNames[0] << "\" loaded." << endl;
    for (i = 1; i < NROFTYPES; i++) {
      pix[i] = new QPixmap;
      *pix[i] = BarIcon( pFNames[i] );
      if (!pix[i]->isNull()) {
	kDebug(12009) << "Pixmap \"" << pFNames[i] << "\" loaded." << endl;
      } else {
	pix[i] = pix[i-1];
	pix[i]->detach();
	kDebug(12009) << "Cannot find pixmap \"" << pFNames[i] << "\". Using previous one." << endl;
      }
    }
  } else {
    kDebug(12009) << "Cannot find pixmap \"" << pFNames[0] << "\". Pixmaps will not be loaded." << endl;
    delete pix[0];
    delete [] pix;
    pix = 0;
  }




  curRow = curCol = 0;
  setFocusPolicy( Qt::NoFocus );
  QPalette palette;
  palette.setColor( backgroundRole(), Qt::gray );
  setPalette( palette );
  setCellWidth( CELLW );		// set width of cell in pixels
  setCellHeight( CELLH );		// set height of cell in pixels
  setMouseTracking( false );
  
  if (pix == NULL) pixScaled = NULL;
  else {
    pixScaled = new QPixmap * [NROFTYPES];
    for (i = 0; i < NROFTYPES; i++) {
       pixScaled[i] = new QPixmap;
    }
  }
  graphicBoard = NULL;
  drawBuffer = NULL;
}

/*
   Destructor: deallocates memory for contents
*/

KBBGraphic::~KBBGraphic()
{
  int i;

  if (pix != NULL) {
    for (i = 0; i < NROFTYPES; i++) {
      delete pix[i];
    }
    delete pix;
  }
  if (pixScaled != NULL) {
    for (i = 0; i < NROFTYPES; i++) {
      delete pixScaled[i];
    }
    delete pixScaled;
  }
  delete graphicBoard;
  delete drawBuffer;
}

/*
   Sets the size of the table
*/

void KBBGraphic::newGame( int w, int h )
{
  // +4 is the space for "lasers" and an edge...
  w += 4;
  h += 4;
  
  if ((w != numCols) || (h != numRows)) {
    delete graphicBoard;
    graphicBoard = new RectOnArray( w, h );
    setNumCols( w );
    setNumRows( h );
    setCellWidth( CELLW );
    setCellHeight( CELLH );
    minW = cellW * numRows;
    minH = cellH * numCols;
  }


  detourCounter = -1;

  
  int i,j;

  graphicBoard->fill( INNERBBT );
  for (j = 0; j < (numR()); j++) {
    graphicBoard->set( 0, j, OUTERBBT );
    graphicBoard->set( numC()-1, j, OUTERBBT );
  }
  for (i = 0; i < (numC()); i++) {
    graphicBoard->set( i, 0, OUTERBBT );
    graphicBoard->set( i, numR()-1, OUTERBBT );
  }
  for (j = 2; j < (numR()-2); j++) {
    graphicBoard->set( 1, j, LASERBBT );
    graphicBoard->set( numC()-2, j, LASERBBT );
  }
  for (i = 2; i < (numC()-2); i++) {
    graphicBoard->set( i, 1, LASERBBT );
    graphicBoard->set( i, numR()-2, LASERBBT );
  }
  graphicBoard->set( 1, 1, OUTERBBT );
  graphicBoard->set( 1, numR()-2, OUTERBBT );
  graphicBoard->set( numC()-2, 1, OUTERBBT );
  graphicBoard->set( numC()-2, numR()-2, OUTERBBT );


  repaint();
  resizeEvent(0);
}


void KBBGraphic::solve()
{
  int tgra;
  int pos[DIM_MAX];

  for (pos[DIM_Y] = 0; pos[DIM_Y] < numR()-4; pos[DIM_Y]++) {
    for (pos[DIM_X] = 0; pos[DIM_X] < numC()-4; pos[DIM_X]++) {
      tgra = graphicBoard->get( pos[DIM_X]+2, pos[DIM_Y]+2 );
      if ((m_board->containsBall( pos[DIM_X] + pos[DIM_Y]*(numR()-4) )) && (tgra != TBALLBBG)) {
	graphicBoard->set( pos[DIM_X]+2, pos[DIM_Y]+2, WBALLBBG );
	updateElement( pos[DIM_X]+2, pos[DIM_Y]+2 );
      }
      if (!(m_board->containsBall( pos[DIM_X] + pos[DIM_Y]*(numR()-4) )) && (tgra == TBALLBBG)) {
	graphicBoard->set( pos[DIM_X]+2, pos[DIM_Y]+2, FBALLBBG );
	updateElement( pos[DIM_X]+2, pos[DIM_Y]+2 );
      }
    }
  }
}

void KBBGraphic::setCellWidth( int w )
{
  cellW = w;
}

void KBBGraphic::setCellHeight( int h )
{
  cellH = h;
}

void KBBGraphic::setNumRows( int rows )
{
  numRows = rows;
}

void KBBGraphic::setNumCols( int cols )
{
  numCols = cols;
}

/*
   Scales all pixmaps to desired size.
*/

void KBBGraphic::scalePixmaps( int w, int h )
{
  int i, w0, h0;
  QMatrix wm;

  w0 = pix[0]->width();
  h0 = pix[0]->height();
  wm.scale( (float) w / (float) w0, (float) h / (float) h0 );
  for (i = 0; i < NROFTYPES; i++) {
    *pixScaled[i] = pix[i]->transformed( wm );
  }
}

/*
   Returns the sizes of the table
*/

int KBBGraphic::numC() { return numCols; }
int KBBGraphic::numR() { return numRows; }
int KBBGraphic::width() { return cellW * numRows; }
int KBBGraphic::height() { return cellH * numCols; }
int KBBGraphic::wHint() const { return minW; }
int KBBGraphic::hHint() const { return minH; }
QSize KBBGraphic::sizeHint() const { return QSize(wHint(), hHint()); }

/*
   Returns a pointer to graphicBoard
*/

RectOnArray *KBBGraphic::getGraphicBoard() { return graphicBoard; }

/*
   Handles cell painting for the KBBGraphic widget.
*/

void KBBGraphic::paintCell( QPainter* p, int row, int col )
{
  if (pix == NULL) paintCellDefault( p, row, col );
  else paintCellPixmap( p, row, col );
}

void KBBGraphic::paintCellPixmap( QPainter* p, int row, int col )
{
  int w = cellW;
  int h = cellH;
  int x2 = w - 1;
  int y2 = h - 1;
  int type;
  QPixmap pm;

  //  kDebug(12009) << p->viewport().width() << endl;

  switch (type = graphicBoard->get( col, row )) {
  case MARK1BBG: pm = *pixScaled[MARK1BBG]; break;
  case OUTERBBG: pm = *pixScaled[OUTERBBG]; break;
  case INNERBBG: pm = *pixScaled[INNERBBG]; break;
  case LASERBBG: pm = *pixScaled[LASERBBG]; break;
  case LFIREBBG: pm = *pixScaled[LFIREBBG]; break;
  case FBALLBBG: pm = *pixScaled[FBALLBBG]; break;
  case TBALLBBG: pm = *pixScaled[TBALLBBG]; break;
  case WBALLBBG: pm = *pixScaled[WBALLBBG]; break;
  default: pm = *pixScaled[OUTERBBG];
  }
  //  kDebug(12009) << pm.width() << " " << w << endl;
  p->drawPixmap( 0, 0, pm );
  //  bitBlt( this, col * w, row * h, &pm );

  p->setPen( Qt::black );

  if (type == INNERBBG) {
    p->drawLine( x2, 0, x2, y2 );		// draw vertical line on right
    p->drawLine( 0, y2, x2, y2 );		// draw horiz. line at bottom
    p->drawLine( 0, 0, x2, 0 );
    p->drawLine( 0, 0, 0, y2 );
  }

  /*
     Extra drawings for boxes aroud lasers.
  */
  QString s;
  switch (type) {
  case RLASERBBG:
    s.sprintf( "%c", 'R' );
    p->drawText( 1, 1, x2-1, y2-1, Qt::AlignCenter, s );
    break;
  case HLASERBBG:
    s.sprintf( "%c", 'H' );
    p->drawText( 1, 1, x2-1, y2-1, Qt::AlignCenter, s );
    break;
  }
  if (type < 0) {
    s.sprintf( "%d", -type );
    p->drawText( 1, 1, x2-1, y2-1, Qt::AlignCenter, s );
  }

  /*
     Draw extra frame inside if this is the current cell.
  */
  p->setPen( Qt::yellow );
  if ( (row == curRow) && (col == curCol) ) {	// if we are on current cell,
    if ( hasFocus() ) {
      p->drawRect( 0, 0, x2, y2 );
    }
    else {					// we don't have focus, so
      p->setPen( Qt::DotLine );		        // use dashed line to
      p->drawRect( 0, 0, x2, y2 );
      p->setPen( Qt::SolidLine );		        // restore to normal
    }
  }
}

void KBBGraphic::paintCellDefault( QPainter* p, int row, int col )
{
  int w = cellW;
  int h = cellH;
  int x2 = w - 1;
  int y2 = h - 1;
  int type;
  QColor color;

  switch (type = graphicBoard->get( col, row )) {
  case MARK1BBG: color = Qt::darkRed; break;
  case OUTERBBG: color = Qt::white; break;
  case INNERBBG: color = Qt::gray; break;
  case LASERBBG: color = Qt::darkGreen; break;
  case LFIREBBG: color = Qt::green; break;
  case FBALLBBG: color = Qt::red; break;
  case TBALLBBG: color = Qt::blue; break;
  case WBALLBBG: color = Qt::cyan; break;
  default: color = Qt::white;
  }
  p->fillRect( 0, 0, x2, y2, color );

  p->setPen( Qt::black );
  p->drawLine( x2, 0, x2, y2 );		// draw vertical line on right
  p->drawLine( 0, y2, x2, y2 );		// draw horiz. line at bottom

  /*
     Extra drawings for boxes aroud lasers.
  */
  QString s;
  switch (type) {
  case RLASERBBG:
    s.sprintf( "%c", 'R' );
    p->drawText( 1, 1, x2-1, y2-1, Qt::AlignCenter, s );
    break;
  case HLASERBBG:
    s.sprintf( "%c", 'H' );
    p->drawText( 1, 1, x2-1, y2-1, Qt::AlignCenter, s );
    break;
  }
  if (type < 0) {
    s.sprintf( "%d", -type );
    p->drawText( 1, 1, x2-1, y2-1, Qt::AlignCenter, s );
  }

  /*
     Draw extra frame inside if this is the current cell.
  */
  if ( (row == curRow) && (col == curCol) ) {	// if we are on current cell,
    if ( hasFocus() ) {
      p->drawEllipse( 1, 1, x2-2, y2-2 );	// draw ellipse
    }
    else {					// we don't have focus, so
      p->setPen( Qt::DotLine );		        // use dashed line to
      p->drawEllipse( 1, 1, x2-2, y2-2 );	// draw ellipse
      p->setPen( Qt::SolidLine );		        // restore to normal
    }
  }
}

/*
   Xperimantal...
*/

void KBBGraphic::paintEvent( QPaintEvent* p)
{
  QRect r = p->rect();
  int fromCol = r.left() / cellW;
  int toCol = r.right() / cellW;
  int fromRow = r.top() / cellH;
  int toRow = r.bottom() /cellH;

  QPainter paint( drawBuffer );
  //  kDebug(12009) << drawBuffer->width() << endl;
  for (int i = fromRow; i <= toRow; i++) {
    for (int j = fromCol; j <= toCol; j++) {
      paint.setViewport( j * cellW, i * cellH, width(), height() );
      paintCell( &paint, i, j );
    }
  }
  paint.end();
  paint.begin(this);
  paint.drawPixmap(0,0, *drawBuffer);
  paint.end();
}

/*
   Resize event of the KBBGraphic widget.
*/

void KBBGraphic::resizeEvent( QResizeEvent*  )
{
  int w = QWidget::width();
  int h = QWidget::height();
  int wNew, hNew;

  //  kbDebug() << w << " " << h << " " << minW << " " << minH << endl;
  if (w > minW) {
    wNew = w / numC();
  } else {
    wNew = CELLW;
  }
  if (h > minH) {
    hNew = h / numR();
  } else {
    hNew = CELLH;
  }
  if (pix != NULL) scalePixmaps( wNew, hNew );
  setCellWidth( wNew );
  setCellHeight( hNew );

  delete drawBuffer;
  drawBuffer = new QPixmap( cellW * numRows, cellH * numCols );
}

/*
   Handles mouse press events for the KBBGraphic widget.
*/
void KBBGraphic::mousePressEvent( QMouseEvent* e )
{
  if (inputAccepted) {
    /*
     * Middle click finishes the game.
     */
    if (e->button() == Qt::MidButton) {
      emit endMouseClicked();
      return;
    }
    int oldRow = curRow;
    int oldCol = curCol;
    QPoint pos = e->pos();		// extract pointer position
    curRow = pos.y() / cellH;
    curCol = pos.x() / cellW;
    //kDebug(12009) << e->state() << " " << Qt::LeftButton << " " << e->state()&LeftButton << endl;
    updateElement( oldCol, oldRow );
    inputAt( curCol, curRow, e->button() );
  }
}


void KBBGraphic::inputAt( int col, int row, int state )
{
  RectOnArray *r = getGraphicBoard();
  int type = r->get( col, row );
  int x, y;
  int ex, ey;
  int w = graphicBoard->width() - 2;
  int h = graphicBoard->height() - 2;

  if (state & Qt::LeftButton) {
    switch (type) {
    case WBALLBBG: // because of the tutorial mode
    case INNERBBG:
      r->set( col, row, TBALLBBG );
      emit addPlayerBall( (col - 2) + (row - 2) * w  );
      break;
    case MARK1BBG:
      r->set( col, row, INNERBBG );
      break;
    case TBALLBBG:
      r->set( col, row, INNERBBG );
      emit removePlayerBall( (col - 2) + (row - 2) * w  );
      break;
    case LASERBBG:
      int endX, endY, result;
      result = traceRay( col, row, &endX, &endY );
      r->set( col, row, LFIREBBG );
      //kDebug << endX << " " << endY << endl;
      if (col == 1) x = 0; else
      if (col == w) x = w + 1;
      else x = col;

      if (row == 1) y = 0; else
      if (row == h) y = h + 1;
      else y = row;

      switch (result) {
      case DETOUR:
	r->set( endX, endY, LFIREBBG );
	r->set( x, y, detourCounter );
	if (endX == 1) ex = 0; else
	  if (endX == w) ex = w + 1;
	  else ex = endX;
	if (endY == 1) ey = 0; else
	  if (endY == h) ey = h + 1;
	  else ey = endY;
	r->set( ex, ey, detourCounter-- );
	updateElement( x, y );
	updateElement( ex, ey );
	updateElement( endX, endY );
	break;
      case REFLECTION:
	r->set( x, y, RLASERBBG );
	updateElement( x, y );
	break;
      case HIT:
	r->set( x, y, HLASERBBG );
	updateElement( x, y );
	break;
      case WRONGSTART:
	kDebug(12009) << "Wrong start?! It should't happen!!" << endl;
	break;
      }
      break;
    }
  } else if (state & Qt::RightButton) {
    switch (type) {
    case INNERBBG:
      r->set( col, row, MARK1BBG );
      break;
    }
  }
  updateElement( col, row );
}


int KBBGraphic::traceRay( int startX, int startY, int *endX, int *endY )
{
	int startPos[DIM_MAX];
	startPos[DIM_X] = startX;
	startPos[DIM_Y] = startY;

	int startBorderPos = m_board->absolutePositionToBorderPosition(startPos);
	int endBorderPos = m_board->shootRay(startBorderPos);

	if (endBorderPos == m_board->HIT_POSITION)
		return HIT;

	if (startBorderPos == endBorderPos)
		return REFLECTION;

	int endPos[DIM_MAX];
	m_board->borderPositionToAbsolutePosition(endBorderPos, endPos);
	*endX = endPos[DIM_X];
	*endY = endPos[DIM_Y];

	return DETOUR;
}


/*
   Handles mouse move events for the KBBGraphic widget.
*/

void KBBGraphic::mouseMoveEvent( QMouseEvent* e ) {
  if (inputAccepted) {
    int oldRow = curRow;
    int oldCol = curCol;
    QPoint pos = e->pos();                 // extract pointer position
    int movRow = pos.y() / cellH;
    int movCol = pos.x() / cellW;
    // kDebug(12009) << movRow << " " << curRow << endl;
    if ( (curRow != movRow) 			// if current cell has moved,
	|| (curCol != movCol) ) {
      curRow = movRow;
      curCol = movCol;
      updateElement( oldCol, oldRow );
      inputAt( curCol, curRow, e->buttons() );
    }
  }
}

void KBBGraphic::slotUp()
{
  if( curRow > 0 ) {
    moveSelection( -1, 0 );
  }
}

void KBBGraphic::slotDown()
{
  if( curRow < numRows-1 ) {
    moveSelection( 1, 0 );
  }
}

void KBBGraphic::slotLeft()
{
  if( curCol > 0 ) {
    moveSelection( 0, -1 );
  }
}

void KBBGraphic::slotRight()
{
  if( curCol < numCols-1 ) {
    moveSelection( 0, 1 );
  }
}

void KBBGraphic::slotInput()
{
  if ( !inputAccepted ) {
    return;
  }
  inputAt( curCol, curRow, Qt::LeftButton );
//  updateElement( curCol, curRow );
}

void KBBGraphic::moveSelection(int drow, int dcol)
{
  if ( !dcol && !drow || !inputAccepted ) {
    return;
  }
  curCol += dcol;
  curRow += drow;
  updateElement( curCol - dcol, curRow - drow );
  updateElement( curCol, curRow );
}

/*
   Handles focus reception events for the KBBGraphic widget.
*/

void KBBGraphic::focusInEvent( QFocusEvent* )
{
  repaint();
}


/*
   Handles focus loss events for the KBBGraphic widget.
*/

void KBBGraphic::focusOutEvent( QFocusEvent* )
{
  repaint();
}

/*
   Sets whether user input is processed or not.
*/

void KBBGraphic::setInputAccepted( bool b )
{
  inputAccepted = b;
  if (b) {
    setFocusPolicy( Qt::StrongFocus );
    setFocus();
  } else {
    setFocusPolicy( Qt::NoFocus );
    clearFocus();
  }
}

void KBBGraphic::updateElement( int col, int row )
{
  repaint( col*cellW, row*cellH, cellW, cellH );
}
#include "kbbgfx.moc"
