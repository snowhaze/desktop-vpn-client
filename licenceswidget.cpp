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

#include "licenceswidget.h"
#include "ui_licenceswidget.h"

licencesWidget::licencesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::licencesWidget)
{
    ui->setupUi(this);

    connect(ui->label_showQtLicense, &ClickableQLabel::leftClicked, this, &licencesWidget::on_showQtLicenseClicked);
    connect(ui->label_showOvpnLicense, &ClickableQLabel::leftClicked, this, &licencesWidget::on_showOpenVPNLicenseClicked);
    connect(ui->label_showLibsodiumLicense, &ClickableQLabel::leftClicked, this, &licencesWidget::on_showLibsodiumLicenseClicked);
    connect(ui->label_showOpenSSLLicense, &ClickableQLabel::leftClicked, this, &licencesWidget::on_showOpenSSLLicenseClicked);
    connect(ui->label_showPythonLicense, &ClickableQLabel::leftClicked, this, &licencesWidget::on_showPythonLicenceClicked);
    connect(ui->label_showQtKeychainLicense, &ClickableQLabel::leftClicked, this, &licencesWidget::on_showQtKeychainLicenseClicked);


    ui->label_SnowHazeVPNVersion->setText(QCoreApplication::translate("licence widget label_SnowHazeVPNVersion text","SnowHaze VPN version ") +
                                          QString::fromStdString(std::to_string(VERSION_MAJOR)) + "." +
                                          QString::fromStdString(std::to_string(VERSION_MINOR)) + "." +
                                          QString::fromStdString(std::to_string(VERSION_BUILD)));
}

licencesWidget::~licencesWidget()
{
    delete ui;
}

void licencesWidget::on_showQtLicenseClicked()
{
    LicenceDialog *dialog = new LicenceDialog("license", ":/licenses/QtLICENSE", this);
    dialog->show();
}

void licencesWidget::on_showOpenVPNLicenseClicked()
{
    LicenceDialog *dialog = new LicenceDialog("license", QCoreApplication::applicationDirPath() + "/binaries/ovpn/license.txt", this);
    dialog->show();
}

void licencesWidget::on_showLibsodiumLicenseClicked()
{
    LicenceDialog *dialog = new LicenceDialog("license", ":/licenses/LibsodiumLicense", this);
    dialog->show();
}

void licencesWidget::on_showOpenSSLLicenseClicked()
{
    LicenceDialog *dialog = new LicenceDialog("license", QCoreApplication::applicationDirPath() + "/binaries/ssl/LICENSE.txt", this);
    dialog->show();
}

void licencesWidget::on_showPythonLicenceClicked()
{
    LicenceDialog *dialog = new LicenceDialog("license", QCoreApplication::applicationDirPath() + "/binaries/python/LICENSE.txt", this);
    dialog->show();
}

void licencesWidget::on_showQtKeychainLicenseClicked()
{
    LicenceDialog *dialog = new LicenceDialog("license", ":/licenses/QtKeychainLicense", this);
    dialog->show();
}
