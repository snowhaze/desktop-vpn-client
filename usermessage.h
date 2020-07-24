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

#ifndef USERMESSAGE_H
#define USERMESSAGE_H

#include <QObject>
#include <QMessageBox>
#include <QCoreApplication>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QCheckBox>
#include <QDebug>

#define SNOWHAZE_WEBSITE_URL "https://snowhaze.com/"
#define SNOWHAZE_DASHBOARD_URL "https://dashboard.snowhaze.com/"
#define OPENVPN_DOWNLOADS_URL "https://openvpn.net/community-downloads/"

class UserMessage:  public QObject
{
    Q_OBJECT
public:
    UserMessage();

signals:
    void goToLoginPage();
    void requestLogout();
public slots:
    void wrongCredentials();
    void networkError(int);
    void QKeychainError();
    void userMasterSecretError(int status_code);

    void invalidSubscription();
    void publicKeyNotRegistered();
    void processError(QString process);
    void vpnProcessTimeoutError();
    void loadVPNProfilesFailed();
    void logoutWarning();
    static void encryptionFailure(QString error);
    void overusedTokensWarning();
    bool missingDependecyWarning(QString dependency);
    void memLockErrorWarning();


};

#endif // USERMESSAGE_H
