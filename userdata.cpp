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

#include "userdata.h"

UserData::UserData(QObject *parent) : QObject(parent)
{
    m_public_private_key_set = false;
    m_password_hash_set = false;
    m_bool_master_secret_set = false;
    m_bool_VPN_profiles_active = false;
    m_bool_subscription_status_set = false;
    m_public_ipv4 = "";
    m_public_ipv6 = "";

    if(sodium_mlock(&m_master_secret, m_master_secret.size()) or
            sodium_mlock(&m_private_key, m_private_key.size()) or
            sodium_mlock(&m_public_key, m_public_key.size()) or
            sodium_mlock(&m_password_hash, m_password_hash.size())){
        emit memLockErrorWarning();
    }

    //Set or create the paths
    vpn_profiles_path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/OpenVPN";
    if(!QDir(vpn_profiles_path).exists()){
        QDir().mkdir(vpn_profiles_path);

    }
    vpn_profiles_path += "/config";
    if(!QDir(vpn_profiles_path).exists()){
        QDir().mkdir(vpn_profiles_path);

    }
    vpn_profiles_path += "/snowhaze/";
    if(!QDir(vpn_profiles_path).exists()){
        QDir().mkdir(vpn_profiles_path);
    }

    vpn_data_path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +"/SnowHazeVPN";
    if(!QDir(vpn_data_path).exists()){
        QDir().mkdir(vpn_data_path);
    }
    vpn_data_path += "/data/";
    if(!QDir(vpn_data_path).exists()){
        QDir().mkdir(vpn_data_path);
    }

    //Set default value of vpn expiration date to current time
    m_earliest_vpn_expiration = QDateTime::currentDateTimeUtc();
}

ZKACrypto::public_key_t UserData::getPublicKey()
{
    if (! m_public_private_key_set){
        ZKACrypto::calculatePublicPrivateKey(m_master_secret,  m_public_key, m_private_key);
        m_public_private_key_set = true;
    }
    return m_public_key;
}

ZKACrypto::private_key_t UserData::getPrivateKey()
{
    if (! m_public_private_key_set){
        ZKACrypto::calculatePublicPrivateKey(m_master_secret,  m_public_key, m_private_key);
        m_public_private_key_set = true;
    }
    return m_private_key;
}

ZKACrypto::password_hash_t UserData::getPasswordHash()
{
    return m_password_hash;
}

void UserData::setMastersecretFromCredentials(QByteArray m_s)
{
    auto pw_hash = getPasswordHash();
    if(! ZKACrypto::decryptMasterSecret(m_s, m_master_secret, pw_hash)){
        throw std::runtime_error("Server response decryption failed");
    }
    //store the mastersecret localy
    storeMasterSecret();

    //change in settings the "userData/boolMasterSecret" to true
    emit QKeychainMasterSecretStored(m_bool_master_secret_set);
}

void UserData::setTokens(QByteArray encrypted_tokens_message, QByteArray separator)
{
    emit setEncryptedTokensMessage(encrypted_tokens_message);
    std::vector<unsigned char> tokens_vector = ZKACrypto::decryptTokens(encrypted_tokens_message, m_public_key, m_private_key);
    QByteArray tokens(reinterpret_cast<char const*>(tokens_vector.data()), static_cast<int>(tokens_vector.size()));
    m_tokens_list = tokens.split(separator.at(0));
    std::random_shuffle(m_tokens_list.begin(), m_tokens_list.end());
    emit tokensListSet();
}

void UserData::storeMasterSecret()
{
    //store the mastersecret localy
    QKeychain::WritePasswordJob job_ms( QLatin1String("SnowHazeVPN_master_secret"));
    job_ms.setAutoDelete(false);
    const QString key = "master_secret_user";
    job_ms.setKey(key);
    QByteArray master_secret_array(reinterpret_cast<char const*>(m_master_secret.data()), sizeof(m_master_secret));
    QString master_secret_string_base64 = master_secret_array.toBase64();

    job_ms.setTextData(master_secret_string_base64);
    QEventLoop loop;
    job_ms.connect(&job_ms, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()));
    job_ms.start();
    loop.exec();
    if ( job_ms.error() ) {
        emit QKeychainError();
        return;
    }

    m_bool_master_secret_set = true;
}

