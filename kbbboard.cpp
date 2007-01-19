//
// KBlackbox
//
// A simple game inspired by an emacs module
//
/***************************************************************************
 *   Copyright (c) 1999-2000, Robert Cimrman                               *
 *   cimrman3@students.zcu.cz                                              *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <QPixmap>
#include <QWidget>


#include <kdebug.h>
#include <kicon.h>
#include <kiconloader.h>
#include <kmainwindow.h>
#include <krandomsequence.h>


#include "kbbboard.h"
#include "kbbgfx.h"
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



//
// Constructor / Destructor
//

KBBBoard::KBBBoard(KMainWindow *parent)
{
  int i;
  gameBoard = 0;
  QPixmap **pix = new QPixmap * [NROFTYPES];
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


	gr = new KBBGraphic( pix, parent );
	gr->setObjectName("KBBGraphic");

	connect( gr, SIGNAL(inputAt(int,int,int)), this, SLOT(gotInputAt(int,int,int)) );

	connect( gr, SIGNAL(endMouseClicked()), parent, SLOT(gameFinished()) );
}


KBBBoard::~KBBBoard()
{
	if (gameBoard) delete gameBoard;
	// All the rest has "this" for parent so it doesn't need to be deleted.
}



//
// Public
//

void KBBBoard::drawNewBoard(int balls, bool tutorial)
{
  int i,j;

  gameBoard->fill( INNERBBT );
  for (j = 0; j < (gr->numR()); j++) {
    gameBoard->set( 0, j, OUTERBBT );
    gameBoard->set( gr->numC()-1, j, OUTERBBT );
  }
  for (i = 0; i < (gr->numC()); i++) {
    gameBoard->set( i, 0, OUTERBBT );
    gameBoard->set( i, gr->numR()-1, OUTERBBT );
  }
  for (j = 2; j < (gr->numR()-2); j++) {
    gameBoard->set( 1, j, LASERBBT );
    gameBoard->set( gr->numC()-2, j, LASERBBT );
  }
  for (i = 2; i < (gr->numC()-2); i++) {
    gameBoard->set( i, 1, LASERBBT );
    gameBoard->set( i, gr->numR()-2, LASERBBT );
  }
  gameBoard->set( 1, 1, OUTERBBT );
  gameBoard->set( 1, gr->numR()-2, OUTERBBT );
  gameBoard->set( gr->numC()-2, 1, OUTERBBT );
  gameBoard->set( gr->numC()-2, gr->numR()-2, OUTERBBT );

  placeBalls(balls);

  remap( gameBoard, gr->getGraphicBoard(), tutorial );
  gr->repaint();
}


bool KBBBoard::gameReallyStarted()
{
	return m_gameReallyStarted;
}


int KBBBoard::getBallsPlaced()
{
	return ballsPlaced;
}


int KBBBoard::getHeight()
{
	return gr->numR() - 4;
}


int KBBBoard::getScore()
{
	return score;
}


QWidget* KBBBoard::getWidget()
{
	return gr;
}


int KBBBoard::getWidgetHeight()
{
	return gr->hHint();
}


int KBBBoard::getWidgetWidth()
{
	return gr->wHint();
}


int KBBBoard::getWidth()
{
	return gr->numC() - 4;
}


void KBBBoard::setSize( int w, int h )
{
	gr->setSize(w + 4, h + 4); // +4 is the space for "lasers" and an edge...
	if (gameBoard) delete gameBoard;
	gameBoard = new RectOnArray( gr->numC(), gr->numR() );
}


/*
   Computes the final score and indicate errors.
*/
void KBBBoard::solve()
{
  int i, j, tgam, tgra;
  RectOnArray *r = gr->getGraphicBoard();
  for (j = 0; j < (gr->numR()); j++) {
    for (i = 0; i < (gr->numC()); i++) {
      tgam = gameBoard->get( i, j );
      tgra = r->get( i, j );
      if ((tgam == BALLBBT) && (tgra != TBALLBBG)) {
	r->set( i, j, WBALLBBG );
	gr->updateElement( i, j );
	setScore( score + 5);
      }
      if ((tgam != BALLBBT) && (tgra == TBALLBBG)) {
	r->set( i, j, FBALLBBG );
	gr->updateElement( i, j );
      }
    }
  }
}



//
// Slots
//

void KBBBoard::gameOver()
{
	ballsPlaced = 0;
	m_gameReallyStarted = false;
	gr->clearFocus();
	gr->setInputAccepted(false);
}

void KBBBoard::gameStarting()
{
	ballsPlaced = 0;
	detourCounter = -1;
	setScore( 0 );
	m_gameReallyStarted = false;
	gr->setInputAccepted(true);
	gr->setFocus();
}


