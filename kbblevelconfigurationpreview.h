/*
Copyright (c) 2006, 2007, Nicolas Roffet, <nicolas-kde@roffet.com>

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU Library General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*/

#ifndef KBBLEVELCONFIGURATIONPREVIEW_H
#define KBBLEVELCONFIGURATIONPREVIEW_H



#include <QGraphicsView>
#include <QList>


class KBBGraphicsItem;
class KBBGraphicsItemBlackBox;
class KBBThemeManager;



/**
 * @brief Widget with the preview of the field in the configuration widget
 */
class KBBLevelConfigurationPreview : public QGraphicsView
{
	Q_OBJECT

	public:
		KBBLevelConfigurationPreview(QWidget *parent, KBBThemeManager* themeManager);


	public slots:
		void preview(int balls, int columns, int rows);


	protected:
		void drawBackground(QPainter* painter, const QRectF&) Q_DECL_OVERRIDE;


	private:
		QGraphicsView* m_view;
		QList<KBBGraphicsItem*> m_balls;
		KBBGraphicsItemBlackBox* m_blackbox;
		KBBThemeManager* m_themeManager;


	private slots:
		void resizeEvent(QResizeEvent*) Q_DECL_OVERRIDE;
};


#endif //KSIMILILEVELCONFIGURATIONPREVIEW_H
