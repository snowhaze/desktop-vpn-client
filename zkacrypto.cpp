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

#include "zkacrypto.h"


static_assert(ZKACrypto::email_secret_len == crypto_kdf_KEYBYTES, "email_secret_len must be crypto_kdf_KEYBYTES");
static_assert(ZKACrypto::password_salt_len == crypto_pwhash_SALTBYTES, "email_secret_len must be crypto_pwhash_SALTBYTES");
static_assert(ZKACrypto::password_hash_len == crypto_kdf_KEYBYTES, "password_hash_len must be crypto_kdf_KEYBYTES");
static_assert(ZKACrypto::keyPair_seed_len == crypto_box_SEEDBYTES, "keyPair_seed_len must be crypto_box_SEEDBYTES");
static_assert(ZKACrypto::private_key_len == crypto_box_SECRETKEYBYTES, "private_key_len must be crypto_box_SECRETKEYBYTES");
static_assert(ZKACrypto::public_key_len == crypto_box_PUBLICKEYBYTES, "public_key_len must be crypto_box_PUBLICKEYBYTES");
static_assert(ZKACrypto::encryption_key_len == crypto_aead_chacha20poly1305_ietf_KEYBYTES, "encryption_key_len must be crypto_aead_chacha20poly1305_ietf_KEYBYTES");
static_assert(ZKACrypto::receipt_key_len == crypto_aead_chacha20poly1305_ietf_KEYBYTES, "receipt_key_len must be crypto_aead_chacha20poly1305_ietf_KEYBYTES");
static_assert(ZKACrypto::pseudonym_len == 32, "password_hash_len must be 32");

static_assert(ZKACrypto::email_hash_key_len == crypto_generichash_KEYBYTES, "email hash key lenght must be crypto_generichash_KEYBYTES");
static_assert(ZKACrypto::password_key_len == crypto_secretbox_KEYBYTES, "password key length mus be crypto_secretbox_KEYBYTES");


QHash<QString,QString> ZKACrypto::getMasterSecretparameters(const QString &email, const QString &pw, password_hash_t &password_hash)
{
    //generate Email Secret
    const unsigned char email_hash_salt[] = "\xac\xee\xb5\x86\x38\x52\xf3\xb1\xa8\xaf\x03\x94\x5c\x92\xef\x30";
    QString normalized_email = email.normalized(QString::NormalizationForm_KC).toLower();

    email_secret_t email_secret;
    if(crypto_pwhash(
                email_secret.data(),
                email_secret.size(),
                reinterpret_cast<const char*>(normalized_email.toUtf8().data()),
                static_cast<unsigned>(normalized_email.size()),
                email_hash_salt,
                crypto_pwhash_OPSLIMIT_INTERACTIVE,
                crypto_pwhash_MEMLIMIT_MODERATE,
                crypto_pwhash_ALG_DEFAULT)){
        throw std::runtime_error("email hasing failed (email secret calculation)");
    }

    //generate password hash
    //get password hash salt
    password_salt_t password_hash_salt;
    if (crypto_kdf_derive_from_key(password_hash_salt.data(),
                                  password_hash_salt.size(),
                                  0,
                                  "kshorten",
                                  email_secret.data())){
        throw std::runtime_error("kdf derive from key failed (kdf from email_secret)");
    }
    QString normalized_pw = pw.normalized(QString::NormalizationForm_KC);
    if (crypto_pwhash(password_hash.data(),
                      password_hash.size(),
                      reinterpret_cast<const char*>(normalized_pw.toUtf8().data()),
                      static_cast<unsigned>(normalized_pw.toUtf8().size()),
                      password_hash_salt.data(),
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_MODERATE,
                      crypto_pwhash_ALG_DEFAULT)){
        throw std::runtime_error("pwhash failed, hashing of the password failed");
    }

    //generate password key
    password_key_t password_key;

    if (crypto_kdf_derive_from_key(password_key.data(), password_key.size(), 1, "keyderiv", password_hash.data())) {
        throw std::runtime_error("kdf derive from key failed");
    }

    //generate email hash
    email_hash_t<32> email_hash;
    if (crypto_kdf_derive_from_key(email_hash.data(),
                               email_hash.size(),
                               0,
                               "hidemail",
                               email_secret.data())){
        throw std::runtime_error("kdf derive from key (generate email hash from email_secret) failed");
    }

    QByteArray email_hash_array(reinterpret_cast<char const*>(email_hash.data()), email_hash.size());
    email_hash_array.prepend(1);
    QByteArray email_hash_base64 = email_hash_array.toBase64();

    QByteArray password_key_array(reinterpret_cast<char const*>(password_key.data()), password_key.size());
    password_key_array.prepend(1);
    QByteArray password_key_base64 = password_key_array.toBase64();

    QHash<QString,QString> parameters;
    parameters.insert("password_key", url_encode(password_key_base64).data());
    parameters.insert("email_hash", url_encode(email_hash_base64).data());
    parameters.insert("action", "get_master_secret");


    return parameters;

}

