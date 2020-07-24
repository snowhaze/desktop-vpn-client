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

#ifndef LICENCESWIDGET_H
#define LICENCESWIDGET_H

#include <QWidget>
#include <QCoreApplication>
#include "licencedialog.h"
#include "clickableqlabel.h"

namespace Ui {
class licencesWidget;
}

class licencesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit licencesWidget(QWidget *parent = nullptr);
    ~licencesWidget();

public slots:
    void on_showQtLicenseClicked();
    void on_showOpenVPNLicenseClicked();
    void on_showLibsodiumLicenseClicked();
    void on_showOpenSSLLicenseClicked();
    void on_showPythonLicenceClicked();
    void on_showQtKeychainLicenseClicked();

private:
    Ui::licencesWidget *ui;
};

#endif // LICENCESWIDGET_H
