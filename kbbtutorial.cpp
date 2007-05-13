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

#include "kbbtutorial.h"



#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>


#include <kicon.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <ktextedit.h>


#include "kbbgraphicsitemtutorialmarker.h"
#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBTutorial::KBBTutorial(QWidget* parent) : QGroupBox(i18n("Tutorial"), parent)
{
	m_marker = 0;
	m_gameWidget = 0;
	m_stepMaxAllowed = 0;

	setMinimumSize(QSize(WIDTH, WIDTH));
	setFixedWidth(WIDTH);

	QVBoxLayout *tutorialLayout = new QVBoxLayout();
	setLayout(tutorialLayout);
	setFlat(true);

	m_progression = new QProgressBar(this);
	m_progression->setTextVisible(true);
	m_progression->setFormat("%v / %m");
	m_progression->setMinimum(FIRST_STEP-1);
	m_progression->setMaximum(LAST_STEP);
	m_progression->setWhatsThis(i18n("Display the progression in the tutorial."));
	tutorialLayout->addWidget(m_progression);

	m_title = new QLabel(this);
	tutorialLayout->addWidget(m_title, 0, Qt::AlignHCenter);

	m_explanation = new KTextEdit(this);
	m_explanation->setReadOnly(true);
	m_explanation->setFrameStyle(QFrame::NoFrame);
	m_explanation->setAlignment(Qt::AlignJustify);
	tutorialLayout->addWidget(m_explanation);

	tutorialLayout->addStretch();


	QHBoxLayout *actionLayout = new QHBoxLayout();
	tutorialLayout->addLayout(actionLayout);
	QLabel* iconLabel = new QLabel(this);
	iconLabel->setFixedSize(24, 24);
	iconLabel->setPixmap(KIcon("go-next").pixmap(24, 24));
	actionLayout->addWidget(iconLabel, 0, Qt::AlignVCenter);
	m_playerAction = new QLabel(this);
	m_playerAction->setWhatsThis(i18n("Describe what you have to do to reach the next tutorial step."));
	m_playerAction->setAlignment(Qt::AlignLeft);
	m_playerAction->setWordWrap(true);
	actionLayout->addWidget(m_playerAction, 0, Qt::AlignVCenter);

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	tutorialLayout->addLayout(buttonLayout);
	m_buttonPrevious = new KPushButton(KIcon("go-previous"), i18n("&Previous"), this);
	m_buttonPrevious->setWhatsThis(i18n("Jump to the previous tutorial step."));
	connect(m_buttonPrevious, SIGNAL(clicked()), this, SLOT(previousStep()));
	buttonLayout->addWidget(m_buttonPrevious);
	m_buttonNext = new KPushButton(KIcon("go-next"), i18n("&Next"), this);
	m_buttonNext->setWhatsThis(i18n("Jump to the next tutorial step."));
	connect(m_buttonNext, SIGNAL(clicked()), this, SLOT(nextStep()));
	m_buttonNext->setDefault(true);
	buttonLayout->addWidget(m_buttonNext);
}



//
// Public
//

void KBBTutorial::hideEvent(QHideEvent*)
{
	showMarker(MAY_NOT_USE);
}


bool KBBTutorial::maySolve()
{
	return m_step==LAST_STEP;
}


bool KBBTutorial::mayShootRay(const int incomingPosition)
{
	if (m_step==LAST_STEP)
		return true;
	else
		if (incomingPosition==m_laserToUse) {
			nextStep();
			return true;
		} else {
			// TODO: Highlight m_playerAction
			return false;
		}
}


void KBBTutorial::setGameWidget(KBBScalableGraphicWidget* gameWidget, KBBGraphicsItemTutorialMarker* marker)
{
	m_gameWidget = gameWidget;
	m_marker = marker;
}


