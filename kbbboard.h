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



#ifndef KBBBOARD_H
#define KBBBOARD_H


#include <QObject>
class QWidget;


class KMainWindow;
#include <krandomsequence.h>


class KBBGraphic;
class RectOnArray;




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




/**
 * @brief This is the logical board of the game.
 *
 */
class KBBBoard : public QObject
{
	Q_OBJECT

	public:
		KBBBoard(KMainWindow *parent);
		~KBBBoard();
		
		void drawNewBoard(int balls, bool tutorial);
		
		/**
		* Game is running and player sent at least one laser ray.
		* Before that, user doesn't need to confirm the end of the game, if he tries to start a new game.
		*/
		bool gameReallyStarted();

		int getBallsPlaced();
		int getHeight();
		int getScore();
		QWidget* getWidget();
		int getWidgetHeight();
		int getWidgetWidth();
		int getWidth();
		void setSize( int w, int h );
		void solve();


	signals:
		void updateStats();


	public slots:
		void gameOver();
		void gameStarting();
		void gotInputAt( int col, int row, int state );


	private:
		void placeBalls( int n );
		void remap( RectOnArray *gam, RectOnArray *gra, bool tutorial );
		void setScore( int n );
		int traceRay( int startX, int startY, int *endX, int *endY );
		
		int ballsPlaced;
		int detourCounter;
		RectOnArray *gameBoard;
		KBBGraphic *gr;
		bool m_gameReallyStarted;
		KRandomSequence random;
		int score;

};

#endif //KBBBOARD_H
