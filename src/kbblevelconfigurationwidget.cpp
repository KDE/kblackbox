/*
    SPDX-FileCopyrightText: 2006, 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kbblevelconfigurationwidget.h"



#include <QGridLayout>

#include <KLocalizedString>
#include <KPluralHandlingSpinBox>

#include "kbblevelconfigurationpreview.h"
#include "kbbscalablegraphicwidget.h"
#include "kbbthememanager.h"



KBBLevelConfigurationWidget::KBBLevelConfigurationWidget(QWidget *parent, int c, int r, int b, KBBThemeManager* themeManager) : QWidget(parent)
{
	QGridLayout *l = new QGridLayout(this);

	kcfg_balls = new KPluralHandlingSpinBox(this);
	kcfg_balls->setObjectName( QStringLiteral("kcfg_balls" ));
	l->addWidget(kcfg_balls, 0, 0, 1, 2);
	kcfg_balls->setMinimum(1);
	kcfg_balls->setValue(b);
	kcfg_balls->setSuffix(ki18ncp("@item:valuesuffix to a number between 1 and 99", " ball", " balls"));
	connect(kcfg_balls, static_cast<void (KPluralHandlingSpinBox::*)(int)>(&KPluralHandlingSpinBox::valueChanged), this, &KBBLevelConfigurationWidget::boxSizeChanged);

	kcfg_columns = new KPluralHandlingSpinBox(this);
	kcfg_columns->setObjectName( QStringLiteral("kcfg_columns" ));
	l->addWidget(kcfg_columns, 1, 1);
	kcfg_columns->setMinimum(2);
	kcfg_columns->setMaximum(30);
	kcfg_columns->setValue(c);
	kcfg_columns->setSuffix(ki18ncp("@item:valuesuffix to a number between 2 and 30", " column", " columns"));
	connect(kcfg_columns, static_cast<void (KPluralHandlingSpinBox::*)(int)>(&KPluralHandlingSpinBox::valueChanged), this, &KBBLevelConfigurationWidget::boxSizeChanged);

	kcfg_rows = new KPluralHandlingSpinBox(this);
	kcfg_rows->setObjectName( QStringLiteral("kcfg_rows" ));
	l->addWidget(kcfg_rows, 2, 0);
	kcfg_rows->setMinimum(2);
	kcfg_rows->setMaximum(30);
	kcfg_rows->setValue(r);
	kcfg_rows->setSuffix(ki18ncp("@item:valuesuffix to a number between 2 and 30", " row", " rows"));
	connect(kcfg_rows, static_cast<void (KPluralHandlingSpinBox::*)(int)>(&KPluralHandlingSpinBox::valueChanged), this, &KBBLevelConfigurationWidget::boxSizeChanged);

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

#include "moc_kbblevelconfigurationwidget.cpp"
