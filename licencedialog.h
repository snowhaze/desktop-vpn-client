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

#ifndef LICENCEDIALOG_H
#define LICENCEDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class LicenceDialog;
}

class LicenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LicenceDialog(QString key, QString text_argument, QWidget *parent = nullptr);
    ~LicenceDialog();

private:
    Ui::LicenceDialog *ui;

};

#endif // LICENCEDIALOG_H
