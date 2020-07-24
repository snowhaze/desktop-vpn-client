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

#include "apiv3.h"
#include <QJsonDocument>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QSslSocket>

#define API_DOMAIN  "api.snowhaze.com"

APIV3::APIV3(QObject *parent) : QObject(parent)
{
    m_api_url = QUrl("https://" API_DOMAIN);
}

void APIV3::mastersecretRequest(const QHash<QString, QString> &parameters)
{
    network_access_manager.clearConnectionCache();

    QUrlQuery url_parameters;
    url_parameters.addQueryItem("v","3");
    for (auto param = parameters.begin(); param != parameters.end(); param++) {
        url_parameters.addQueryItem(param.key(), param.value());
    }

    QNetworkRequest request = generateRequest(m_api_url);

    QNetworkReply *reply = network_access_manager.post(request, url_parameters.toString().toUtf8());
    connect(reply, &QNetworkReply::finished, this, &APIV3::mastersecretHandleReply);
}

void APIV3::mastersecretHandleReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    const auto server_reply = reply->readAll();
    QJsonDocument json_reply = QJsonDocument::fromJson(server_reply);
    int status_code = reply -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    delete reply;

    if(status_code==200){
        const QByteArray master_secret = json_reply.object().value("master_secret").toString().toUtf8();
        emit serverResponseMasterSecretReady(QByteArray::fromBase64(master_secret));
    } else if(status_code==404){
        emit wrongCredentials();
    } else {
        emit networkError(status_code);
    }
}

void APIV3::tokensRequest(UserData &user)
{
    serverKeyRequest(user, false, false);
}

void APIV3::serverKeyRequest(UserData &user, bool retry, bool revalidate)
{
    network_access_manager.clearConnectionCache();

    QUrlQuery url_parameters;
    url_parameters.addQueryItem("v", "3");
    url_parameters.addQueryItem("action", "get_server_token");

    QNetworkRequest request = generateRequest(m_api_url);

    QNetworkReply *reply = network_access_manager.post(request, url_parameters.toString().toUtf8());
    connect(reply, &QNetworkReply::finished, this, [this, &user, retry, revalidate](){APIV3::serverKeyHandleReply(user, retry, revalidate);});
}

void APIV3::serverKeyHandleReply(UserData &user, bool retry, bool revalidate)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    const auto server_reply = reply->readAll();
    QJsonDocument json_reply = QJsonDocument::fromJson(server_reply);
    int status_code = reply -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    delete reply;

    if(status_code==200){
        const QJsonValue server_token = json_reply.object().value("server_token");
        const QByteArray server_key = QByteArray::fromBase64(json_reply.object().value("server_key").toString().toUtf8());
        if (revalidate) {
            initiateRevalidate(user, server_token, server_key);
        } else {
            initiateTokenFetch(user, retry, server_token, server_key);
        }
    } else {
        emit networkError(status_code);
    }
}

void APIV3::initiateTokenFetch(UserData &user, bool retry, QJsonValue server_token, QByteArray server_key)
{
    network_access_manager.clearConnectionCache();

    // get app
    QJsonObject app_parameters;
    app_parameters.insert("v", 3);
    app_parameters.insert("action", "tokens");
    app_parameters.insert("server_token", server_token);
    app_parameters.insert("pseudonym", QString::fromStdString(user.getPseudonym().toBase64().toStdString()));

    //ZKACrypto::getAPP()
    auto private_key = user.getPrivateKey();
    auto app = ZKACrypto::createAPP(app_parameters, server_key, private_key);

    QByteArray cipher_message_array(reinterpret_cast<char const*>(app.data()), static_cast<int>(app.size()));
    QByteArray cipher_message_base64 = cipher_message_array.toBase64();

    const auto public_key = user.getPublicKey();

    QByteArray public_key_array(reinterpret_cast<char const*>(public_key.data()), public_key.size());
    QByteArray public_key_base64 = public_key_array.toBase64();

    QUrlQuery url_parameters;
    url_parameters.addQueryItem("v", "3");
    url_parameters.addQueryItem("key", url_encode(public_key_base64).data());
    url_parameters.addQueryItem("app", url_encode(cipher_message_base64).data());

    QNetworkRequest request = generateRequest(m_api_url);

    QNetworkReply *reply = network_access_manager.post(request, url_parameters.toString().toUtf8());
    reply->deleteLater();
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, this, [this, &user, retry](){APIV3::tokensHandleReply(user, retry);});
    loop.exec();
}

