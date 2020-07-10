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


#include "kbbthememanager.h"

#include <QColor>
#include <QDomDocument>
#include <QFile>

#include <QStringList>


#include <KFilterDev>
#include <QSvgRenderer>


#include "kbbscalablegraphicwidget.h"



//
// Constructor / Destructor
//

KBBThemeManager::KBBThemeManager(const QString &svgzFileName)
{
	// 1. for SVG items
	m_svgRenderer.load(svgzFileName);
	
	
	// 2. for non SVG items
	QFile svgzFile(svgzFileName);
	KCompressionDevice *f = new KCompressionDevice(&svgzFile, false, KCompressionDevice::GZip);
	
	if (f) {
		QDomDocument doc;
		if (doc.setContent(f,true)) {
			m_root = doc.documentElement();
		}
		
		delete f;
	}
}



//
// Public
//

QColor KBBThemeManager::color(const KBBScalableGraphicWidget::itemType itemType)
{
    return QColor(value(itemType, QStringLiteral("stroke")));
}


QString KBBThemeManager::elementId(const KBBScalableGraphicWidget::itemType itemType)
{
	QString eId;
	
	switch (itemType) {
		case KBBScalableGraphicWidget::background:
            eId = QStringLiteral("background");
			break;
		case KBBScalableGraphicWidget::blackbox:
            eId = QStringLiteral("blackbox");
			break;
		case KBBScalableGraphicWidget::blackboxGrid:
            eId = QStringLiteral("blackbox_grid");
			break;
		case KBBScalableGraphicWidget::cursor:
            eId = QStringLiteral("cursor");
			break;
		case KBBScalableGraphicWidget::informationBackground:
            eId = QStringLiteral("information_background");
			break;
		case KBBScalableGraphicWidget::interactionInfoDeflection:
            eId = QStringLiteral("interaction_info_deflection");
			break;
		case KBBScalableGraphicWidget::interactionInfoHit:
            eId = QStringLiteral("interaction_info_hit");
			break;
		case KBBScalableGraphicWidget::interactionInfoNothing:
            eId = QStringLiteral("interaction_info_nothing");
			break;
		case KBBScalableGraphicWidget::interactionInfoReflection:
            eId = QStringLiteral("interaction_info_reflection");
			break;
		case KBBScalableGraphicWidget::interactionInfoReflectionSym:
            eId = QStringLiteral("interaction_info_reflection_sym");
			break;
		case KBBScalableGraphicWidget::laser0:
            eId = QStringLiteral("laser_0");
			break;
		case KBBScalableGraphicWidget::laser90:
            eId = QStringLiteral("laser_90");
			break;
		case KBBScalableGraphicWidget::laser180:
            eId = QStringLiteral("laser_180");
			break;
		case KBBScalableGraphicWidget::laser270:
            eId = QStringLiteral("laser_270");
			break;
		case KBBScalableGraphicWidget::markerNothing:
            eId = QStringLiteral("marker_nothing");
			break;
		case KBBScalableGraphicWidget::playerBall:
            eId = QStringLiteral("player_ball");
			break;
		case KBBScalableGraphicWidget::playerRay:
            eId = QStringLiteral("player_ray");
			break;
		case KBBScalableGraphicWidget::resultBackground:
            eId = QStringLiteral("result_background");
			break;
		case KBBScalableGraphicWidget::resultBackgroundHighlight:
            eId = QStringLiteral("result_background_highlight");
			break;
		case KBBScalableGraphicWidget::resultHit:
            eId = QStringLiteral("result_hit");
			break;
		case KBBScalableGraphicWidget::resultReflection:
            eId = QStringLiteral("result_reflection");
			break;
		case KBBScalableGraphicWidget::rightPlayerBall:
            eId = QStringLiteral("right_player_ball");
			break;
		case KBBScalableGraphicWidget::solutionBall:
            eId = QStringLiteral("solution_ball");
			break;
		case KBBScalableGraphicWidget::solutionRay:
            eId = QStringLiteral("solution_ray");
			break;
		case KBBScalableGraphicWidget::tutorialMarker:
            eId = QStringLiteral("tutorial_marker");
			break;
		case KBBScalableGraphicWidget::unsureBall:
            eId = QStringLiteral("unsure_ball");
			break;
		case KBBScalableGraphicWidget::wrongPlayerBall:
            eId = QStringLiteral("wrong_player_ball");
			break;
		default:
            eId = QStringLiteral("none");
			break;
	}
	
	return eId;
}


Qt::PenStyle KBBThemeManager::style(const KBBScalableGraphicWidget::itemType itemType)
{
    if (value(itemType, QStringLiteral("stroke-dasharray"))==QLatin1String("none")) {
		return Qt::SolidLine;
	} else
		return Qt::DotLine;
}


QSvgRenderer* KBBThemeManager::svgRenderer()
{
	return &m_svgRenderer;
}


qreal KBBThemeManager::width(const KBBScalableGraphicWidget::itemType itemType)
{
    return value(itemType, QStringLiteral("stroke-width")).toFloat();
}


int KBBThemeManager::zValue(const KBBScalableGraphicWidget::itemType itemType)
{
	return itemType;
}



//
// Private
//

QString KBBThemeManager::value(const KBBScalableGraphicWidget::itemType itemType, const QString &styleElement)
{
	const QString id = elementId(itemType);
    QString style;
    QString v;
	
	QDomNode node = m_root.firstChild();
	while(!node.isNull()) {
        if (node.toElement().attribute(QStringLiteral("id")) == id)
            style = node.toElement().attribute(QStringLiteral("style"));
		node = node.nextSibling();
	}
	
    QStringList styleList = style.split(QLatin1Char(';'));
	for (int i = 0; i < styleList.size(); i++) {
		styleList.replace(i, styleList.at(i).trimmed());
        if (styleList.at(i).startsWith(styleElement + QLatin1Char(':'))) {
			QString s = styleList.at(i);
			v = s.right(s.length()-styleElement.length()-1);
		}
	}
	
	return v;
}
