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



#include <QColor>
#include <QDomDocument>
#include <QFile>
#include <QString>
#include <QStringList>


#include <kdebug.h>
#include <kfilterdev.h>


#include "kbbscalablegraphicwidgetnonsvgitems.h"



//
// Constructor / Destructor
//

KBBScalableGraphicWidgetNonSvgItems::KBBScalableGraphicWidgetNonSvgItems(QString svgzFileName)
{
	QFile svgzFile(svgzFileName);
	QIODevice *f = KFilterDev::device( &svgzFile, QString::fromLatin1("application/x-gzip"), false);
	
	if (!f)
		return;
	
	QDomDocument doc;
	if (doc.setContent(f,true)) {
		m_root = doc.documentElement();
	}
	
	delete f;
}



//
// Public
//

QColor KBBScalableGraphicWidgetNonSvgItems::color(QString elementId)
{
	return QColor(value(elementId, "stroke"));
}


Qt::PenStyle KBBScalableGraphicWidgetNonSvgItems::style(QString elementId)
{
	if (value(elementId, "stroke-dasharray")=="none") {
		return Qt::SolidLine;
	} else
		return Qt::DotLine;
}


qreal KBBScalableGraphicWidgetNonSvgItems::width(QString elementId)
{
	return value(elementId, "stroke-width").toFloat();
}



//
// Private
//


QString KBBScalableGraphicWidgetNonSvgItems::value(QString elementId, QString styleElement)
{
	QString style("");
	QString v("");
	
	QDomNode node = m_root.firstChild();
	while(!node.isNull()) {
		if (node.toElement().attribute("id") == elementId)
			style = node.toElement().attribute("style");
		node = node.nextSibling();
	}
	
	QStringList styleList = style.split(";");
	for (int i = 0; i < styleList.size(); i++) {
		styleList.replace(i, styleList.at(i).trimmed());
		if (styleList.at(i).startsWith(styleElement+":")) {
			QString s = styleList.at(i);
			v = s.right(s.length()-styleElement.length()-1);
		}
	}
	
	return v;
}
