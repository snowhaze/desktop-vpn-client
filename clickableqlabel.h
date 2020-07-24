//Copyright (C) 2020  Illotros GmbH

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <https://www.gnu.org/licenses/>.


#ifndef CLICKABLEQLABEL_H
#define CLICKABLEQLABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <QMouseEvent>
#include <QDebug>

class ClickableQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableQLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
        ~ClickableQLabel();
    bool m_bool_on;

    signals:
        void leftClicked();

    protected:
        void mousePressEvent(QMouseEvent* event);

};

#endif // CLICKABLEQLABEL_H
