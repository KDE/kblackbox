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



#include <kmainwindow.h>
class KSelectAction;
class KToggleAction;


class KBBBoard;



/*
 * Statusbar IDs.
 */
#define SSCORE 0
#define SBALLS 1
#define SRUN   2
#define SSIZE  3



/**
* @brief Main window of the game
*/
class KBBGame : public KMainWindow
{
  Q_OBJECT
public:
  KBBGame();
  ~KBBGame();


public slots:
  void updateStats();


private slots:
  void slotSize(int selection);
  void slotBalls(int selection);

  void tutorialSwitch();

  /**
   * @brief Start a new game afer user confirmation (if needed).
   */
  void newGame();

  bool setSize( int w, int h );
  bool setBalls( int n );
  void gameResize();
  void setMinSize();
  void gameFinished();
  void abortGame();
  void giveUp();


private:
  /**
  * @brief As the user if he wants to end the game, if needed.
  * 
  * If the game is running and really started, the user has to confirm the end of the game.
  * If the game is not running or running but not really started, the game may end without user confirmation.
  */
  bool comfirmGameEndIfNeeded();
  
  /**
   * @brief Start a new game (without user comfirmation)
   * 
   * @see newGame()
   */
  void startGame();

  void initKAction();

  int balls;
  int m_columns;
  int m_rows;
  KBBBoard *m_board;
  bool running;
  bool tutorial;

  KSelectAction *ballsAction, *sizeAction;
  KToggleAction *tutorialAction;
};

#endif // KBBGAME_H
