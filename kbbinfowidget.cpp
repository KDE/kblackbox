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


#include "kbbinfowidget.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>


#include <kgamelcd.h>
#include <klocale.h>


#include "kbbballsgraphicwidget.h"



//
// Constructor / Destructor
//

KBBInfoWidget::KBBInfoWidget(KBBThemeManager* themeManager)
{
	// General
	QVBoxLayout *mainLayout = new QVBoxLayout();
	setMaximumWidth(3*BALL_SIZE);

	
	// 1st part: balls
	QGroupBox *ballsGroupBox = new QGroupBox(i18n("Balls"));
    ballsGroupBox->setFlat(true);
	QVBoxLayout *ballsLayout = new QVBoxLayout();
	m_ballsTitle = new QLabel(this);
	m_ballsTitle->setWordWrap(true);
	ballsLayout->addWidget(m_ballsTitle);
	m_ballsWidget = new KBBBallsGraphicWidget(themeManager);
	m_ballsWidget->setMinimumWidth(BALL_SIZE);
	ballsLayout->addWidget(m_ballsWidget);
	ballsGroupBox->setLayout(ballsLayout);
	mainLayout->addWidget(ballsGroupBox);
	
	
	// 2nd part: score
	QGroupBox *scoreGroupBox = new QGroupBox(i18n("Score"));
    scoreGroupBox->setFlat(true);
	QVBoxLayout *scoreLayout = new QVBoxLayout();
	m_score = new KGameLCD(3, this);
    m_score->setFrameStyle(QFrame::NoFrame);
	m_score->setMinimumHeight(3*BALL_SIZE/2);
	m_score->setHighlightColor(Qt::red);
	scoreLayout->addWidget(m_score);
	scoreGroupBox->setLayout(scoreLayout);
	mainLayout->addWidget(scoreGroupBox);
	
	
	setLayout(mainLayout);
}



//
// Public
//

void KBBInfoWidget::setGameParameters(const int ballsToPlace, const int scoreLimit)
{
	m_ballsToPlace = ballsToPlace;
	m_scoreLimit = scoreLimit;
	
	m_ballsWidget->setBallsToPlace(ballsToPlace);
}


void KBBInfoWidget::setPlacedBalls(const int placedBalls)
{
	m_ballsWidget->setPlacedBalls(placedBalls);
	
	int ballsLeftToPlace = m_ballsToPlace - placedBalls;
	if (ballsLeftToPlace<0) {
		m_ballsTitle->setText(i18np("1 ball too much", "%1 balls too much!", -ballsLeftToPlace));
		m_ballsTitle->setToolTip(i18np("You need to remove 1 ball from the black box.", "You need to remove %1 balls from the black box.", -ballsLeftToPlace));
	} else if (ballsLeftToPlace==0) {
		m_ballsTitle->setText(i18n("No more ball to place."));
		m_ballsTitle->setToolTip(i18n("If you think you are done: click on \"solve\" to check if you successful guessed the ball positions!"));
	} else if (ballsLeftToPlace>0) {
		m_ballsTitle->setText(i18np("1 ball to place", "%1 balls to place", ballsLeftToPlace));
		m_ballsTitle->setToolTip(i18np("You need to place 1 more ball on the black box.", "You need to place %1 more balls on the black box.\n", ballsLeftToPlace) + i18np("You will be done after having found the positions of the ball.", "You will be done after having found the positions of the %1 balls.", m_ballsToPlace));
	}
}


void KBBInfoWidget::setScore(const int score)
{
	m_score->displayInt(score);
	if (score>m_scoreLimit)
		m_score->highlight();
}