void ZKACrypto::calculatePublicPrivateKey(master_secret_t &master_secret, public_key_t &public_key, private_key_t &private_key)
{
    keyPair_seed_t seed;
    if (crypto_kdf_derive_from_key(seed.data(),
                                  seed.size(),
                                  0,
                                  "ms->skey",
                                  master_secret.data())){
        throw std::runtime_error("kdf derive from key failed (kdf from master_secret to derive seed for key-pair failed)");
    }

    if(crypto_box_seed_keypair(public_key.data(),
                               private_key.data(),
                               seed.data())){
        throw std::runtime_error("keypair genearation failed)");
    }
}

bool ZKACrypto::decryptMasterSecret(const QByteArray &server_response_encrypted, ZKACrypto::master_secret_t &master_secret_decrypted, password_hash_t &password_hash)
{
    if(server_response_encrypted.size() < 1 + static_cast<int>(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES)){
        return false;
    }
    if(server_response_encrypted[0] != 1){
        return false;
    }
    encryption_key_t encryption_key;
    if (crypto_kdf_derive_from_key(encryption_key.data(),
                               encryption_key.size(),
                               0,
                               "keyderiv",
                               password_hash.data())){
        throw std::runtime_error("kdf derive from key (generate encryption key from email hash) failed");
    }

    const auto begin_nonce = server_response_encrypted.begin()+1;
    const auto begin_master_secret = server_response_encrypted.begin()+1+crypto_aead_xchacha20poly1305_ietf_NPUBBYTES;

    std::array<unsigned char, crypto_aead_xchacha20poly1305_ietf_NPUBBYTES> nonce;
    std::copy(begin_nonce, begin_master_secret, nonce.data());

    const unsigned char ad[] = "\x01";

    unsigned long long master_secret_decrypted_len = master_secret_decrypted.size();
    if(crypto_aead_xchacha20poly1305_ietf_decrypt(master_secret_decrypted.data(),
                                                  &master_secret_decrypted_len,
                                                  nullptr,
                                                  reinterpret_cast<const unsigned char *>(begin_master_secret),
                                                  static_cast<unsigned long long>(std::distance(begin_master_secret, server_response_encrypted.end())),
                                                  ad,
                                                  sizeof (ad)-1,
                                                  nonce.data(),
                                                  encryption_key.data())){
        throw std::runtime_error("kdf derive from key (generate encryption key from email hash) failed");
    }
    return true;
}

std::vector<unsigned char>  ZKACrypto::createAPP(QJsonObject json_message, QByteArray &server_key, ZKACrypto::private_key_t &private_key)
{
    QJsonDocument message_json_document(json_message);
    const auto message = message_json_document.toJson(QJsonDocument::Compact);
    std::vector<unsigned char> cipher_text(crypto_box_NONCEBYTES + crypto_box_MACBYTES + static_cast<unsigned int>(message.size()));
    randombytes_buf(cipher_text.data(), crypto_box_NONCEBYTES);

    if(crypto_box_easy(cipher_text.data() + crypto_box_NONCEBYTES,
                       reinterpret_cast<const unsigned char*>(message.data()),
                       static_cast<unsigned long long>(message.size()),
                       cipher_text.data(),
                       reinterpret_cast<const unsigned char *>(server_key.data()),
                       private_key.data())){
        throw std::runtime_error("APP generation failed"); //TODO: creat excpetion class
    }
    return  cipher_text;
}

std::vector<unsigned char> ZKACrypto::decryptTokens(QByteArray &encrypted_message, public_key_t &public_key, ZKACrypto::private_key_t &private_key)
{
    std::vector<unsigned char> tokens(static_cast<unsigned long long>(encrypted_message.size()) - crypto_box_SEALBYTES - crypto_sign_BYTES);

    if(crypto_box_seal_open(tokens.data(),
                       reinterpret_cast<const unsigned char*>(encrypted_message.begin() + crypto_sign_BYTES),
                       static_cast<unsigned long long>(encrypted_message.size())  - crypto_sign_BYTES,
                       public_key.data(),
                       private_key.data())){
        UserMessage::encryptionFailure("Tokens decryption failure");
        throw std::runtime_error("decrypting tokens failed");
    }
    return tokens;
}

QByteArray ZKACrypto::calculatePseudonym(ZKACrypto::master_secret_t &master_secret)
{
    pseudonym_t pseudonym;
    if (crypto_kdf_derive_from_key(pseudonym.data(),
                               pseudonym.size(),
                               0,
                               "ms->psdm",
                               master_secret.data())){
        throw std::runtime_error("kdf derive from key (generate pseudonym from mastersecret) failed");
    }
     QByteArray pseudonym_array(reinterpret_cast<char const*>(pseudonym.data()), pseudonym.size());
     pseudonym_array.prepend(1);

     return pseudonym_array;
}

QByteArray ZKACrypto::calculateReceiptKey(ZKACrypto::master_secret_t &master_secret)
{
  receipt_key_t reveipt_key;
  if (crypto_kdf_derive_from_key(reveipt_key.data(),
                 reveipt_key.size(),
                 0,
                 "ms->rctk",
                 master_secret.data())){
    throw std::runtime_error("kdf derive from key (generate pseudonym from mastersecret) failed");
  }
   QByteArray receipt_key_array(reinterpret_cast<char const*>(reveipt_key.data()), reveipt_key.size());
   return receipt_key_array;
}