void APIV3::initiateRevalidate(UserData &user, QJsonValue server_token, QByteArray server_key)
{
    network_access_manager.clearConnectionCache();

    // get app
    QJsonObject app_parameters;
    app_parameters.insert("v", 3);
    app_parameters.insert("action", "payment");
    app_parameters.insert("type", "applestore");
    app_parameters.insert("paytype", "revalidate_receipt");
    app_parameters.insert("server_token", server_token);
    app_parameters.insert("pseudonym", QString::fromStdString(user.getPseudonym().toBase64().toStdString()));
    app_parameters.insert("receipt_key", QString::fromStdString(user.getReceiptKey().toBase64().toStdString()));

    //ZKACrypto::getAPP()
    auto private_key = user.getPrivateKey();
    auto app = ZKACrypto::createAPP(app_parameters, server_key, private_key);

    QByteArray cipher_message_array(reinterpret_cast<char const*>(app.data()), static_cast<int>(app.size()));
    QByteArray cipher_message_base64 = cipher_message_array.toBase64();

    const auto public_key = user.getPublicKey();

    QByteArray public_key_array(reinterpret_cast<char const*>(public_key.data()), public_key.size());
    QByteArray public_key_base64 = public_key_array.toBase64();

    QUrlQuery url_parameters;
    url_parameters.addQueryItem("v", "3");
    url_parameters.addQueryItem("key", url_encode(public_key_base64).data());
    url_parameters.addQueryItem("app", url_encode(cipher_message_base64).data());

    QNetworkRequest request = generateRequest(m_api_url);

    QNetworkReply *reply = network_access_manager.post(request, url_parameters.toString().toUtf8());
    reply->deleteLater();
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, this, [this, &user](){APIV3::revalidateHandleReply(user);});
    loop.exec();
}

void APIV3::tokensHandleReply(UserData &user, bool retry)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    const auto server_reply = reply->readAll();
    QJsonDocument json_reply = QJsonDocument::fromJson(server_reply);
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    delete reply;

    if(status_code==200){
        const QByteArray encrypted_tokens_message = QByteArray::fromBase64(json_reply.object().value("tokens").toString().toUtf8());
        const QJsonValue separator_json = json_reply.object().value("separator");
        const QJsonValue vpn_time_stamp = json_reply.object().value("expiration");
        QDateTime timestamp = QDateTime::fromSecsSinceEpoch(static_cast<long long>(vpn_time_stamp.toDouble()), Qt::OffsetFromUTC);
        QByteArray separator = separator_json.toString().toUtf8();

        if(UserData::checkVPNProfileActive(timestamp)){
            emit serverResponseTokensReady(encrypted_tokens_message, separator);
        } else {
            emit invalidSubscription();
        }
    } else if(status_code==402){
        emit invalidSubscription();
    } else if(status_code==403){
        emit publicKeyNotRegistered();
    } else if(status_code==423 && !retry){
        serverKeyRequest(user, false, true);
    } else {
        emit networkError(status_code);
    }
}

void APIV3::revalidateHandleReply(UserData &user)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    const auto server_reply = reply->readAll();
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    delete reply;

    if(status_code==200 || status_code==202){
        serverKeyRequest(user, true, false);
    } else {
        emit networkError(status_code);
    }
}

void APIV3::VPNProfileRequest(UserData &user)
{
    network_access_manager.clearConnectionCache();

    QUrlQuery url_parameters;
    url_parameters.addQueryItem("v","3");
    url_parameters.addQueryItem("action", "vpn");
    url_parameters.addQueryItem("t", url_encode( user.getToken()) );

    QNetworkRequest request = generateRequest(m_api_url);
    QNetworkReply *reply = network_access_manager.post(request, url_parameters.toString().toUtf8());
    reply->deleteLater();

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, this, &APIV3::VPNProfileHandleReply);
    loop.exec();
}

void APIV3::VPNProfileHandleReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    const auto server_reply = reply->readAll();
    QJsonDocument json_reply = QJsonDocument::fromJson(server_reply);
    int status_code = reply -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    delete reply;

    if(status_code==200){
        QJsonValue ovpn_object = json_reply.object().value("ovpn");
        emit openVPNObjectReady(ovpn_object);
    } else if(status_code==429){
        emit repeateVPNProfileRequest();
    } else {
        emit networkError(status_code);
    }
}

void APIV3::getPublicIP(bool check_ip, QString host)
{
    if (check_ip){
        network_access_manager.clearConnectionCache();

        QUrlQuery url_parameters;
        url_parameters.addQueryItem("v","3");
        url_parameters.addQueryItem("action","get_client_ip");

        QNetworkRequest request = generateRequest(QUrl("https://" + host + "." API_DOMAIN));
        request.setTransferTimeout(1000);

        QNetworkReply *reply = network_access_manager.post(request, url_parameters.toString().toUtf8());
        connect(reply, &QNetworkReply::finished, this, [this, host](){APIV3::getPublicIpHandleReply(host);});
    } else {
        emit updateIp(host, "-");
    }

}

void APIV3::getPublicIpHandleReply(QString host)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    const auto server_reply = reply->readAll();
    QJsonDocument json_reply = QJsonDocument::fromJson(server_reply);
    int status_code = reply -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    delete reply;
    if(status_code==200){
        QString ip = json_reply.object().value("ip").toString();
        emit updateIp(host, ip);
    } else {
        emit updateIp(host, QCoreApplication::translate("PublicIpHandleReply no IP resieved", "Not detected"));
    }
}


QNetworkRequest APIV3::generateRequest(QUrl url)
{
    QNetworkRequest request(url);
    QSslConfiguration ssl_config;
    ssl_config.setProtocol(QSsl::TlsV1_3OrLater);
    request.setSslConfiguration(ssl_config);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setTransferTimeout(7000);

    return request;
}

