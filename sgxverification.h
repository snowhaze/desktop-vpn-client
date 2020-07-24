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

#ifndef SGXVERIFICATION_H
#define SGXVERIFICATION_H

#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QProcessEnvironment>
#include <QtDebug>
#include "licencedialog.h"


class sgxVerification: public QObject
{
    Q_OBJECT

public:
    sgxVerification();

signals:
    void processError(QString process);

public slots:
    void verifyTokenResponse(QByteArray encrypted_token_message);
};

#endif // SGXVERIFICATION_H