/*
   Processes the user input.
*/
void KBBBoard::gotInputAt( int col, int row, int state )
{
  RectOnArray *r = gr->getGraphicBoard();
  int type = r->get( col, row );
  int x, y;
  int ex, ey;
  int w = gameBoard->width() - 2;
  int h = gameBoard->height() - 2;

  if (state & Qt::LeftButton) {
    switch (type) {
    case WBALLBBG: // because of the tutorial mode
    case INNERBBG:
      r->set( col, row, TBALLBBG );
      ballsPlaced++;
      break;
    case MARK1BBG:
      r->set( col, row, INNERBBG );
      break;
    case TBALLBBG:
      r->set( col, row, INNERBBG );
      ballsPlaced--;
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
	gr->updateElement( x, y );
	gr->updateElement( ex, ey );
	gr->updateElement( endX, endY );
	setScore( score+2 );
	break;
      case REFLECTION:
	r->set( x, y, RLASERBBG );
	gr->updateElement( x, y );
	setScore( score+1 );
	break;
      case HIT:
	r->set( x, y, HLASERBBG );
	gr->updateElement( x, y );
	setScore( score+1 );
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
      /*case MARK1BBG:
	r->set( col, row, INNERBBG );
      break;*/
    }
  }
  gr->updateElement( col, row );

  emit updateStats();
}


//
// Private
//

/*
   Puts n balls in the black box on random positions.
*/
void KBBBoard::placeBalls( int n )
{
  int i;
  random.setSeed(0);
  for (i = 0; i < n; i++) {
    int x=0, y=0; // there is OUTERBBT...
    while (gameBoard->get( x, y ) != INNERBBT ) {
      x =  2 + random.getLong(gameBoard->width()-4);
      y =  2 + random.getLong(gameBoard->height()-4);
    }
    gameBoard->set( x, y, BALLBBT );
  }
}


/*
   Remaps the gameBoard to its graphic representation.
*/
void KBBBoard::remap( RectOnArray *gam, RectOnArray *gra, bool tutorial )
{
  int i, j;
  for (j = 0; j < (gam->height()); j++) {
    for (i = 0; i < (gam->width()); i++) {
      switch (gam->get( i,j )) {
      case BALLBBT: if (tutorial) { gra->set( i,j, WBALLBBG ); break; }
      case INNERBBT: gra->set( i,j, INNERBBG ); break;
      case OUTERBBT: gra->set( i,j, OUTERBBG ); break;
      case LASERBBT: gra->set( i,j, LASERBBG ); break;
      default: gra->set( i,j, OUTERBBG );
      }
    }
  }
}


/*
   Sets the score value to n.
*/
void KBBBoard::setScore( int n )
{
	score = n;
	emit updateStats();
}


/*
   This is, in fact, the whole game...
*/
int KBBBoard::traceRay( int startX, int startY, int *endX, int *endY )
{
  int type, x, y, d, refl;
  int slx, scx, srx, sly, scy, sry;
  bool directionChanged;
  *endX = x = startX;
  *endY = y = startY;
  /*
    Just to avoid compiler warnings
  */
  type = slx = scx = srx = sly = scy = sry = 0;
  /*
     Get the initial direction d.
     0 .. up, 1 .. right, 2 .. down, 3 .. left
     (0,0) is the upper-left corner.
  */
  if ((gameBoard->get( x, y-1 ) == INNERBBT) ||
      (gameBoard->get( x, y-1 ) == BALLBBT)) { d = 0; }
  else if ((gameBoard->get( x+1, y ) == INNERBBT) ||
	   (gameBoard->get( x+1, y ) == BALLBBT)) { d = 1; }
  else if ((gameBoard->get( x, y+1 ) == INNERBBT) ||
	   (gameBoard->get( x, y+1 ) == BALLBBT)) { d = 2; }
  else if ((gameBoard->get( x-1, y ) == INNERBBT) ||
	   (gameBoard->get( x-1, y ) == BALLBBT)) { d = 3; }
  else return WRONGSTART;
  /*
     And now trace the ray.
  */
  while (1) {
    switch (d) {
    case 0:
      slx = -1; scx = 0; srx = 1;
      sly = -1; scy = -1; sry = -1;
      break;
    case 1:
      slx = 1; scx = 1; srx = 1;
      sly = -1; scy = 0; sry = 1;
      break;
    case 2:
      slx = 1; scx = 0; srx = -1;
      sly = 1; scy = 1; sry = 1;
      break;
    case 3:
      slx = -1; scx = -1; srx = -1;
      sly = 1; scy = 0; sry = -1;
      break;
    }
    directionChanged = false;
    if (gameBoard->get( x+scx, y+scy ) == LASERBBT) {
      type = DETOUR;
      *endX = x+scx;
      *endY = y+scy;
      break;
    }
    if (gameBoard->get( x+scx, y+scy ) == BALLBBT) {
      type = HIT;
      break;
    }
    refl = 0;
    if (gameBoard->get( x+slx, y+sly ) == BALLBBT) {
      type = REFLECTION;
      if (gameBoard->get( x, y ) == LASERBBT) break;
      directionChanged = true;
      refl += 1;
    }
    if (gameBoard->get( x+srx, y+sry ) == BALLBBT) {
      type = REFLECTION;
      if (gameBoard->get( x, y ) == LASERBBT) break;
      directionChanged = true;
      refl +=2;
    }
    // turn to the right
    if (refl == 1) d = (d + 1) % 4;
    // turn to the left
    if (refl == 2) if ((d -= 1) < 0) d += 4;
    // turn back -- no need to trace again the same way
    if (refl == 3) break;
    if (!directionChanged) {
      x += scx;
      y += scy;
    }
  }

  m_gameReallyStarted = true;
  return type;
}


#include "kbbboard.moc"
