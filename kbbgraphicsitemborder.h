/*
    KBlackBox - A simple game inspired by an emacs module

    SPDX-FileCopyrightText: 1999-2000 Robert Cimrman <cimrman3@students.zcu.cz>
    SPDX-FileCopyrightText: 2007 Nicolas Roffet <nicolas-kde@roffet.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/



#ifndef KBBGRAPHICSITEMBORDER_H
#define KBBGRAPHICSITEMBORDER_H



/**
 * @brief Any border element of the scalable graphic widget
 */
class KBBGraphicsItemBorder
{
	public:
		/**
		 * @brief Constructor
		 */
		KBBGraphicsItemBorder(int borderPosition, int columns, int rows, float offset);
		
		void setSize(int borderPosition, int columns, int rows);


	protected:
		void centerCoordinate(int borderPosition, float &centerX, float &centerY, float offset);
		float centerX() const;
		float centerY() const;
		int rotationAngle();
		void setBorderPosition(int borderPosition);


		int m_borderPosition;
		float m_centerX;
		float m_centerY;
		int m_columns;
		float m_offset;
		int m_rows;
};

#endif // KBBGRAPHICSITEMBORDER_H
