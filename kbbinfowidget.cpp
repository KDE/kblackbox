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



#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>


#include <klocale.h>


#include "kbbballsgraphicwidget.h"
#include "kbbinfowidget.h"



//
// Constructor / Destructor
//

KBBInfoWidget::KBBInfoWidget(KBBThemeManager* themeManager)
{
	QHBoxLayout *mainLayout = new QHBoxLayout();
	
	// TODO: The following line should depend on the locale to correct manage languages that display text from right to left. See note in file "kbbballsstatusbarwidget.h".
	mainLayout->setDirection(QBoxLayout::LeftToRight);
	
	m_title = new QLabel(i18n("Balls to place:"), this);
	mainLayout->addWidget(m_title);
	
	m_ballsWidget = new KBBBallsGraphicWidget(BALL_SIZE, themeManager);
	mainLayout->addWidget(m_ballsWidget);
	
	setLayout(mainLayout);
	
	setMinimumHeight(BALL_SIZE);
}



//
// Public
//

void KBBInfoWidget::setBalls(const int placedBalls, const int ballsToPlace)
{
	m_ballsWidget->setBallsToPlace(ballsToPlace);
	m_ballsWidget->setPlacedBalls(placedBalls);
}