void UserData::setBoolMasterSecret(bool &bool_master_secret)
{
    m_bool_master_secret_set = bool_master_secret;
}

bool UserData::checkVPNProfileActive(QDateTime &vpn_profile_time_stamp)
{
    return QDateTime::currentDateTimeUtc()<vpn_profile_time_stamp;
}

bool UserData::checkProfileUpdateOverdue()
{
    //Checks whether the last profile update is older than m_settings_days_next_profile_update. Returns true if profiles must be updated
    return m_settings_days_next_profile_update < m_last_vpn_profile_update.daysTo(QDateTime::currentDateTime());
}

void UserData::setMasterSecret(const QString &master_secret_string)
{
    QByteArray master_secret_qba = QByteArray::fromHex(master_secret_string.toUtf8().data()); //QByteArray::fromBase64(QByteArray(master_secret_string.toUtf8().data()));

    //check correctness of master secret
    if( master_secret_qba.length() != 1+ crypto_box_SECRETKEYBYTES + 2){
        emit userMasterSecretError(1);
        return;
    }
    if(master_secret_qba.at(0) != 1){
        emit userMasterSecretError(2);
        return;
    }

    if (utils::crc16ccitt(master_secret_qba.begin(), master_secret_qba.end())!= 0){
        emit userMasterSecretError(3);
        return;
    }

    std::copy(master_secret_qba.begin()+1, master_secret_qba.begin()+1+crypto_box_SECRETKEYBYTES, m_master_secret.data());
    //store the mastersecret
    storeMasterSecret();

    //change in settings the "userData/boolMasterSecret" to true
    emit QKeychainMasterSecretStored(m_bool_master_secret_set);
}

void UserData::loadMasterSecret()
{
    //Load the master secret from the QKeychain
    QKeychain::ReadPasswordJob job_ms( QLatin1String("SnowHazeVPN_master_secret") );
    job_ms.setAutoDelete( false );
    const QString key = "master_secret_user";
    job_ms.setKey(key);
    QEventLoop loop;
    job_ms.connect( &job_ms, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()) );
    job_ms.start();
    loop.exec();
    if ( job_ms.error() ) {
        emit QKeychainError();
        return;
    }
    const QString master_secret_string = job_ms.textData();
    QByteArray master_secret_qba = QByteArray(master_secret_string.toUtf8().data());
    QByteArray master_secret_array = QByteArray::fromBase64(master_secret_qba);

    std::copy(reinterpret_cast<const unsigned char *>(master_secret_array.begin()), reinterpret_cast<const unsigned char *>(master_secret_array.end()), m_master_secret.data());
}

QByteArray UserData::getPseudonym()
{
    return ZKACrypto::calculatePseudonym(m_master_secret);
}

QByteArray UserData::getReceiptKey()
{
    return ZKACrypto::calculateReceiptKey(m_master_secret);
}

QByteArray UserData::getToken()
{
    if (m_tokens_list.isEmpty())
    {
        emit overusedTokensWarning();
        return 0;
    } else
    {
        //QByteArray token = m_tokens_list.at(rand() % m_tokens_list.size());
        QByteArray token = m_tokens_list.first();
        m_tokens_list.removeFirst();
        return token;
    }
}

