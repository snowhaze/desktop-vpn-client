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

#ifndef ZKACRYPTO_H
#define ZKACRYPTO_H
#include <QString>
#include <array>
#include <stdexcept>
#include <tuple>
#include <QObject>
#include <QUrlQuery>
#include <QtDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <type_traits>
#include "sodium.h"
#include "utils.h"
#include "usermessage.h"
#include "urlencoding.h"

class ZKACrypto :public QObject
{
    Q_OBJECT

public:

    static constexpr size_t email_secret_len = 32;
    static constexpr size_t password_salt_len = 16;
    static constexpr size_t password_hash_len = 32;
    static constexpr size_t keyPair_seed_len = 32;
    static constexpr size_t private_key_len = 32;
    static constexpr size_t public_key_len = 32;
    static constexpr size_t master_secret_len = 32;
    static constexpr size_t encryption_key_len = 32;
    static constexpr size_t pseudonym_len = 32;
    static constexpr size_t receipt_key_len = 32;

    static constexpr size_t email_hash_key_len = 32;
    static constexpr size_t password_key_len = 32;

    typedef std::array<unsigned char, email_secret_len> email_secret_t;
    typedef std::array<unsigned char, password_salt_len> password_salt_t;
    typedef std::array<unsigned char, password_hash_len> password_hash_t;
    typedef std::array<unsigned char, keyPair_seed_len> keyPair_seed_t;
    typedef std::array<unsigned char, private_key_len> private_key_t;
    typedef std::array<unsigned char, public_key_len> public_key_t;
    typedef std::array<unsigned char, master_secret_len> master_secret_t;
    typedef std::array<unsigned char, encryption_key_len> encryption_key_t;
    typedef std::array<unsigned char, pseudonym_len> pseudonym_t;
    typedef std::array<unsigned char, receipt_key_len> receipt_key_t;


    typedef std::array<uint8_t, password_key_len> password_key_t;


    template <size_t size = 32>
    using email_hash_t = std::array<unsigned char, size>;
    template <size_t c_size = 32>
    using cipher_text_t = std::array<unsigned char, c_size>;

    static QHash<QString,QString> getMasterSecretparameters(const QString &email, const QString &pw, password_hash_t &password_hash);

    static void calculatePublicPrivateKey(master_secret_t &master_secret, public_key_t &public_key, private_key_t &private_key);

    static bool decryptMasterSecret(const QByteArray &server_response_encrypted, master_secret_t &master_secret_decrypted, password_hash_t &password_hash);

    static std::vector<unsigned char> createAPP(QJsonObject message, QByteArray &server_key, private_key_t &private_key);

    static std::vector<unsigned char> decryptTokens(QByteArray &encrypted_message, public_key_t &public_key, private_key_t &private_key);

    static QByteArray calculatePseudonym(master_secret_t &master_secret);

    static QByteArray calculateReceiptKey(master_secret_t &master_secret);

signals:
    void passwordHashReady(password_hash_t password_hash);
    void requestLogout();

};

#endif // ZKACRYPTO_H
