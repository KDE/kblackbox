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



#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>


#include <kgamelcd.h>
#include <klocale.h>


#include "kbbballsgraphicwidget.h"
#include "kbbinfowidget.h"



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
	QVBoxLayout *ballsLayout = new QVBoxLayout();
	m_ballsTitle = new QLabel(this);
	m_ballsTitle->setWordWrap(true);
	ballsLayout->addWidget(m_ballsTitle);
	m_ballsWidget = new KBBBallsGraphicWidget(BALL_SIZE, themeManager);
	m_ballsWidget->setMinimumWidth(BALL_SIZE);
	ballsLayout->addWidget(m_ballsWidget);
	ballsGroupBox->setLayout(ballsLayout);
	mainLayout->addWidget(ballsGroupBox);
	
	
	// 2nd part: score
	QGroupBox *scoreGroupBox = new QGroupBox(i18n("Score"));
	QVBoxLayout *scoreLayout = new QVBoxLayout();
	m_score = new KGameLCD(3, this);
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
		m_ballsTitle->setText(i18n("%1 balls too much!", -ballsLeftToPlace));
	}
	if (ballsLeftToPlace==0) {
		m_ballsTitle->setText(i18n("You're done!"));
	}
	if (ballsLeftToPlace>0)
		m_ballsTitle->setText(i18n("%1 balls to place", ballsLeftToPlace));
}


void KBBInfoWidget::setScore(const int score)
{
	m_score->displayInt(score);
	if (score>m_scoreLimit)
		m_score->highlight();
}
