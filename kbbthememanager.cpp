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


#include <kfilterdev.h>
#include <ksvgrenderer.h>


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
	QIODevice *f = KFilterDev::device(&svgzFile, QString::fromLatin1("application/x-gzip"), false);
	
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
	return QColor(value(itemType, "stroke"));
}


QString KBBThemeManager::elementId(const KBBScalableGraphicWidget::itemType itemType)
{
	QString eId;
	
	switch (itemType) {
		case KBBScalableGraphicWidget::background:
			eId = "background";
			break;
		case KBBScalableGraphicWidget::blackbox:
			eId = "blackbox";
			break;
		case KBBScalableGraphicWidget::blackboxGrid:
			eId = "blackbox_grid";
			break;
		case KBBScalableGraphicWidget::cursor:
			eId = "cursor";
			break;
		case KBBScalableGraphicWidget::interactionInfoDeflection:
			eId = "interaction_info_deflection";
			break;
		case KBBScalableGraphicWidget::interactionInfoHit:
			eId = "interaction_info_hit";
			break;
		case KBBScalableGraphicWidget::interactionInfoNothing:
			eId = "interaction_info_nothing";
			break;
		case KBBScalableGraphicWidget::interactionInfoReflection:
			eId = "interaction_info_reflection";
			break;
		case KBBScalableGraphicWidget::interactionInfoReflectionSym:
			eId = "interaction_info_reflection_sym";
			break;
		case KBBScalableGraphicWidget::laser0:
			eId = "laser_0";
			break;
		case KBBScalableGraphicWidget::laser90:
			eId = "laser_90";
			break;
		case KBBScalableGraphicWidget::laser180:
			eId = "laser_180";
			break;
		case KBBScalableGraphicWidget::laser270:
			eId = "laser_270";
			break;
		case KBBScalableGraphicWidget::markerNothing:
			eId = "marker_nothing";
			break;
		case KBBScalableGraphicWidget::playerBall:
			eId = "player_ball";
			break;
		case KBBScalableGraphicWidget::playerRay:
			eId = "player_ray";
			break;
		case KBBScalableGraphicWidget::resultBackground:
			eId = "result_background";
			break;
		case KBBScalableGraphicWidget::resultBackgroundHighlight:
			eId = "result_background_highlight";
			break;
		case KBBScalableGraphicWidget::resultHit:
			eId = "result_hit";
			break;
		case KBBScalableGraphicWidget::resultReflection:
			eId = "result_reflection";
			break;
		case KBBScalableGraphicWidget::solutionBall:
			eId = "solution_ball";
			break;
		case KBBScalableGraphicWidget::solutionRay:
			eId = "solution_ray";
			break;
		case KBBScalableGraphicWidget::unsureBall:
			eId = "unsure_ball";
			break;
		case KBBScalableGraphicWidget::wrongPlayerBall:
			eId = "wrong_player_ball";
			break;
		default:
			eId = "none";
			break;
	}
	
	return eId;
}


Qt::PenStyle KBBThemeManager::style(const KBBScalableGraphicWidget::itemType itemType)
{
	if (value(itemType, "stroke-dasharray")=="none") {
		return Qt::SolidLine;
	} else
		return Qt::DotLine;
}


KSvgRenderer* KBBThemeManager::svgRenderer()
{
	return &m_svgRenderer;
}


qreal KBBThemeManager::width(const KBBScalableGraphicWidget::itemType itemType)
{
	return value(itemType, "stroke-width").toFloat();
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
	QString style("");
	QString v("");
	
	QDomNode node = m_root.firstChild();
	while(!node.isNull()) {
		if (node.toElement().attribute("id") == id)
			style = node.toElement().attribute("style");
		node = node.nextSibling();
	}
	
	QStringList styleList = style.split(";");
	for (int i = 0; i < styleList.size(); i++) {
		styleList.replace(i, styleList.at(i).trimmed());
		if (styleList.at(i).startsWith(styleElement + ':')) {
			QString s = styleList.at(i);
			v = s.right(s.length()-styleElement.length()-1);
		}
	}
	
	return v;
}
