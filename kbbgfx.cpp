//
//
// KBlackBox
//
// A simple game inspired by an emacs module
// 
// File: kbbgfx.cpp
//
// The implementation of the KBBGraphic widget
// 

#include <qpainter.h>
#include <qpixmap.h>
#include <qcolor.h> 
#include <qkeycode.h>
#include <qwmatrix.h>

#include "kbbgfx.h"
#include "util.h"

/*
   Constructs a KBBGraphic widget.
*/

KBBGraphic::KBBGraphic( QPixmap **p, QWidget* parent, const char* name )
    : QWidget( parent, name )
{
  int i;

  curRow = curCol = 0;
  setFocusPolicy( NoFocus );
  setBackgroundColor( gray );
  setCellWidth( CELLW );		// set width of cell in pixels
  setCellHeight( CELLH );		// set height of cell in pixels
  setMouseTracking( FALSE );
  
  pix = p;
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
      if (pix[i] != NULL) delete pix[i];
    }
    delete pix;
  }
  if (pixScaled != NULL) {
    for (i = 0; i < NROFTYPES; i++) {
      if (pixScaled[i] != NULL) delete pixScaled[i];
    }
    delete pixScaled;
  }
  if (graphicBoard != NULL) delete graphicBoard;
  if (drawBuffer != NULL) delete drawBuffer;
}

/*
   Sets the size of the table
*/

void KBBGraphic::setSize( int w, int h )
{
  if ((w != numCols) || (h != numRows)) {
    if (graphicBoard != NULL) delete graphicBoard;
    graphicBoard = new RectOnArray( w, h );
    graphicBoard->fill( OUTERBBG );
    setNumCols( w );
    setNumRows( h );
    setCellWidth( CELLW );
    setCellHeight( CELLH );
    minW = cellW * numRows;
    minH = cellH * numCols;
    emit(sizeChanged());
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
  QWMatrix wm;

  w0 = pix[0]->width();
  h0 = pix[0]->height();
  wm.scale( (float) w / (float) w0, (float) h / (float) h0 );
  for (i = 0; i < NROFTYPES; i++) {
    *pixScaled[i] = pix[i]->xForm( wm );
  }
}

/*
   Returns the sizes of the table
*/

int KBBGraphic::numC() { return numCols; }
int KBBGraphic::numR() { return numRows; }
int KBBGraphic::width() { return cellW * numRows; }
int KBBGraphic::height() { return cellH * numCols; }
int KBBGraphic::wHint() { return minW; }
int KBBGraphic::hHint() { return minH; }

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

  //  kdDebug() << p->viewport().width() << endl;

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
  //  kdDebug() << pm.width() << " " << w << endl;
  p->drawPixmap( 0, 0, pm );
  //  bitBlt( this, col * w, row * h, &pm );

  p->setPen( black );

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
    p->drawText( 1, 1, x2-1, y2-1, AlignCenter, s );
    break;
  case HLASERBBG:
    s.sprintf( "%c", 'H' );
    p->drawText( 1, 1, x2-1, y2-1, AlignCenter, s );
    break;
  }
  if (type < 0) {
    s.sprintf( "%d", -type );
    p->drawText( 1, 1, x2-1, y2-1, AlignCenter, s );
  }

  /*
     Draw extra frame inside if this is the current cell.
  */
  p->setPen( yellow );
  if ( (row == curRow) && (col == curCol) ) {	// if we are on current cell,
    if ( hasFocus() ) {
      p->drawRect( 0, 0, x2, y2 );
    }
    else {					// we don't have focus, so
      p->setPen( DotLine );		        // use dashed line to
      p->drawRect( 0, 0, x2, y2 );
      p->setPen( SolidLine );		        // restore to normal
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
  case MARK1BBG: color = darkRed; break;
  case OUTERBBG: color = white; break;  
  case INNERBBG: color = gray; break;
  case LASERBBG: color = darkGreen; break;
  case LFIREBBG: color = green; break;
  case FBALLBBG: color = red; break;
  case TBALLBBG: color = blue; break;
  case WBALLBBG: color = cyan; break;
  default: color = white;
  }
  p->fillRect( 0, 0, x2, y2, color );

  p->setPen( black );
  p->drawLine( x2, 0, x2, y2 );		// draw vertical line on right
  p->drawLine( 0, y2, x2, y2 );		// draw horiz. line at bottom
  
  /*
     Extra drawings for boxes aroud lasers. 
  */
  QString s;
  switch (type) {
  case RLASERBBG:
    s.sprintf( "%c", 'R' );
    p->drawText( 1, 1, x2-1, y2-1, AlignCenter, s );
    break;
  case HLASERBBG:
    s.sprintf( "%c", 'H' );
    p->drawText( 1, 1, x2-1, y2-1, AlignCenter, s );
    break;
  }
  if (type < 0) {
    s.sprintf( "%d", -type );
    p->drawText( 1, 1, x2-1, y2-1, AlignCenter, s );
  }

  /*
     Draw extra frame inside if this is the current cell.
  */
  if ( (row == curRow) && (col == curCol) ) {	// if we are on current cell,
    if ( hasFocus() ) {
      p->drawEllipse( 1, 1, x2-2, y2-2 );	// draw ellipse
    }
    else {					// we don't have focus, so
      p->setPen( DotLine );		        // use dashed line to
      p->drawEllipse( 1, 1, x2-2, y2-2 );	// draw ellipse
      p->setPen( SolidLine );		        // restore to normal
    }
  }
}

/*
   Xperimantal...
*/

void KBBGraphic::paintEvent( QPaintEvent* )
{
  int i, j;
  QPainter paint( drawBuffer );

  //  kdDebug() << drawBuffer->width() << endl;
  for (i = 0; i < numRows; i++) {
    for (j = 0; j < numCols; j++) {
      paint.setViewport( j * cellW, i * cellH, width(), height() );
      paintCell( &paint, i, j );
    }
  }
  bitBlt( this, 0, 0, drawBuffer );
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

  if (drawBuffer != NULL) delete drawBuffer;
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
    if (e->button() == MidButton) {
      emit endMouseClicked();
      return;
    }
    int oldRow = curRow;
    int oldCol = curCol;
    QPoint pos = e->pos();		// extract pointer position
    curRow = pos.y() / cellH;
    curCol = pos.x() / cellW;
    //kdDebug() << e->state() << " " << LeftButton << " " << e->state()&LeftButton << endl;
    updateElement( oldCol, oldRow );
    emit inputAt( curCol, curRow, e->button() ); 
  }
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
    // kdDebug() << movRow << " " << curRow << endl;
    if ( (curRow != movRow) 			// if current cell has moved,
	|| (curCol != movCol) ) {
      curRow = movRow;
      curCol = movCol;
      updateElement( oldCol, oldRow );
      emit inputAt( curCol, curRow, e->state() ); 
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
  emit inputAt( curCol, curRow, LeftButton ); 
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
  repaint( FALSE );
}    


/*
   Handles focus loss events for the KBBGraphic widget.
*/

void KBBGraphic::focusOutEvent( QFocusEvent* )
{
  repaint( FALSE );
}    

/*
   Sets whether user input is processed or not.
*/

void KBBGraphic::setInputAccepted( bool b )
{
  inputAccepted = b;
  if (b) setFocusPolicy( StrongFocus );
  else setFocusPolicy( NoFocus );
}    

/*
   Updates the cell at (col,row).
*/

void KBBGraphic::updateElement( int col, int row )
{
  QPainter paint( this );

  paint.setViewport( col * cellW, row * cellH, width(), height() );
  paintCell( &paint, row, col );
}

#include "kbbgfx.moc"
