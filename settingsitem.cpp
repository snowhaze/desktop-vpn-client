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

#include "settingsitem.h"
#include "ui_settingsitem.h"

SettingsItem::SettingsItem(void (*buttonClickedFunction)(),QString settingsName, QString buttonText, bool button_on, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsItem)
{
    ui->setupUi(this);
    if (settingsName == "SettingsTitle") {
        ui->horizontalSpacer_1->changeSize(0,20);
        ui->label_settingsName->setText(QCoreApplication::translate("settingsitem settings title",
                                                                    "<h2 style=\" margin-top:16px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                                                                    "<span style=\" font-size:x-large; font-weight:600; color:#bf9659;\">Settings</span></h2>"));
        ui->label_settingsButton->hide();
    } else {
        ui->label_settingsName->setText(settingsName);
        if (buttonText == "on_off"){
            m_on = QCoreApplication::translate("Setting item: ON of on-off button", "On");
            m_off = QCoreApplication::translate("Setting item: OFF of on-off button", "Off");
            on_button_style_sheet = "font: 8pt; background-color: rgb(191, 150, 89); color: rgb(240, 240, 240); border: 1px solid black; border-radius: 3px 3px; padding: 0px 10px;";
            off_button_style_sheet = "font: 8pt; background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); border: 1px solid black; border-radius: 3px 3px; padding: 0px 10px;";

            if(button_on){
                ui->label_settingsButton->setStyleSheet(on_button_style_sheet);
                ui->label_settingsButton->setText(m_on);
            } else {
                ui->label_settingsButton->setStyleSheet(off_button_style_sheet);
                ui->label_settingsButton->setText(m_off);
            }
            connect(parent, SIGNAL(setOnOffButton(bool)), this, SLOT(setOnOffButton(bool)));
            connect(ui->label_settingsButton, &ClickableQLabel::leftClicked, this, &SettingsItem::onButtonClicked);

        } else {
            ui->label_settingsButton->setText(buttonText);
        }

        connect(ui->label_settingsButton, &ClickableQLabel::leftClicked, this, buttonClickedFunction);
    }
}

void SettingsItem::setOnOffButton(bool on)
{
    if(on){
        ui->label_settingsButton->setStyleSheet(on_button_style_sheet);
        ui->label_settingsButton->setText(m_on);
    } else {
        ui->label_settingsButton->setStyleSheet(off_button_style_sheet);
        ui->label_settingsButton->setText(m_off);
    }
}

SettingsItem::~SettingsItem()
{
    delete ui;
}

