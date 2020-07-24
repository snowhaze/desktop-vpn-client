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

#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>
#include <array>
#include <sodium.h>
#include <QJsonDocument>
#include <QJsonValue>
#include <QDateTime>
#include <QEventLoop>
#include <QDir>
#include <QStandardPaths>
#include <QJsonObject>
#include <QProcess>
#include <QElapsedTimer>
#include <QLocale>
#include <QHostInfo>
#include <QtConcurrent>
#include <QFuture>
#include <QThread>
#include <QMutex>
#include <QFutureSynchronizer>
#include "zkacrypto.h"
#include "keychain.h"
#include "usermessage.h"
#include "urlencoding.h"
#include "utils.h"
#include "vpnitem.h"


class UserData: public QObject
{
    Q_OBJECT

public:
    explicit UserData(QObject *parent = nullptr);


    ZKACrypto::public_key_t getPublicKey();
    ZKACrypto::private_key_t getPrivateKey();
    ZKACrypto::password_hash_t getPasswordHash();
    void setPasswordHash(const ZKACrypto::password_hash_t &p_h);
    void setBoolMasterSecret(bool &bool_master_secret);
    static bool checkVPNProfileActive(QDateTime &vpn_profile_time_stamp);
    bool checkProfileUpdateOverdue();
    void setMasterSecret(const QString &master_secret_qba);
    void loadMasterSecret();
    QByteArray getPseudonym();
    QByteArray getReceiptKey();
    QByteArray getToken();
    bool loadVPNProfiles(QStringList list_vpn_ids);
    void clearLocalVPNData();
    void getPing(int index);


    bool m_public_private_key_set;
    bool m_password_hash_set;
    bool m_bool_master_secret_set;
    bool m_bool_VPN_profiles_active;
    bool m_bool_subscription_status_set;

    QList<QByteArray> m_tokens_list;

    struct ovpnProfile {
        QString id;
        QString names_path;
        QString name;
        QString profile_path;
        QString flag_path;
        QDateTime expiration;
        QString host;
        QString ping;
        vpnItem *vpn_widget_item;

    };

    QString vpn_profiles_path;
    QString vpn_data_path;
    QList<ovpnProfile> list_ovpnProfiles;
    QDateTime m_earliest_vpn_expiration;
    QStringList m_list_vpn_ids;
    QDateTime m_last_vpn_profile_update;
    int m_settings_days_next_profile_update;

    QString m_public_ipv4;
    QString m_public_ipv6;

    bool m_check_dependencies;

    QMap<QString,QVariant> m_host_id_map;
    QMap<QString,QVariant> m_ip_id_map;
    QMultiMap<QString,QVariant> m_id_ip_map;
    QMap<QString,QVariant> m_id_ipv4_map;


    bool m_show_ip;

    QMutex mutex;


signals:
    void goToLogingPage();
    void QKeychainError();
    void QKeychainMasterSecretStored(bool bool_master_secret);
    void userMasterSecretError(int status_code);
    void setEncryptedTokensMessage(QByteArray encrypted_tokens_message);
    void tokensListSet();
    void clearSettingsRequest();
    void setVPNProfileTimeStamp(QDateTime timestamp);
    void setLastVPNProfileUpdate(QDateTime last_vpn_profile_update);
    void setVPNIds(QStringList list_vpn_ids);
    void vpnDataReady();
    void loadVPNProfilesFailed();
    void IpUpdated(bool, QString, QString);
    void overusedTokensWarning();
    void settingsUpdateIpIdMap(QMap<QString,QVariant> id_ip_map, QMap<QString,QVariant> ip_id_map, QMap<QString,QVariant> host_id_map, QMap<QString,QVariant> id_ipv4_map);
    void resultActiveConnection(bool active_connection, QString id);
    void setPingFinished();
    void memLockErrorWarning();

public slots:
    void setMastersecretFromCredentials(QByteArray m_s);
    void setTokens(QByteArray tokens, QByteArray separator);
    void setVPNProfiles(QJsonValue ovpn_object);
    void updateIp(QString host, QString new_ip="");
    void setPing();
    void lookedUpHost(const QHostInfo &host);

    void logout();


private:

    ZKACrypto::public_key_t m_public_key;
    ZKACrypto::private_key_t m_private_key;
    ZKACrypto::master_secret_t m_master_secret;
    ZKACrypto::password_hash_t m_password_hash;

    void storeMasterSecret();

};

#endif // USERDATA_H
