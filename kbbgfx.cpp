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

KBBGraphic::KBBGraphic( int w, int h, QPixmap **p,
			QWidget *parent, const char *name )
    : QTableView(parent,name)
{
  int i;

  curRow = curCol = 0;			// set currently selected cell
  setFocusPolicy( NoFocus );
  setBackgroundColor( gray );		// set widgets background white
  setNumCols( w );			// set number of col's in table
  setNumRows( h );			// set number of rows in table
  setCellWidth( CELLW );		// set width of cell in pixels
  setCellHeight( CELLH );		// set height of cell in pixels
  setTableFlags( Tbl_autoScrollBars |	// always vertical scroll bar
		Tbl_clipCellPainting |	// avoid drawing outside cell
		Tbl_smoothScrolling);	// easier to see the scrolling
  setMouseTracking( FALSE );
  
  pix = p;
  if (pix == NULL) pixScaled = NULL;
  else {
    pixScaled = new (QPixmap *) [NROFTYPES];
    for (i = 0; i < NROFTYPES; i++) {
       pixScaled[i] = new QPixmap;
    }
  }
  graphicBoard = new RectOnArray( w, h);
}

KBBGraphic::KBBGraphic( QPixmap **p, QWidget* parent, const char* name )
    : QTableView(parent,name)
{
  int i;

  curRow = curCol = 0;			// set currently selected cell
  setFocusPolicy( NoFocus );
  setBackgroundColor( gray );		// set widgets background gray
  setNumCols( 0 );			// number of col's in table
  setNumRows( 0 );			// set number of rows in table
  setCellWidth( CELLW );		// set width of cell in pixels
  setCellHeight( CELLH );		// set height of cell in pixels
  setTableFlags( Tbl_autoScrollBars |	// scrollbars when needed...
		Tbl_clipCellPainting |	// avoid drawing outside cell
		Tbl_smoothScrolling);	// easier to see the scrolling
  setMouseTracking( FALSE );
  
  pix = p;
  if (pix == NULL) pixScaled = NULL;
  else {
    pixScaled = new (QPixmap *) [NROFTYPES];
    for (i = 0; i < NROFTYPES; i++) {
       pixScaled[i] = new QPixmap;
    }
  }
  graphicBoard = NULL;
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
  delete graphicBoard;			// deallocation
}

/*
   Sets the size of the table
*/

void KBBGraphic::setSize( int w, int h )
{
  if ((w != numCols()) || (h != numRows())) {
    if (graphicBoard != NULL) delete graphicBoard;
    graphicBoard = new RectOnArray( w, h );
    graphicBoard->fill( OUTERBBG );
    setAutoUpdate( FALSE );
    setNumCols( w );	       	        // set number of cols in table
    setNumRows( h );	       	        // set number of rows in table
    setCellWidth( CELLW );		// set width of cell in pixels
    setCellHeight( CELLH );		// set height of cell in pixels
    minW = totalWidth();
    minH = totalHeight();
    emit(sizeChanged());
    setAutoUpdate( TRUE );
  }
}

/*
   Scales all pixmaps to desired size.
*/

void KBBGraphic::scalePixmaps( int w, int h )
{
  int i;
  QWMatrix wm;

  wm.scale( (float) w / (float) 48, (float) h / (float) 48 );
  for (i = 0; i < NROFTYPES; i++) {
    *pixScaled[i] = pix[i]->xForm( wm );
  }
}

/*
   Returns the sizes of the table
*/

int KBBGraphic::numC() { return numCols(); }
int KBBGraphic::numR() { return numRows(); }
int KBBGraphic::width() { return totalWidth(); }
int KBBGraphic::height() { return totalHeight(); }
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
  int w = cellWidth( col );	        // width of cell in pixels
  int h = cellHeight( row );		// height of cell in pixels
  int x2 = w - 1;
  int y2 = h - 1;
  int type;
  QPixmap pm;

  switch(type = graphicBoard->get( col, row )) {
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
  p->drawPixmap( 0, 0, pm );
  //  bitBlt( this, col * w, row * h, pm );

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
      p->drawEllipse( 4, 4, x2-6, y2-6 );	// draw ellipse
    }
    else {					// we don't have focus, so
      p->setPen( DotLine );		        // use dashed line to
      p->drawEllipse( 4, 4, x2-6, y2-6 );	// draw ellipse
      p->setPen( SolidLine );		        // restore to normal
    }
  }
}

