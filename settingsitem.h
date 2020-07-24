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

#ifndef SETTINGSITEM_H
#define SETTINGSITEM_H

#include <QWidget>

namespace Ui {
class SettingsItem;
}

class SettingsItem : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsItem(void (*buttonClickedFunction)(),QString settingsName, QString buttonText = "", bool button_on = false, QWidget *parent = nullptr);
    ~SettingsItem();
    Ui::SettingsItem *ui;

    QString on_button_style_sheet;
    QString off_button_style_sheet;
    QString m_on;
    QString m_off;

signals:
    void onButtonClicked();

public slots:
    void setOnOffButton(bool on);

private:

};

#endif // SETTINGSITEM_H
