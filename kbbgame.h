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

#include <kmainwindow.h>
#include <krandomsequence.h>

class KSelectAction;
class KToggleAction;

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
 * Statusbar IDs.
 */
#define SSCORE 0
#define SBALLS 1
#define SRUN   2
#define SSIZE  3

class KBBGame : public KMainWindow
{
  Q_OBJECT
public:
  KBBGame();
  ~KBBGame();

signals:
  void gameRuns( bool );

private slots:
  void slotSize();
  void slotBalls();

  void tutorialSwitch();

  void newGame();

  bool setSize( int w, int h );
  bool setBalls( int n );
  void setMinSize();
  void randomBalls( int n );
  void slotResize();
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
  void initKAction();

  int balls;
  int detourCounter;
  int ballsPlaced;
  bool running;
  bool tutorial;
  RectOnArray *gameBoard;
  KBBGraphic *gr;

  int score;
  /*  QLabel *scoreText;
  QLabel *statusText;*/
  KRandomSequence random;

  KSelectAction *ballsAction, *sizeAction;
  KToggleAction *tutorialAction;
};

#endif // KBBGAME_H
