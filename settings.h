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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include "userdata.h"

class Settings: public QObject
{
    Q_OBJECT
public:
    explicit Settings(UserData &user, QObject *parent = nullptr);

    QDateTime getVPNProfileTimeStamp();
    QStringList getVPNIds();
    QByteArray getEncryptedTokenMessage();

    QSettings m_settings;

signals:
    void masterSecretFinished();

public slots:
    void updateBoolMasterSecret(bool bool_master_secret);
    void setEncryptedTokensMessage(QByteArray encrypted_tokens_message);
    void setVPNProfileTimeStamp(QDateTime timestamp);
    void setVPNIds(QStringList list_vpn_ids);
    void setLastVPNProfileUpdate(QDateTime last_vpn_profile_update);
    void setdaysNextProfileUpdate(int days);
    void setCheckDependencies(bool);
    void updateIpIdMap(QMap<QString,QVariant> id_ip_map, QMap<QString,QVariant> ip_id_map, QMap<QString,QVariant> host_id_map, QMap<QString,QVariant> id_ipv4_map);
    void getIpIdMap(UserData &user);
    void setShowIp(bool show_ip);
    void clearSettings();


private:
    QDateTime m_default_vpn_profile_timestamp;
    bool m_default_bool_master_secret;
    bool m_default_bool_vpn_profiles;

};

#endif // SETTINGS_H