void KBBGraphic::paintCellDefault( QPainter* p, int row, int col )
{
  int w = cellWidth( col );	        // width of cell in pixels
  int h = cellHeight( row );		// height of cell in pixels
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

void KBBGraphic::paintEvent( QPaintEvent* e )
{
  QTableView::paintEvent( e );
}

/*
   Resize event of the KBBGraphic widget.
*/

void KBBGraphic::resizeEvent( QResizeEvent* e )
{
  int w = QWidget::width();
  int h = QWidget::height();
  int wNew, hNew;

  //  debug("%d %d %d %d", w, h, minW, minH );
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
  setAutoUpdate( FALSE );
  setCellWidth( wNew );
  setCellHeight( hNew );
  setAutoUpdate( TRUE );
  QTableView::resizeEvent( e );
}

/*
   Handles mouse press events for the KBBGraphic widget.
   The current cell marker is set to the cell the mouse is clicked in.
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

    int oldRow = curRow;			// store previous current cell
    int oldCol = curCol;
    QPoint clickedPos = e->pos();		// extract pointer position
    curRow = findRow( clickedPos.y() );		// map to row; set current cell
    curCol = findCol( clickedPos.x() );		// map to col; set current cell
    if ( (curRow != oldRow) 			// if current cell has moved,
	|| (curCol != oldCol) ) {
      updateCell( oldRow, oldCol );		// erase previous marking
      updateCell( curRow, curCol );		// show new current cell
    }
    //debug("%d %d %d ", e->state(), LeftButton, e->state()&LeftButton);
    emit inputAt( curCol, curRow, e->button() ); 
  }
}


/*
   Handles mouse move events for the KBBGraphic widget.
   The current cell marker is set to the cell the mouse is moved in.
   (Mouse tracking is disabled - move events iff a button is pressed...)
*/

void KBBGraphic::mouseMoveEvent( QMouseEvent* e ) {
  if (inputAccepted) {
    int oldRow = curRow;			// store previous current cell
    int oldCol = curCol;
    QPoint movedPos = e->pos();                 // extract pointer position
    int movRow = findRow( movedPos.y() );      	// map to row; set current cell
    int movCol = findCol( movedPos.x() );      	// map to col; set current cell
    // debug("%d %d", movRow,curRow);
    if ( (curRow != movRow) 			// if current cell has moved,
	|| (curCol != movCol) ) {
      curRow = movRow;
      curCol = movCol;
      updateCell( oldRow, oldCol );		// erase previous marking
      updateCell( curRow, curCol );		// show new current cell
      emit inputAt( curCol, curRow, e->state() ); 
    }
  }
}

/*
   Handles key press events for the KBBGraphic widget.
*/

void KBBGraphic::keyPressEvent( QKeyEvent* e )
{
  if (inputAccepted) {
    int oldRow = curRow;	       	// store previous current cell
    int oldCol = curCol;
    int edge = 0;
    switch( e->key() ) {	       	// Look at the key code
    case Key_Left:		       	// If 'left arrow'-key, 
      if( curCol > 0 ) {	       	// and cr't not in leftmost col
	curCol--;     			// set cr't to next left column
	edge = leftCell();		// find left edge
	if ( curCol < edge )		// if we have moved off  edge,
	  setLeftCell( edge - 1 );	// scroll view to rectify
      }
      break;
    case Key_Right:		       	// Correspondingly...
      if( curCol < numCols()-1 ) {
	curCol++;
	edge = lastColVisible();
	if ( curCol >= edge )
	  setLeftCell( leftCell() + 1 );
      }
      break;
    case Key_Up:
      if( curRow > 0 ) {
	curRow--;
	edge = topCell();
	if ( curRow < edge )
	  setTopCell( edge - 1 );
      }
      break;
    case Key_Down:
      if( curRow < numRows()-1 ) {
	curRow++;
	edge = lastRowVisible();
	if ( curRow >= edge )
	  setTopCell( topCell() + 1 );
      }
      break;
    case Key_Return:
    case Key_Enter:
      emit inputAt( curCol, curRow, LeftButton ); 
      break;
    default:				// If not an interesting key,
      e->ignore();			// we don't accept the event
      return;	
    }
    
    if ( (curRow != oldRow) 	       	// if current cell has moved,
	|| (curCol != oldCol)  ) {
      updateCell( oldRow, oldCol );    	// erase previous marking
      updateCell( curRow, curCol );    	// show new current cell
    }
  }
}

/*
   Handles focus reception events for the KBBGraphic widget.
   Repaint only the current cell; to avoid flickering
*/

void KBBGraphic::focusInEvent( QFocusEvent* )
{
  updateCell( curRow, curCol );		// draw current cell
}    


/*
   Handles focus loss events for the KBBGraphic widget.
   Repaint only the current cell; to avoid flickering
*/

void KBBGraphic::focusOutEvent( QFocusEvent* )
{
  updateCell( curRow, curCol );		// draw current cell
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
  updateCell( row, col, TRUE );
}

/*
   Sets the top left cell to the cell at (col, row). 
*/

void KBBGraphic::setTopLeft( int col, int row ) {
  setTopLeftCell( row, col );
}
