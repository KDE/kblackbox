//
// KBlackBox
//
// A simple game inspired by an emacs module
//
/***************************************************************************
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

#ifndef KBBTUTORIAL_H
#define KBBTUTORIAL_H



#include <QGroupBox>
class QHideEvent;
class QLabel;
class QProgressBar;
class QWidget;


class QPushButton;
class KTextEdit;


class KBBGraphicsItemTutorialMarker;
class KBBScalableGraphicWidget;



/**
 * @brief Tutorial widget
 */
class KBBTutorial : public QGroupBox
{
	Q_OBJECT

	public:
		/**
		 * @brief Number of balls in the tutorial
		 */
		static int const BALLS = 3;

		/**
		 * @brief Number of columns in the tutorial
		 */
		static int const COLUMNS = 6;

		/**
		 * @brief Number of rows in the tutorial
		 */
		static int const ROWS = 6;


		/**
		 * @brief Constructor
		 */
		explicit KBBTutorial(QWidget* parent);


		/**
		 * @brief Event: Leaving the tutorial
		 *
		 * Called by exiting the tutorial
		 */
		void hideEvent(QHideEvent*) override;

		/**
		 * @brief May the player solve the running game?
		 *
		 * The player may solve the game only at the end of the tutorial during the last step.
		 */
		bool maySolve();

		/**
		 * @brief May the player shoot the ray?
		 *
		 * The player may only shoot given rays during the tutorial (except during the last step).
		 */
		bool mayShootRay(const int incomingPosition);

		/**
		 * @brief Define the scalable graphic widget
		 */
		void setGameWidget(KBBScalableGraphicWidget* gameWidget, KBBGraphicsItemTutorialMarker* marker);

		/**
		 * @brief Go to the given tutorial step
		 */
		void setStep(const int step);

		/**
		 * @brief Start the tutorial
		 * Start or restart the tutorial at the 1st step.
		 */
		void start();


	private Q_SLOTS:
		/**
		 * @brief Go to the next tutorial step
		 */
		void nextStep();

		/**
		 * @brief Go to the previous tutorial step
		 */
		void previousStep();

		/**
		 * @brief Restore the default style of the label m_playerAction
		 *
		 * This slot is called by the timer.
		 */
		void restoreStyle();


	private:
		/**
		 * @brief First tutorial step
		 */
		static int const FIRST_STEP = 1;

		/**
		 * @brief Time to highlight the label m_playerAction
		 *
		 * If the player clicks on a disabled laser, the label m_playerAction is highlighted during this time (in ms).
		 */
		static int const HIGHLIGHT_TIME = 600;

		/**
		 * @brief Last tutorial step
		 */
		static int const LAST_STEP = 11;

		/**
		 * @brief Position of laser, if laser may not be used
		 */
		static int const MAY_NOT_USE = -1;

		/**
		 * @brief Fixed width and minimum height
		 *
		 * This is the fixed width and the minimum height of the widget. The widget needs a minimal size: it is ugly if it is too small.
		 */
		static int const WIDTH = 275;


		/**
		 * @brief define the maximal step
		 *
		 * The player may just navigate between the steps he already read.
		 */
		void setNewStepMaxAllowed(const int newStepMax);

		/**
		 * @brief Set the title and the main text
		 */
		void setTexts(const QString &title, const QString &text, const QString &action);

		/**
		 * @brief Show tutorial marker on the board
		 */
		void showMarker(const int laserPosition) const;


		QPushButton* m_buttonNext;
		QPushButton* m_buttonPrevious;
		KTextEdit* m_explanation;
		KBBScalableGraphicWidget* m_gameWidget;
		int m_laserToUse;
		KBBGraphicsItemTutorialMarker* m_marker;
		QLabel* m_playerAction;
		QProgressBar* m_progression;

		/**
		 * @brief Tutorial step number
		 */
		int m_step;

		int m_stepMaxAllowed;
		QLabel* m_title;
};

#endif //KBBTUTORIAL_H