bool UserData::loadVPNProfiles(QStringList list_vpn_ids)
{
    if (list_vpn_ids.size() == 0){
        emit loadVPNProfilesFailed();
        return false;
    }

    //Construct the openvpn profiles structs
    foreach (QString id, list_vpn_ids) {
        struct ovpnProfile profile;
        profile.id = id;
        profile.names_path = vpn_data_path + id + ".json";
        profile.profile_path = vpn_profiles_path + id + ".ovpn";
        profile.flag_path = vpn_data_path + id + ".png";

        QLocale local;
        QString language = local.name().split("_")[0];
        QFile names_file;
        names_file.setFileName(profile.names_path);
        names_file.open(QIODevice::ReadOnly | QIODevice::Text);
        QJsonDocument data = QJsonDocument::fromJson(names_file.readAll());
        names_file.close();

        if(data.object().value("names").toObject().contains(language)){
            profile.name = data.object().value("names").toObject().value(language).toString();
        } else {
            profile.name = data.object().value("names").toObject().value("en").toString();
        }
        profile.profile_path = vpn_profiles_path + profile.name + "_" + id + ".ovpn";
        profile.host = data.object().value("host").toString();

        list_ovpnProfiles << profile;
        m_list_vpn_ids << profile.id;
    }
    return true;
}

void UserData::clearLocalVPNData()
{
    QDir(vpn_profiles_path).removeRecursively();
    QDir(vpn_data_path).removeRecursively();
}

void UserData::getPing(int index)
{
    mutex.lock();
    QString m_ping_not_available = "-";
    list_ovpnProfiles[index].ping = m_ping_not_available;
    QProcess pingProcess;
    QString ping_response;
    QStringList arg;
    arg << m_id_ipv4_map.value(list_ovpnProfiles[index].id).toString() << "-n" << "2";
    pingProcess.start("ping", arg);
    pingProcess.waitForStarted();
    QElapsedTimer timer;
    timer.start();
    while( pingProcess.state() == QProcess::Running){
        pingProcess.waitForReadyRead();
        ping_response += pingProcess.readAll();
        if (timer.elapsed()> 4000){
            mutex.unlock();
            return;
        }
    }
    if(ping_response.contains("= ")){
        QStringList list = ping_response.split("= ");
        if (!list.isEmpty()){
            QString ping = list.last().trimmed();
            list_ovpnProfiles[index].ping = ping.split("ms").first();
            mutex.unlock();
            return;
        } else {
            mutex.unlock();
            return;
        }

    } else {
        mutex.unlock();
        return;
    }

}

void UserData::setVPNProfiles(QJsonValue ovpn_object)
{
    int i = 0;
    while (ovpn_object.operator[](i).isObject()){

        //Generate a new ovpnProfile struct and add the id, names_path, etc.
        struct ovpnProfile profile;
        profile.id = ovpn_object.operator[](i).toObject().value("id").toString().toUtf8();
        profile.names_path = vpn_data_path + ovpn_object.operator[](i).toObject().value("id").toString() + ".json";
        profile.flag_path = vpn_data_path + ovpn_object.operator[](i).toObject().value("id").toString() + ".png";
        profile.host = ovpn_object.operator[](i).toObject().value("hosts")[0].toString();
        profile.expiration = QDateTime::fromSecsSinceEpoch(static_cast<long long>(ovpn_object.operator[](i).toObject().value("expiration").toDouble()), Qt::OffsetFromUTC);
        if(i == 0){
            m_earliest_vpn_expiration = profile.expiration;
        } else if ( profile.expiration < m_earliest_vpn_expiration){
            m_earliest_vpn_expiration = profile.expiration;
        }

        //Make the entries in tha maps which are used to identify active connections
        m_host_id_map.insert(profile.host, profile.id);
        QHostInfo info = QHostInfo::fromName(profile.host);
        lookedUpHost(info);

        //Set the profile name depending on the system language
        QLocale local;
        QString language = local.name().split("_")[0];
        if(ovpn_object.operator[](i).toObject().value("names").toObject().contains(language)){
            profile.name = ovpn_object.operator[](i).toObject().value("names").toObject().value(language).toString();
        } else {
            profile.name = ovpn_object.operator[](i).toObject().value("names").toObject().value("en").toString();
        }
        profile.profile_path = vpn_profiles_path + profile.name + ".ovpn";

        //Localy store the ovpn profile
        QFile ovpnfile(profile.profile_path);
        ovpnfile.open(QIODevice::WriteOnly);
        QTextStream profile_out(&ovpnfile);
        profile_out << ovpn_object.operator[](i).toObject().value("profile").toString().toUtf8()
                <<  "\nsetenv FRIENDLY_NAME \" " << " "
                << profile.name << "\"";
        ovpnfile.close();

        //Localy store the flag
        QImage flag;
        flag.loadFromData(QByteArray::fromBase64(ovpn_object.operator[](i).toObject().value("flag").toString().toUtf8()));
        flag.save(profile.flag_path);

        //Localy store the names in a Json file
        QFile namesfile(profile.names_path);
        namesfile.open(QIODevice::WriteOnly);
        QJsonObject json_object;
        json_object.insert("host", profile.host);
        json_object.insert("names", ovpn_object.operator[](i).toObject().value("names").toObject());
        QJsonDocument saveData(json_object);
        namesfile.write(saveData.toJson());
        namesfile.close();

        list_ovpnProfiles << profile;
        m_list_vpn_ids << profile.id;
        i++;
    }
    m_last_vpn_profile_update = QDateTime::currentDateTimeUtc();

    emit setLastVPNProfileUpdate(m_last_vpn_profile_update);
    emit setVPNProfileTimeStamp(m_earliest_vpn_expiration);
    emit setVPNIds(m_list_vpn_ids);
    emit settingsUpdateIpIdMap(m_id_ip_map, m_ip_id_map, m_host_id_map, m_id_ipv4_map);

    emit vpnDataReady();
}

