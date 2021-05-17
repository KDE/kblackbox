/*
    SPDX-FileCopyrightText: 2006, 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KBBLEVELCONFIGURATIONWIDGET_H
#define KBBLEVELCONFIGURATIONWIDGET_H



#include <QWidget>


class KBBLevelConfigurationPreview;
class KBBThemeManager;
class KPluralHandlingSpinBox;


/**
 * @brief Widget to configure the level difficulty
 */
class KBBLevelConfigurationWidget : public QWidget
{
	Q_OBJECT

	public:
		KBBLevelConfigurationWidget(QWidget *parent, int c, int r, int b, KBBThemeManager* themeManager);

		int balls() const;
		int columns() const;
		int rows() const;

	private Q_SLOTS:
		void boxSizeChanged();

	private:
		KPluralHandlingSpinBox* kcfg_balls;
		KPluralHandlingSpinBox* kcfg_columns;
		KPluralHandlingSpinBox* kcfg_rows;

		KBBLevelConfigurationPreview* m_view;
};


#endif //KSIMILILEVELCONFIGURATIONWIDGET_H
