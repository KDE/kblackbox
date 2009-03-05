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
#include <QTimer>
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
	m_progression->setWhatsThis(i18n("Displays the progress of the tutorial."));
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
	m_playerAction->setWhatsThis(i18n("Describes what you should do to reach the next tutorial step."));
	m_playerAction->setAlignment(Qt::AlignLeft);
	m_playerAction->setWordWrap(true);
	m_playerAction->setFrameStyle(QFrame::StyledPanel);
	m_playerAction->setStyleSheet("border-style: none");
	actionLayout->addWidget(m_playerAction, 0, Qt::AlignVCenter);

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	tutorialLayout->addLayout(buttonLayout);
	m_buttonPrevious = new KPushButton(KIcon("go-previous"), i18nc("Previous tutorial step", "&Previous"), this);
	m_buttonPrevious->setWhatsThis(i18n("Go back to the previous tutorial step."));
	connect(m_buttonPrevious, SIGNAL(clicked()), this, SLOT(previousStep()));
	buttonLayout->addWidget(m_buttonPrevious);
	m_buttonNext = new KPushButton(KIcon("go-next"), i18nc("Next tutorial step", "&Next"), this);
	m_buttonNext->setWhatsThis(i18n("Go to the next tutorial step."));
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
			// Highlight m_playerAction to show what the player has to do
			m_playerAction->setStyleSheet("color: black; background-color: #de0000");
			QTimer::singleShot(HIGHLIGHT_TIME, this, SLOT(restoreStyle()));
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
			m_gameWidget->newGame(KBBTutorial::COLUMNS, KBBTutorial::ROWS, KBBTutorial::BALLS);
			setTexts(i18n("Welcome!"), i18n("This tutorial will teach you how to play KBlackBox, using a simple example.<br /><br />We are playing with a square black box of 6 columns and 6 rows. It has 3 balls <b>hidden</b> inside it and 24 laser probes around it.<br /><br />The goal is to <b>find the positions of the balls.</b>"), i18n("Click on \"Next\""));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+1);
			break;
		case FIRST_STEP+1:
			setTexts(i18n("Black box principles"), i18n("The balls are not visible, but we can shoot laser beams into the box at different entry points and observe if the laser beams leave the box, and if they do, where they come out.<br /><br />The laser beams interact with the balls in various ways."), i18n("Please click on the marked laser to shoot a beam into the black box."));
			m_laserToUse = 0;
			m_gameWidget->drawRay(0);
			break;
		case FIRST_STEP+2:
			setTexts(i18n("No interaction"), i18n("If a laser beam does not interact with any ball in the black box, it comes out at the point opposite the entry point.<br /><br />Example: Suppose we have 3 balls in the box as shown. They will not affect laser beam \"1\".<br /><br />As the game progresses, each pair of entry/exit points is marked with a different number."), i18n("Now shoot the marked laser to discover the first kind of interaction."));
			m_laserToUse = 4;
			m_gameWidget->addBallUnsure(8);
			m_gameWidget->addBallUnsure(27);
			m_gameWidget->addBallUnsure(34);
			m_gameWidget->drawRay(0);
			break;
		case FIRST_STEP+3:
			setTexts(i18n("Hit"), i18n("A direct impact on a ball is called a \"<b>hit</b>\". A beam that hits a ball does <b>not</b> emerge from the black box.<br /><br />Example: The beam might have hit a ball at the position shown, but the exact position of the hit is not certain: There are many other possibilities."), i18n("Shoot the marked laser to discover the second kind of interaction."));
			m_laserToUse = 22;
			m_gameWidget->addBallUnsure(28);
			m_gameWidget->drawRay(4);
			break;
		case FIRST_STEP+4:
			setTexts(i18n("Simple deflection"), i18n("The interaction of a beam that does not actually hit a ball, but aims to one side of it, is called a \"<b>deflection</b>\". The angle of deflection of the beam is always <b>90 degrees</b>.<br /><br />Example: The ball shown would deflect beam \"2\" upward, as shown, but this is <b>not</b> the only possibility."), i18n("Click on \"Next\" to see another combination of ball positions that deflects the laser beam as shown."));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+5);
			m_gameWidget->addBallUnsure(16);
			m_gameWidget->drawRay(22);
			break;
		case FIRST_STEP+5:
			setTexts(i18n("Several deflections"), i18n("As you can see, interactions in the black box can be quite complicated!<br />A laser beam entering and exiting at the positions \"2\" might have been deflected by this configuration of 3 balls."), i18n("Shoot the marked laser to discover another kind of result."));
			m_laserToUse = 19;
			m_gameWidget->addBallUnsure(5);
			m_gameWidget->addBallUnsure(26);
			m_gameWidget->addBallUnsure(29);
			m_gameWidget->drawRay(22);
			break;
		case FIRST_STEP+6:
			setTexts(i18n("Reflection"), i18n("If the laser beam leaves the black box <b>at the entry point</b>, it has been reflected backward inside the black box.<br /><br />Example: We have placed 2 balls for you in a configuration that would lead to such a reflection."), i18n("Shoot the marked laser to see another backward reflection case."));
			m_laserToUse = 15;
			m_gameWidget->addBallUnsure(22);
			m_gameWidget->addBallUnsure(34);
			m_gameWidget->drawRay(19);
			break;
		case FIRST_STEP+7:
			setTexts(i18n("Special reflection"), i18n("If a ball is <b>at the edge of the box</b> (with no other ball nearby), a beam which is aimed into the black box directly beside it causes a backward reflection.<br /><br />Example: The configuration shown can cause a backward reflection."), i18n("Nearly done. Click on \"Next\"."));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+8);
			m_gameWidget->addBallUnsure(33);
			m_gameWidget->drawRay(15);
			break;
		case FIRST_STEP+8:
			setTexts(i18n("Marker for \"free position\""), i18n("We are sure there are no balls in the first 2 columns. If there were any, the beam entering at position \"1\" would hit a ball or be deflected by a ball in column 2. You can mark a \"free position\" with a right mouse click (see also keyboard shortcuts).<br /><br />Example: There are 12 markers in the first 2 columns."), i18n("Click on \"Next\"."));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+9);
			for (int i=0;i<ROWS;i++) {
				m_gameWidget->addMarkerNothing(i*COLUMNS);
				m_gameWidget->addMarkerNothing(i*COLUMNS+1);
			}
			break;
		case FIRST_STEP+9:
			setTexts(i18n("Marking balls"), i18n("When you have worked out where a ball is, please use the left mouse button to mark it. To remove a ball mark, use the left mouse button again. Last tip: If you are not sure about a position, you can use a right click on a ball to mark it as \"unsure\". (See also keyboard shortcuts.)<br /><br />Example: We marked one position as sure, the other one as unsure."), i18n("Click on \"Next\"."));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+10);
			m_gameWidget->addBall(33);
			m_gameWidget->addBallUnsure(35);
			break;
		case FIRST_STEP+10:
			setTexts(i18n("Let us play!"), i18n("<b>Congratulations!</b> You now know <b>all the rules</b> for KBlackBox.<br /><br /><b>You can start to play.</b> Try to finish this tutorial game by yourself!<br /><br />Tip: We have sent in enough beams to deduce the positions of the 3 balls with certainty. Of course, you can use some more shots if needed."), i18n("Finish placing the balls and click on \"Done!\" when you are done!"));
			m_laserToUse = MAY_NOT_USE;
			setNewStepMaxAllowed(FIRST_STEP+9);
			break;
	}

	m_buttonPrevious->setEnabled(m_step!=FIRST_STEP);
	m_buttonNext->setEnabled(m_step<m_stepMaxAllowed);
	m_progression->setValue(m_step);
	showMarker(m_laserToUse);
}


void KBBTutorial::start()
{
	m_stepMaxAllowed = 0;
	show();
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


void KBBTutorial::restoreStyle()
{
	m_playerAction->setStyleSheet("color: palette(text); background-color: palette(window)");
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
	m_playerAction->setText("<qt><b>" + action + "</b></qt>");
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
