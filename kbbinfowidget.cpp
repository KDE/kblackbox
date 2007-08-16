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



#include <QAction>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QWidget>


#include <kgamelcd.h>
#include <klocale.h>



//
// Constructor / Destructor
//

KBBInfoWidget::KBBInfoWidget(KBBThemeManager* themeManager, QAction* check)
{
	// General
	m_scoreLimitDefinied = false;
	QVBoxLayout *mainLayout = new QVBoxLayout();
	setMaximumWidth(3*BALL_SIZE);

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
	m_scoreLimit = scoreLimit;
	m_scoreLimitDefinied = true;
}


void KBBInfoWidget::setScore(const int score)
{
	m_score->displayInt(score);
	if ((score>m_scoreLimit) && m_scoreLimitDefinied)
		m_score->highlight();
}