void KBBTutorial::setStep(const int step)
{
	Q_ASSERT((step>=FIRST_STEP) && (step<=LAST_STEP));
	Q_ASSERT(m_gameWidget!=0);

	m_step = step;
	setNewStepMaxAllowed(m_step);

	if (m_step!=LAST_STEP)
		m_gameWidget->removeAllBalls();

	switch (m_step) {
		case FIRST_STEP:
			m_gameWidget->newGame(KBBTutorial::COLUMNS, KBBTutorial::ROWS);
			setTexts(i18n("Welcome!"), i18n("This tutorial will teach you how to play on the basis of an easy example.<br /><br />We are playing with a square black box of 6 columns and 6 rows with 3 balls <b>hidden</b> into it and 24 lasers around it.<br /><br />The goal is to <b>find the position of the balls.</b>"), i18n("Click on \"Next\""));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+1);
			break;
		case FIRST_STEP+1:
			setTexts(i18n("Black box principles"), i18n("The hidden balls are not visible, but we can (and will) shoot laser rays into the box at different entry points and observe if the laser rays exit the box, and if they do, where they exit.<br /><br />The balls are namely unvisible, but they interact in different ways with laser rays."), i18n("Please shoot a ray with the marked laser."));
			m_laserToUse = 0;
			m_gameWidget->drawRay(0);
			break;
		case FIRST_STEP+2:
			setTexts(i18n("No interaction"), i18n("If a laser ray doen't interact with any ball in the black box, it exits at the opposite side of the enter point.<br /><br />Example: we put for you 3 balls on the box (it is just a supposition). They do not interact with the shot ray.<br /><br />Every couple of entry/exit point is numbered with a different number."), i18n("Shoot the marked laser to discover the first kind of interaction."));
			m_laserToUse = 4;
			m_gameWidget->addBallUnsure(8);
			m_gameWidget->addBallUnsure(27);
			m_gameWidget->addBallUnsure(34);
			m_gameWidget->drawRay(0);
			break;
		case FIRST_STEP+3:
			setTexts(i18n("Hit"), i18n("A direct impact on a ball by a ray is a \"<b>hit</b>\". A ray which hits a ball does <b>not</b> emerge from the black box.<br /><br />Example: the shot laser ray may have hit the ball we placed on the box for you. The exact position of the hit ball is still unknown so far."), i18n("Shoot the marked laser to discover the second kind of interaction."));
			m_laserToUse = 22;
			m_gameWidget->addBallUnsure(28);
			m_gameWidget->drawRay(4);
			break;
		case FIRST_STEP+4:
			setTexts(i18n("Simple deflection"), i18n("The interaction resulting from a ray which does not actually hit a ball, but which passes directly to one side of the ball is called a \"<b>deflection</b>\". The angle of deflection for ray/ball interactions is <b>90 degrees</b>.<br /><br />Example: the placed ball would deflect the laser ray as expected. This position is a <b>possible</b> position. However, it is not the only one."), i18n("Click on \"Next\" to see another possible combination of ball positions that deflect the laser ray as expected."));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+5);
			m_gameWidget->addBallUnsure(16);
			m_gameWidget->drawRay(22);
			break;
		case FIRST_STEP+5:
			setTexts(i18n("Several deflections"), i18n("As you can see: interactions in the black box may be quite complicated!<br />A laser ray entering and exiting at the positions \"2\" would be deflected as expected with this configuration of balls."), i18n("Shoot the marked laser to discover another kind of result."));
			m_laserToUse = 19;
			m_gameWidget->addBallUnsure(5);
			m_gameWidget->addBallUnsure(26);
			m_gameWidget->addBallUnsure(29);
			m_gameWidget->drawRay(22);
			break;
		case FIRST_STEP+6:
			setTexts(i18n("Reflection"), i18n("If the laser ray exits the black box <b>at the entry point</b>, it has been reflected inside the black box.<br /><br />Example: We placed 2 balls for you in a configuration that would lead to a reflection."), i18n("Shoot the marked laser to see another reflection configuration."));
			m_laserToUse = 15;
			m_gameWidget->addBallUnsure(22);
			m_gameWidget->addBallUnsure(34);
			m_gameWidget->drawRay(19);
			break;
		case FIRST_STEP+7:
			setTexts(i18n("Special reflection"), i18n("If a ball is <b>at the edge of the box</b> (with no other ball around itself), any ray which is aimed into the black box directly beside it causes a reflection.<br /><br />Example: The following configuration causes a reflection."), i18n("Click on \"Next\"."));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+8);
			m_gameWidget->addBallUnsure(33);
			m_gameWidget->drawRay(15);
			break;
		case FIRST_STEP+8:
			setTexts(i18n("Marker \"free position\""), i18n("We are sure there are no ball in the first 2 colums. If there where some, rays entering at the position \"1\" would be hit or deflected. With a right mouse click (see also keyboard shortcuts), you can put a marker \"free position\".<br /><br />Example: We put for you 12 markers on the 2 first columns."), i18n("Click on \"Next\"."));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+9);
			for (int i=0;i<ROWS;i++) {
				m_gameWidget->addMarkerNothing(i*COLUMNS);
				m_gameWidget->addMarkerNothing(i*COLUMNS+1);
			}
			break;
		case FIRST_STEP+9:
			setTexts(i18n("Marker \"?\""), i18n("Last tip: if you are not sure about a ball position, you can also use the right click over a placed ball to mark this postion for you as \"unsure\".<br /><br />Example: We marked one position as sure, the other one as unsure."), i18n("Click on \"Next\"."));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+10);
			m_gameWidget->addBall(33);
			m_gameWidget->addBallUnsure(35);
			break;
		case FIRST_STEP+10:
			setTexts(i18n("Let us play!"), i18n("<b>Congratulations!</b> You know now <b>all the rules</b> that apply to the black box.<br /><br /><b>You can start to play</b>: try to finish this tutorial by yourself!<br /><br />Tip: We shot enough rays into the black box to be able to guess unequivocally the positions of the 3 balls. Of course, you can use some more lasers if needed."), i18n("Place all the balls and click on \"Solve\" when you are finished!"));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+9);
			break;
	}

	m_buttonPrevious->setEnabled(m_step!=FIRST_STEP);
	m_buttonNext->setEnabled(m_step<m_stepMaxAllowed);
	m_progression->setValue(m_step);
	showMarker(m_laserToUse);
}



//
// Private slots
//

void KBBTutorial::nextStep()
{
	setStep(m_step+1);
}


void KBBTutorial::previousStep()
{
	setStep(m_step-1);
}



//
// Private
//

void KBBTutorial::setNewStepMaxAllowed(const int newStepMax)
{
	if (m_step>m_stepMaxAllowed)
		m_stepMaxAllowed = m_step;
	if (newStepMax>m_stepMaxAllowed)
		m_stepMaxAllowed = newStepMax;
}


void KBBTutorial::setTexts(const QString &title, const QString &text, const QString &action)
{
	m_title->setText("<qt><strong>" + title + "</strong></qt>");
	m_explanation->setText("<qt>" + text + "</qt>");
	m_playerAction->setText("<qt><i><b>" + action + "</b></i></qt>");
}


void KBBTutorial::showMarker(const int laserPosition) const
{
	Q_ASSERT(m_marker!=0);

	if (laserPosition==MAY_NOT_USE)
		m_marker->hide();
	else {
		m_marker->setBorderPosition(laserPosition);
		m_marker->show();
	}
}

#include "kbbtutorial.moc"
