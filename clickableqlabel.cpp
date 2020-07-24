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

#include "clickableqlabel.h"

ClickableQLabel::ClickableQLabel(QWidget* parent, Qt::WindowFlags )
    : QLabel(parent) {
    m_bool_on = false;
    this->setCursor(QCursor(Qt::PointingHandCursor));

}

ClickableQLabel::~ClickableQLabel()
{

}

void ClickableQLabel::mousePressEvent(QMouseEvent *mouse_event)
{
    if(mouse_event->button() == Qt::LeftButton) emit leftClicked();
}
