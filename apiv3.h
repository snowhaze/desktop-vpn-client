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

#ifndef APIV3_H
#define APIV3_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <qtimer.h>
#include "userdata.h"
#include "zkacrypto.h"
#include "urlencoding.h"


class APIV3: public QObject
{
    Q_OBJECT

public:

    explicit APIV3(QObject *parent = nullptr);

    void mastersecretRequest(const QHash<QString, QString> &parameters);
    void VPNProfileRequest(UserData &user);


signals:
    void serverResponseMasterSecretReady(QByteArray mastersecret);
    void wrongCredentials();
    void networkError(int);
    void serverResponseTokensReady(QByteArray tokens, QByteArray separator);
    void invalidSubscription();
    void publicKeyNotRegistered();

    void repeateVPNProfileRequest();
    void openVPNObjectReady(QJsonValue ovpn_object);

    void updateIp(QString host, QString ip);

public slots:
    void getPublicIP(bool check_ip, QString host);
    void tokensRequest(UserData &user);


private:
    QNetworkAccessManager network_access_manager;
    QNetworkRequest generateRequest(QUrl url);

    QUrl m_api_url;

private slots:
    void mastersecretHandleReply();
    void tokensHandleReply(UserData &user, bool retry);
    void revalidateHandleReply(UserData &user);
    void serverKeyRequest(UserData &user, bool retry, bool revalidate);
    void serverKeyHandleReply(UserData &user, bool retry, bool revalidate);
    void VPNProfileHandleReply();
    void getPublicIpHandleReply(QString host);
    void initiateTokenFetch(UserData &user, bool retry, QJsonValue server_token, QByteArray server_key);
    void initiateRevalidate(UserData &user, QJsonValue server_token, QByteArray server_key);

};

#endif // APIV3_H