void UserData::updateIp(QString host, QString new_ip)
{
    if (host == "ipv4" and m_public_ipv4 != new_ip) {
        m_public_ipv4 = new_ip;
        emit IpUpdated(true, m_public_ipv4, m_public_ipv6);
    } else if ( host == "ipv6"  and m_public_ipv6 != new_ip){
        m_public_ipv6 = new_ip;
        emit IpUpdated(true, m_public_ipv4, m_public_ipv6);
    } else {
        emit IpUpdated(false, m_public_ipv4, m_public_ipv6);
    }
}

void UserData::setPing()
{
    QFutureSynchronizer<void> synchronizer;
    for(int i = 0; i< list_ovpnProfiles.count(); i++){
        QFuture<void> future = QtConcurrent::run(this, &UserData::getPing, i);
        synchronizer.addFuture(future);
    }
    synchronizer.waitForFinished();
    emit setPingFinished();
    return;
}

void UserData::lookedUpHost(const QHostInfo &host)
{
    if (host.error() != QHostInfo::NoError) {
        return;
    }

    const auto addresses = host.addresses();
    QString id = m_host_id_map.value(host.hostName()).toString();
    for (const QHostAddress &address : addresses)
    {
        m_ip_id_map.insertMulti(address.toString(),id);
        m_id_ip_map.insertMulti(id, address.toString());
        if (address.protocol() == QAbstractSocket::IPv4Protocol) m_id_ipv4_map.insert(id, address.toString());
    }
    return;
}

void UserData::setPasswordHash(const ZKACrypto::password_hash_t &p_h)
{
    if (! m_password_hash_set){
        m_password_hash = p_h;
        m_password_hash_set = true;
    }
}

void UserData::logout()
{
    //delete all user data: zeroing all sensitive data,
    sodium_memzero(&m_public_key, sizeof (m_public_key));
    sodium_memzero(&m_private_key, sizeof (m_private_key));
    sodium_memzero(&m_master_secret, sizeof (m_master_secret));
    sodium_memzero(&m_password_hash, sizeof (m_password_hash));
    //zeroing data in keychain-> store zeroed master secret to keychain
    storeMasterSecret();
    //reset all userdata variables,
    m_public_private_key_set = false;
    m_password_hash_set = false;
    m_bool_master_secret_set = false;
    m_bool_VPN_profiles_active = false;
    m_bool_subscription_status_set = false;

    list_ovpnProfiles.erase(list_ovpnProfiles.begin(), list_ovpnProfiles.end());
    list_ovpnProfiles.clear();

    clearLocalVPNData();

    //set all settings to default value
    emit clearSettingsRequest();
}









