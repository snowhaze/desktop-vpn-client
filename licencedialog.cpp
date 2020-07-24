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

#include "licencedialog.h"
#include "ui_licencedialog.h"

LicenceDialog::LicenceDialog(QString key, QString text_argument, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenceDialog)
{
    ui->setupUi(this);
    if (key == "license"){
        setWindowTitle(QCoreApplication::translate("dialog: window title license", "License"));
        QFile file(text_argument);
        file.open(QIODevice::ReadOnly);
        QTextStream licence(&file);
        ui->textBrowser_licence->setText(licence.readAll());
    } else if (key == "sgx_response"){
        setWindowTitle(QCoreApplication::translate("dialog: window title SGX response", "SGX verification response"));
        ui->textBrowser_licence->setText(text_argument);
    }

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

LicenceDialog::~LicenceDialog()
{
    delete ui;
}
