#Copyright (C) 2020  Illotros GmbH

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <https://www.gnu.org/licenses/>.

QT       += core gui
QT       += widgets
QT       += network
QT       += svg
QT       += core
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += C:\Qt\Tools\OpenSSL\Win_x64\include

LIBS += "$$PWD\libsodium-23.dll"

include($$PWD/qtkeychain/qt5keychain.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    apiv3.cpp \
    clickableqlabel.cpp \
    licencedialog.cpp \
    licenceswidget.cpp \
    main.cpp \
    mainwindow.cpp \
    ovpnprocess.cpp \
    settings.cpp \
    settingsitem.cpp \
    sgxverification.cpp \
    urlencoding.cpp \
    userdata.cpp \
    usermessage.cpp \
    utils.cpp \
    vpnitem.cpp \
    zkacrypto.cpp

HEADERS += \
    apiv3.h \
    clickableqlabel.h \
    licencedialog.h \
    licenceswidget.h \
    mainwindow.h \
    ovpnprocess.h \
    settings.h \
    settingsitem.h \
    sgxverification.h \
    sodium.h \
    sodium/core.h \
    sodium/crypto_aead_aes256gcm.h \
    sodium/crypto_aead_chacha20poly1305.h \
    sodium/crypto_aead_xchacha20poly1305.h \
    sodium/crypto_auth.h \
    sodium/crypto_auth_hmacsha256.h \
    sodium/crypto_auth_hmacsha512.h \
    sodium/crypto_auth_hmacsha512256.h \
    sodium/crypto_box.h \
    sodium/crypto_box_curve25519xchacha20poly1305.h \
    sodium/crypto_box_curve25519xsalsa20poly1305.h \
    sodium/crypto_core_ed25519.h \
    sodium/crypto_core_hchacha20.h \
    sodium/crypto_core_hsalsa20.h \
    sodium/crypto_core_ristretto255.h \
    sodium/crypto_core_salsa20.h \
    sodium/crypto_core_salsa2012.h \
    sodium/crypto_core_salsa208.h \
    sodium/crypto_generichash.h \
    sodium/crypto_generichash_blake2b.h \
    sodium/crypto_hash.h \
    sodium/crypto_hash_sha256.h \
    sodium/crypto_hash_sha512.h \
    sodium/crypto_kdf.h \
    sodium/crypto_kdf_blake2b.h \
    sodium/crypto_kx.h \
    sodium/crypto_onetimeauth.h \
    sodium/crypto_onetimeauth_poly1305.h \
    sodium/crypto_pwhash.h \
    sodium/crypto_pwhash_argon2i.h \
    sodium/crypto_pwhash_argon2id.h \
    sodium/crypto_pwhash_scryptsalsa208sha256.h \
    sodium/crypto_scalarmult.h \
    sodium/crypto_scalarmult_curve25519.h \
    sodium/crypto_scalarmult_ed25519.h \
    sodium/crypto_scalarmult_ristretto255.h \
    sodium/crypto_secretbox.h \
    sodium/crypto_secretbox_xchacha20poly1305.h \
    sodium/crypto_secretbox_xsalsa20poly1305.h \
    sodium/crypto_secretstream_xchacha20poly1305.h \
    sodium/crypto_shorthash.h \
    sodium/crypto_shorthash_siphash24.h \
    sodium/crypto_sign.h \
    sodium/crypto_sign_ed25519.h \
    sodium/crypto_sign_edwards25519sha512batch.h \
    sodium/crypto_stream.h \
    sodium/crypto_stream_chacha20.h \
    sodium/crypto_stream_salsa20.h \
    sodium/crypto_stream_salsa2012.h \
    sodium/crypto_stream_salsa208.h \
    sodium/crypto_stream_xchacha20.h \
    sodium/crypto_stream_xsalsa20.h \
    sodium/crypto_verify_16.h \
    sodium/crypto_verify_32.h \
    sodium/crypto_verify_64.h \
    sodium/export.h \
    sodium/randombytes.h \
    sodium/randombytes_internal_random.h \
    sodium/randombytes_sysrandom.h \
    sodium/runtime.h \
    sodium/utils.h \
    sodium/version.h \
    urlencoding.h \
    userdata.h \
    usermessage.h \
    utils.h \
    vpnitem.h \
    zkacrypto.h

FORMS += \
    licencedialog.ui \
    licenceswidget.ui \
    mainwindow.ui \
    settingsitem.ui \
    vpnitem.ui

TRANSLATIONS += translations/shvpn_de.ts \
    translations/shvpn_fr.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
TARGET = "SnowHaze VPN"

DISTFILES += \
    qtkeychain/CMakeLists.txt \
    qtkeychain/ChangeLog \
    qtkeychain/QtKeychainConfig.cmake.in \
    qtkeychain/ReadMe.markdown \
    qtkeychain/ReadMe.txt \
    qtkeychain/appveyor.yml \
    qtkeychain/appveyorHelp.psm1 \
    qtkeychain/cmake/Modules/ECMGeneratePriFile.cmake \
    qtkeychain/cmake/Modules/ECMPackageConfigHelpers.cmake \
    qtkeychain/cmake/Modules/ECMQueryQmake.cmake \
    qtkeychain/cmake/Modules/ECMSetupVersion.cmake \
    qtkeychain/cmake/Modules/GNUInstallDirs.cmake \
    qtkeychain/org.kde.KWallet.xml \
    qtkeychain/qt5keychain.pri \
    qtkeychain/translations/qtkeychain_de.ts \
    qtkeychain/translations/qtkeychain_fr.ts \
    qtkeychain/translations/qtkeychain_ro.ts \
    qtkeychain/translations/qtkeychain_ru.ts \
    qtkeychain/translations/qtkeychain_zh.ts

RESOURCES += \
    resources.qrc

#Adding the icon
win32:RC_ICONS = snowhaze_icon.ico
macx: ICON = snowhaze_icon.icns

#Application version
VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = 0

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
