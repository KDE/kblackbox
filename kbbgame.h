//
//
// KBlackBox
//
// A simple game inspired by an emacs module
// 
// File: kbbgame.h
//
// The definition of the KBBGame widget
// 

#ifndef KBBGAME_H
#define KBBGAME_H

#include "kbbgfx.h"
#include "util.h"

#include <qwidget.h>
#include <qmenubar.h> 
#include <qlabel.h>
#include <qpushbutton.h>
#include <ktmainwindow.h>

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
 * Menu & toolbar IDs.
 */
#define ID_HELP     0
#define ID_QUIT     3
#define ID_NEW      4
#define ID_GIVEUP   5
#define ID_RESIZE   6
#define ID_DONE     7

/*
 * Statusbar IDs.
 */
#define SSCORE 0
#define SBALLS 1
#define SRUN   2
#define SSIZE  3

class KBBGame : public KTMainWindow
{
  Q_OBJECT
public:
  KBBGame();
  ~KBBGame();

signals:
  void gameRuns( bool );

protected:
  void resizeEvent( QResizeEvent * );

private slots:
  void callBack( int );

  void size1();
  void size2();
  void size3();

  void balls1();
  void balls2();
  void balls3();

  void tutorialSwitch();

  void help();
  
  void newGame();

  bool setSize( int w, int h );
  bool setBalls( int n );
  void setMinSize();
  void randomBalls( int n );
  void gameResize();
  void setScore( int n );
  void updateStats();
  void gameFinished();
  void abortGame();
  void giveUp();
  void gotInputAt( int col, int row, int state );

private:
  int traceRay( int startX, int startY, int *endX, int *endY );
  void remap( RectOnArray *gam, RectOnArray *gra );
  void getResults();

  int balls;
  int detourCounter;
  int ballsPlaced;
  bool running;
  bool tutorial;
  RectOnArray *gameBoard;
  KBBGraphic *gr;

  KMenuBar *menu;
  QPopupMenu *sizesm, *ballsm, *options;
  int sizes1id, sizes2id, sizes3id;
  int balls1id, balls2id, balls3id;
  int tut1id;

  KToolBar *tool;
  KStatusBar *stat;
  int score;
  /*  QLabel *scoreText;
  QLabel *statusText;*/
};

#endif // KBBGAME_H
