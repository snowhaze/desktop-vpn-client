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

#include "settings.h"

Settings::Settings(UserData &user, QObject *parent) : QObject(parent)
{
    m_default_vpn_profile_timestamp.setDate(QDate(2000,1,1));
    m_default_bool_master_secret = false;
    m_default_bool_vpn_profiles = false;

    user.m_bool_master_secret_set = m_settings.value("userData/boolMasterSecret", m_default_bool_master_secret).toBool();
    user.m_last_vpn_profile_update = m_settings.value("userData/LastVPNProfileUpdate", m_default_vpn_profile_timestamp).toDateTime();
    user.m_settings_days_next_profile_update = m_settings.value("userData/daysNextProfileUpdate", 1).toInt();
    user.m_check_dependencies = m_settings.value("userData/checkDependencies", true).toBool();
    user.m_id_ip_map = m_settings.value("userData/id_ip_map").toMap();
    user.m_ip_id_map = m_settings.value("userData/ip_id_map").toMap();
    user.m_host_id_map = m_settings.value("userData/host_id_map").toMap();
    user.m_id_ipv4_map = m_settings.value("userData/id_ipv4_map").toMap();
    user.m_show_ip = m_settings.value("userData/show_ip", true).toBool();
}

QDateTime Settings::getVPNProfileTimeStamp()
{
    return m_settings.value("userData/VPNProfileTimeStamp", m_default_vpn_profile_timestamp).toDateTime();
}

QStringList Settings::getVPNIds()
{
    QStringList ids_list =  m_settings.value("userData/VPNProfileIds").toStringList();
    return ids_list;
}

QByteArray Settings::getEncryptedTokenMessage()
{
    return m_settings.value("userData/encryptedTokensMessage" ).toByteArray();
}

void Settings::setEncryptedTokensMessage(QByteArray encrypted_tokens_message)
{
    m_settings.setValue("userData/encryptedTokensMessage", encrypted_tokens_message);
    m_settings.sync();
}

void Settings::updateBoolMasterSecret(bool bool_master_secret)
{
    m_settings.setValue("userData/boolMasterSecret", bool_master_secret);
    m_settings.sync();

    emit masterSecretFinished();
}

void Settings::setVPNProfileTimeStamp(QDateTime timestamp)
{
    m_settings.setValue("userData/VPNProfileTimeStamp", timestamp);
    m_settings.sync();
}

void Settings::setVPNIds(QStringList list_vpn_ids)
{
    m_settings.setValue("userData/VPNProfileIds", list_vpn_ids);
    m_settings.sync();
}

void Settings::setLastVPNProfileUpdate(QDateTime last_vpn_profile_update)
{
    m_settings.setValue("userData/LastVPNProfileUpdate", last_vpn_profile_update);
    m_settings.sync();
}

void Settings::setdaysNextProfileUpdate(int days)
{
    m_settings.setValue("userData/daysNextProfileUpdate", days);
    m_settings.sync();

}

void Settings::setCheckDependencies(bool check_dependencies)
{
    m_settings.setValue("userData/checkDependencies", check_dependencies);
    m_settings.sync();
}

void Settings::updateIpIdMap(QMap<QString,QVariant> id_ip_map, QMap<QString,QVariant> ip_id_map, QMap<QString,QVariant> host_id_map, QMap<QString,QVariant> id_ipv4_map)
{
    m_settings.setValue("userData/id_ip_map", id_ip_map);
    m_settings.setValue("userData/ip_id_map", ip_id_map);
    m_settings.setValue("userData/host_id_map", host_id_map);
    m_settings.setValue("userData/id_ipv4_map", id_ipv4_map);
    m_settings.sync();
}

void Settings::getIpIdMap(UserData &user)
{
    user.m_id_ip_map = m_settings.value("userData/id_ip_map").toMap();
    user.m_ip_id_map = m_settings.value("userData/ip_id_map").toMap();
}

void Settings::setShowIp(bool show_ip)
{
    m_settings.setValue("userData/show_ip", show_ip);
    m_settings.sync();
}

void Settings::clearSettings()
{
    //Remove all user settings
    m_settings.remove("userData/boolMasterSecret");
    m_settings.remove("userData/boolVPNProfiles");
    m_settings.remove("userData/VPNProfileTimeStamp");
    m_settings.remove("userData/VPNProfileIds");
    m_settings.remove("userData/LastVPNProfileUpdate");
    m_settings.remove("userData/encryptedTokensMessage");
    m_settings.remove("userData/daysNextProfileUpdate");
    m_settings.remove("userData/checkDependencies");
    m_settings.remove("userData/id_ip_map");
    m_settings.remove("userData/ip_id_map");
    m_settings.remove("userData/host_id_map");
    m_settings.remove("userData/id_ipv4_map");
    m_settings.remove("userData/show_ip");
    m_settings.sync();
}







