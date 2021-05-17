/*
    SPDX-FileCopyrightText: 2006, 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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


	public Q_SLOTS:
		void preview(int balls, int columns, int rows);


	protected:
		void drawBackground(QPainter* painter, const QRectF&) override;


	private:
		QGraphicsView* m_view;
		QList<KBBGraphicsItem*> m_balls;
		KBBGraphicsItemBlackBox* m_blackbox;
		KBBThemeManager* m_themeManager;


	private Q_SLOTS:
		void resizeEvent(QResizeEvent*) override;
};


#endif //KSIMILILEVELCONFIGURATIONPREVIEW_H
