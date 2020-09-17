/*
Copyright (c) 2006, 2007, Nicolas Roffet, <nicolas-kde@roffet.com>

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU Library General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
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
