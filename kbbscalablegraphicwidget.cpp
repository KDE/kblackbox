//
// KBlackBox
//
// A simple game inspired by an emacs module
//
/***************************************************************************
 *   Copyright (c) 1999-2000, Robert Cimrman                               *
 *   cimrman3@students.zcu.cz                                              *
 *                                                                         *
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

#include "kbbscalablegraphicwidget.h"



#include <QAction>
#include <QFont>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLCDNumber>
#include <QResizeEvent>


#include <kgamepopupitem.h>
#include <kicon.h>
#include <klocale.h>
#include <kpushbutton.h>


#include "kbbballsonboard.h"
#include "kbbgamedoc.h"
#include "kbbgraphicsitemball.h"
#include "kbbgraphicsitemballrepository.h"
#include "kbbgraphicsitemblackbox.h"
#include "kbbgraphicsitemcursor.h"
#include "kbbgraphicsitemlaser.h"
#include "kbbgraphicsitemonbox.h"
#include "kbbgraphicsitemray.h"
#include "kbbgraphicsitemrayresult.h"
#include "kbbgraphicsitemset.h"
#include "kbbthememanager.h"



//
// Constructor / Destructor
//

KBBScalableGraphicWidget::KBBScalableGraphicWidget(KBBGameDoc* gameDoc, KBBThemeManager* themeManager, QAction* done)
{
	m_gameDoc = gameDoc;
	m_themeManager = themeManager;
	m_columns = -2;
	m_rows = -2;
	m_pause = false;
	m_ballNumber = 0;
	m_doneAction = done;


	setFrameStyle(QFrame::NoFrame);
	setCacheMode(QGraphicsView::CacheBackground);
	setMinimumSize(QSize(MINIMUM_SIZE, MINIMUM_SIZE));


	m_scene = new QGraphicsScene( 0, 0, 2*BORDER_SIZE, 2*BORDER_SIZE, this );
	
	m_blackbox = new KBBGraphicsItemBlackBox(this, m_scene, m_themeManager);
	m_blackbox->setKBBScalableGraphicWidget(this);
	m_balls = new KBBGraphicsItemSet(m_scene);
	m_cursor = new KBBGraphicsItemCursor(this, m_themeManager);
	connect(m_cursor, SIGNAL(cursorAtNewPosition(int)), this, SLOT(cursorAtNewPosition(int)));
	m_markersNothing = new KBBGraphicsItemSet(m_scene);
	m_ballsSolution = new KBBGraphicsItemSet(m_scene);
	m_ballsUnsure = new KBBGraphicsItemSet(m_scene);
	m_lasers = new KBBGraphicsItemSet(m_scene);
	m_rayResults = new KBBGraphicsItemSet(m_scene);
	
	m_playerRay = new KBBGraphicsItemRay(playerRay, m_scene, m_themeManager);
	m_solutionRay = new KBBGraphicsItemRay(solutionRay, m_scene, m_themeManager);
	
	// Information message about the score
	m_infoScore = new KGamePopupItem();
	m_infoScore->setMessageIcon(QPixmap()); // No icon, because they are no scalable.
	m_scene->addItem(m_infoScore); // it hides itself by default

	m_ballRepository = new KBBGraphicsItemBallRepository(this, themeManager);

	setScene(m_scene);

	m_doneButton = new KPushButton(m_doneAction->text(), this);
	m_doneButton->setIcon(KIcon(m_doneAction->icon()));
	m_doneButton->setWhatsThis(m_doneAction->whatsThis());
	connect(m_doneButton, SIGNAL(clicked(bool)), m_doneAction, SLOT(trigger()));
	QFont font;
	font.setPointSize(m_doneButton->font().pointSize()+2);
	font.setBold(true);
	m_doneButton->setFont(font);

	m_score = new QLCDNumber(3, this);
	m_score->setFrameStyle(QFrame::NoFrame);
	m_score->setMaximumWidth(m_doneButton->width());
	m_score->setFixedHeight((int)(1.5*m_doneButton->height()));
	m_score->setToolTip(i18n("Score"));
	m_score->setWhatsThis(i18n("<qt><p>This is <b>your score</b>. You should try to get the lowest possible.</p><p>The score increases:<ul><li>with time: <b>1 point</b> per second.</li><li>with the use of lasers:<ul><li><b>3 points</b> if the laser beam hits a ball or exits at the entry point,</li><li><b>9 points</b> if it exits at another entry point.</li></ul></li></ul></p><p>Your score is set to <b>999</b> at the end of the game if you make a mistake.</p></qt>"));

	// TODO: not displayed... :(
	setWhatsThis(i18n("<qt><p>This is the <b>main game area</b>.</p><ul><li>The <b>black box</b> is in the center.</li><li>On the left, there are the <b>balls</b> you have to place over the black box.</li><li>Around the black box, there are <b>lasers</b> that are replaced with <b>interaction information</b> if you use them.</li></ul></qt>"));

}


KBBScalableGraphicWidget::~KBBScalableGraphicWidget()
{
	delete m_balls;
	delete m_markersNothing;
	delete m_ballsSolution;
	delete m_ballsUnsure;
	delete m_lasers;
	delete m_rayResults;

	delete m_ballRepository;

	delete m_scene;
}



//
// Public
//

void KBBScalableGraphicWidget::addBall(int boxPosition)
{
	addBall(boxPosition, KBBGraphicsItemOnBox::NO_POSITION);
}


void KBBScalableGraphicWidget::addBall(int boxPosition, int outsidePosition)
{
	if (!m_pause && m_inputAccepted && (!m_balls->containsVisible(boxPosition))&& (!m_ballsUnsure->containsVisible(boxPosition))) {
		m_boardBallsPlaced->add(boxPosition);
		m_balls->insert(new KBBGraphicsItemBall(playerBall, this, m_themeManager, boxPosition, m_columns, m_rows));
		m_markersNothing->remove(boxPosition);
		if (outsidePosition==KBBGraphicsItemOnBox::NO_POSITION) {
			outsidePosition = m_ballRepository->ballToTake();
		}
		if (outsidePosition!=KBBGraphicsItemSet::NO_INDEX)
			m_ballRepository->removeBall(outsidePosition);

		updateDoneButton();
	}
}


void KBBScalableGraphicWidget::addBallUnsure(const int boxPosition)
{
	addBall(boxPosition);
	setBallUnsure(boxPosition, true);
}


void KBBScalableGraphicWidget::addMarkerNothing(const int boxPosition)
{
	if (!m_pause && m_inputAccepted && (!m_markersNothing->containsVisible(boxPosition))) {
		m_markersNothing->insert(new KBBGraphicsItemOnBox(markerNothing, this, m_themeManager, boxPosition, m_columns, m_rows));
		m_balls->remove(boxPosition);
		m_ballsUnsure->remove(boxPosition);
		m_boardBallsPlaced->remove(boxPosition);
	}
}


void KBBScalableGraphicWidget::drawRay(const int borderPosition)
{
	if (!m_pause) {
		if (!m_inputAccepted) {
			m_solutionRay->draw(m_boardBalls, borderPosition);
		}
		m_playerRay->draw(m_boardBallsPlaced, borderPosition);
	}
}


void KBBScalableGraphicWidget::mouseBorderClick(const int borderPosition)
{
	useLaser(borderPosition);
	m_cursor->setBorderPosition(borderPosition);
	m_cursor->hide();
}


void KBBScalableGraphicWidget::mouseBoxClick(const Qt::MouseButton button, int boxPosition)
{
	m_cursor->setBoxPosition(boxPosition);
	if (button==Qt::RightButton)
		switchMarker();
	else
		switchBall();
	m_cursor->hide();
}


int KBBScalableGraphicWidget::moveBall(const int boxPositionFrom, const int boxPositionTo)
{
	int newPos = positionAfterMovingBall(boxPositionFrom, boxPositionTo);

	if (!m_pause && m_inputAccepted && (!m_balls->containsVisible(boxPositionTo)) && (!m_ballsUnsure->containsVisible(boxPositionTo))) {
		m_markersNothing->remove(boxPositionTo);
		if (boxPositionFrom>=m_columns*m_rows) {
			// ball moved from outside of the board
			addBall(boxPositionTo, boxPositionFrom);
		} else {
			// ball moved from a board position
			m_boardBallsPlaced->remove(boxPositionFrom);
			m_boardBallsPlaced->add(boxPositionTo);
		}
	}

	return newPos;
}


int KBBScalableGraphicWidget::moveMarkerNothing(const int boxPositionFrom, const int boxPositionTo)
{
	if (!m_pause && m_inputAccepted && (!m_markersNothing->containsVisible(boxPositionTo))) {
		removeBall(boxPositionTo);
		return boxPositionTo;
	} else
		return boxPositionFrom;
}


void KBBScalableGraphicWidget::newGame(int columns, int rows, int ballNumber)
{
	m_rayNumber = 0;
	m_boardBallsPlaced = m_gameDoc->m_ballsPlaced;
	setPause(false);
	m_ballNumber = ballNumber;

	// remove old ray results, all placed balls, all markers "nothing" and all solutions
	m_rayResults->clear();
	m_balls->clear();
	m_ballsUnsure->clear();
	m_markersNothing->clear();
	m_ballsSolution->clear();

	// Reorganize lasers
	if ((columns!=m_columns) || (rows!=m_rows)) {
		// not the same amount of lasers: We can destroy them and create some new ones
		m_lasers->clear();
		for (int i=0; i<2*(columns + rows); i++)
			m_lasers->insert(new KBBGraphicsItemLaser(this, m_themeManager, i, columns, rows));
	} else {
		// same amount of lasers: We "recycle" them. (Just destroying them and re-creating them is not working fine: some lasers remain hidden until the next resize... Strange bug with QGraphicsView...)
		for (int i=0; i<2*(m_columns + m_rows); i++)
			m_lasers->setVisible(i, true);
	}

	m_ballRepository->newGame(columns, rows, ballNumber);

	// set the new size if needed
	if (m_columns!=columns || m_rows!=rows) {
		m_columns = columns;
		m_rows = rows;
		m_blackbox->setSize(m_columns, m_rows);
		m_cursor->setBoardSize(m_columns, m_rows);
		m_scene->setSceneRect(m_ballRepository->x() - RATIO, 0, m_columns*RATIO + 2*BORDER_SIZE - m_ballRepository->x() + RATIO, m_rows*RATIO + 2*BORDER_SIZE);
		resizeEvent(0);
	}
	setInputAccepted(true);
}


void KBBScalableGraphicWidget::popupText(const QString& text, int time)
{
	if (text.isEmpty())
		m_infoScore->forceHide();
	else {
		m_infoScore->setMessageTimeout(time);
		m_infoScore->showMessage(text, KGamePopupItem::TopLeft, KGamePopupItem::ReplacePrevious);
	}
}


int KBBScalableGraphicWidget::positionAfterMovingBall(const int boxPositionFrom, const int boxPositionTo) const
{
	if (!m_pause && m_inputAccepted && (!m_balls->containsVisible(boxPositionTo)) && (!m_ballsUnsure->containsVisible(boxPositionTo))) {
		return boxPositionTo;
	} else
		return boxPositionFrom;
}


void KBBScalableGraphicWidget::setPause(bool state)
{
	m_pause = state;
	for (int i=0;i<2*(m_rows+m_columns);i++) {
		if (m_rayResults->containsVisible(i))
			m_rayResults->item(i)->setPause(state);
	}

	updateDoneButton();
}


void KBBScalableGraphicWidget::resizeEvent( QResizeEvent* )
{
	// 1. Compute the size of m_rectBackground
	const qreal sW = m_scene->width();
	const qreal sH = m_scene->height();
	const qreal wW = width();
	const qreal wH = height();
	const qreal offset = (sH+sW)/100 ;
	if (sH*wW > sW*wH) {
		// The widget is larger than the scene
		qreal w =  wW*sH/wH;
		m_rectBackground.setRect((sW-w)/2-offset+m_ballRepository->x()-RATIO, -offset, w + 2*offset, sH + 2*offset);
	} else {
		// The scene is larger than the widget (or as large)
		qreal h =  wH*sW/wW;
		m_rectBackground.setRect(-offset+m_ballRepository->x()-RATIO, (sH-h)/2-offset, sW + 2*offset, h + 2*offset);
	}
	
	// 2. Resize the scene to fit in the widget
	fitInView(m_ballRepository->x()-RATIO, 0, m_columns*RATIO + 2*BORDER_SIZE - m_ballRepository->x() + RATIO, m_rows*RATIO + 2*BORDER_SIZE, Qt::KeepAspectRatio);


	m_doneButton->move(OFFSET_DONE_BUTTON, height() - m_doneButton->height() - OFFSET_DONE_BUTTON);
	m_score->move(OFFSET_DONE_BUTTON, height() - m_score->height() - m_doneButton->height() - 3*OFFSET_DONE_BUTTON);
}


void KBBScalableGraphicWidget::removeAllBalls()
{
	for (int i=0;i<m_columns*m_rows;i++) {
		removeBall(i);
	}
}


void KBBScalableGraphicWidget::removeBall(const int boxPosition)
{
	if (!m_pause && m_inputAccepted) {
		m_balls->remove(boxPosition);
		m_ballsUnsure->remove(boxPosition);
		m_boardBallsPlaced->remove(boxPosition);
		m_ballRepository->fillBallsOutside(m_gameDoc->m_ballsPlaced->count());

		updateDoneButton();
	}
}


void KBBScalableGraphicWidget::removeRay()
{
	m_playerRay->hide();
	m_solutionRay->hide();
}


QGraphicsScene* KBBScalableGraphicWidget::scene()
{
	return m_scene;
}


void KBBScalableGraphicWidget::setScore(int score)
{
	m_score->display(score);
}


void KBBScalableGraphicWidget::solve(const bool continueGame)
{
	m_boardBalls = m_gameDoc->m_balls;
	
	setInputAccepted(continueGame);
	if (!continueGame)
		setPause(false);
	
	for (int i=0; i<(m_columns * m_rows); i++) {
		if ((m_balls->containsVisible(i) || m_ballsUnsure->containsVisible(i)) && m_boardBalls->contains(i)) {
			m_ballsSolution->remove(i); // For the sandbox mode: a solution ball could already be here.
			m_ballsSolution->insert(new KBBGraphicsItemBall(rightPlayerBall, this, m_themeManager, i, m_columns, m_rows));
		}
		if ((m_balls->containsVisible(i) || m_ballsUnsure->containsVisible(i)) && !m_boardBalls->contains(i))
			m_ballsSolution->insert(new KBBGraphicsItemOnBox(wrongPlayerBall, this, m_themeManager, i, m_columns, m_rows));
		if (!m_balls->containsVisible(i) && !m_ballsUnsure->containsVisible(i) && m_boardBalls->contains(i))
			m_ballsSolution->insert(new KBBGraphicsItemBall(solutionBall, this, m_themeManager, i, m_columns, m_rows));
	}
}



//
// Public slots
//

void KBBScalableGraphicWidget::cursorAtNewPosition(int borderPosition)
{
	removeRay();
	if ((borderPosition!=KBBGraphicsItemCursor::NO_POSITION) && m_cursor->isVisible())
		drawRay(borderPosition);

	// highlight
	for (int i=0;i<2*(m_rows+m_columns);i++) {
		if (m_rayResults->containsVisible(i))
			m_rayResults->item(i)->highlight(false);
	}
	if (m_rayResults->containsVisible(borderPosition))
		m_rayResults->item(borderPosition)->highlightBoth(true);

}


void KBBScalableGraphicWidget::keyboardEnter()
{
	if (m_cursor->isVisible()) {
		if (m_cursor->borderPosition() != KBBGraphicsItemCursor::NO_POSITION)
			useLaser(m_cursor->borderPosition());
		else
			switchBall();
	}
	m_cursor->show();
}


void KBBScalableGraphicWidget::keyboardMoveDown()
{
	if (m_cursor->isVisible())
		m_cursor->moveDown();
	m_cursor->show();
}


void KBBScalableGraphicWidget::keyboardMoveLeft()
{
	if (m_cursor->isVisible())
		m_cursor->moveLeft();
	m_cursor->show();
}


void KBBScalableGraphicWidget::keyboardMoveRight()
{
	if (m_cursor->isVisible())
		m_cursor->moveRight();
	m_cursor->show();
}


void KBBScalableGraphicWidget::keyboardMoveUp()
{
	if (m_cursor->isVisible())
		m_cursor->moveUp();
	m_cursor->show();
}


void KBBScalableGraphicWidget::keyboardSpace()
{
	if (m_cursor->isVisible()) {
		if (m_cursor->boxPosition() != KBBGraphicsItemCursor::NO_POSITION)
			switchMarker();
	}
	m_cursor->show();
}



//
// Protected
//

void KBBScalableGraphicWidget::drawBackground(QPainter* painter, const QRectF&)
{
	m_themeManager->svgRenderer()->render(painter, m_themeManager->elementId(background), m_rectBackground);
}



//
// Private
//

void KBBScalableGraphicWidget::removeMarkerNothing(const int boxPosition)
{
	if (!m_pause && m_inputAccepted) {
		m_markersNothing->remove(boxPosition);
	}
}


void KBBScalableGraphicWidget::setBallUnsure(const int boxPosition, const bool unsure)
{
	if (!m_pause && m_inputAccepted) {
		if (unsure) {
			m_balls->remove(boxPosition);
			m_ballsUnsure->insert(new KBBGraphicsItemBall(unsureBall, this, m_themeManager, boxPosition, m_columns, m_rows));
		} else {
			m_ballsUnsure->remove(boxPosition);
			m_balls->insert(new KBBGraphicsItemBall(playerBall, this, m_themeManager, boxPosition, m_columns, m_rows));
		}
	}
}


void KBBScalableGraphicWidget::setInputAccepted(bool inputAccepted)
{
	m_inputAccepted = inputAccepted;
	if (m_inputAccepted) {
		setFocusPolicy( Qt::StrongFocus );
		setFocus();
	} else {
		setFocusPolicy( Qt::NoFocus );
		clearFocus();
	}

	updateDoneButton();
}


void KBBScalableGraphicWidget::switchBall()
{
	if ((m_balls->containsVisible(m_cursor->boxPosition())) || (m_ballsUnsure->containsVisible(m_cursor->boxPosition())))
		removeBall(m_cursor->boxPosition());
	else
		addBall(m_cursor->boxPosition());
}


void KBBScalableGraphicWidget::switchMarker()
{
	if (m_balls->containsVisible(m_cursor->boxPosition()))
		setBallUnsure(m_cursor->boxPosition(), true);
	else if (m_markersNothing->containsVisible(m_cursor->boxPosition()))
		removeMarkerNothing(m_cursor->boxPosition());
        else{
		removeBall(m_cursor->boxPosition());
		addMarkerNothing(m_cursor->boxPosition());
        }
}


void KBBScalableGraphicWidget::updateDoneButton()
{
	m_doneButton->setEnabled(m_doneAction->isEnabled());
	m_doneButton->setToolTip(m_doneAction->toolTip());
}


void KBBScalableGraphicWidget::useLaser(const int incomingPosition)
{
	if (!m_pause && m_gameDoc->mayShootRay(incomingPosition) && m_inputAccepted && m_lasers->containsVisible(incomingPosition)) {
		const int outgoingPosition = m_gameDoc->shootRay(incomingPosition);
		
		KBBGraphicsItemRayResult* inRay;
		KBBGraphicsItemRayResult* outRay;
		
		int rayNumberOrReflection = 0;
		if (outgoingPosition==KBBGameDoc::HIT_POSITION)
			rayNumberOrReflection = KBBGameDoc::HIT_POSITION;
		if ((outgoingPosition!=incomingPosition) && (outgoingPosition!=KBBGameDoc::HIT_POSITION)) {
			m_rayNumber++;
			m_lasers->setVisible(outgoingPosition, false);
			m_rayResults->insert(outRay = new KBBGraphicsItemRayResult(this, m_themeManager, m_scene, outgoingPosition, m_columns, m_rows, m_rayNumber));
			rayNumberOrReflection = m_rayNumber;
		}
		m_rayResults->insert(inRay = new KBBGraphicsItemRayResult(this, m_themeManager, m_scene, incomingPosition, m_columns, m_rows, rayNumberOrReflection));
		
		if ((outgoingPosition!=incomingPosition) && (outgoingPosition!=KBBGameDoc::HIT_POSITION)) {
			inRay->setOpposite(outRay);
			outRay->setOpposite(inRay);
		}
		
		m_scene->update();
		m_lasers->setVisible(incomingPosition, false);

		popupText(""); // To Remove any displayed text quicker.

		cursorAtNewPosition(incomingPosition);
	}
}


#include "kbbscalablegraphicwidget.moc"
