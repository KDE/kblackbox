/*
Copyright (c) 2006, 2007, Nicolas Roffet, <nicolas-kde@roffet.com>

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU Library General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*/

#include "kbblevelconfigurationwidget.h"



#include <QGridLayout>

#include <klocale.h>
#include <KNumInput>

#include "kbblevelconfigurationpreview.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



KBBLevelConfigurationWidget::KBBLevelConfigurationWidget(QWidget *parent, int c, int r, int b, KBBThemeManager* themeManager) : QWidget(parent)
{
	QGridLayout *l = new QGridLayout(this);

	kcfg_balls = new KIntSpinBox(this);
	kcfg_balls->setObjectName("kcfg_balls");
	l->addWidget(kcfg_balls, 0, 0, 1, 2);
	kcfg_balls->setMinimum(1);
	kcfg_balls->setValue(b);
	kcfg_balls->setSuffix(ki18ncp("A number between 1 and 99 is displayed in front of it.", " ball", " balls"));
	connect(kcfg_balls, SIGNAL(valueChanged(int)), this, SLOT(boxSizeChanged()));

	kcfg_columns = new KIntSpinBox(this);
	kcfg_columns->setObjectName("kcfg_columns");
	l->addWidget(kcfg_columns, 1, 1);
	kcfg_columns->setMinimum(2);
	kcfg_columns->setMaximum(30);
	kcfg_columns->setValue(c);
	kcfg_columns->setSuffix(ki18ncp("A number between 2 and 30 is displayed in front of it.", " column", " columns"));
	connect(kcfg_columns, SIGNAL(valueChanged(int)), this, SLOT(boxSizeChanged()));

	kcfg_rows = new KIntSpinBox(this);
	kcfg_rows->setObjectName("kcfg_rows");
	l->addWidget(kcfg_rows, 2, 0);
	kcfg_rows->setMinimum(2);
	kcfg_rows->setMaximum(30);
	kcfg_rows->setValue(r);
	kcfg_rows->setSuffix(ki18ncp("A number between 2 and 30 is displayed in front of it.", " row", " rows"));
	connect(kcfg_rows, SIGNAL(valueChanged(int)), this, SLOT(boxSizeChanged()));

	m_view = new KBBLevelConfigurationPreview(this, themeManager);
	l->addWidget(m_view, 2, 1);

	boxSizeChanged();
}


int KBBLevelConfigurationWidget::balls() const
{
	return kcfg_balls->value();
}


int KBBLevelConfigurationWidget::columns() const
{
	return kcfg_columns->value();
}


int KBBLevelConfigurationWidget::rows() const
{
	return kcfg_rows->value();
}


void KBBLevelConfigurationWidget::boxSizeChanged()
{
	kcfg_balls->setMaximum(qMin(99, columns()*rows() - 1));
	m_view->preview(balls(), columns(), rows());
}


#include "kbblevelconfigurationwidget.moc"
