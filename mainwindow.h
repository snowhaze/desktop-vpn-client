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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QLocale>
#include <QMovie>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include <QProcessEnvironment>
#include <QSysInfo>
#include <QJsonDocument>
#include <QUrl>
#include <QSslSocket>
#include "stdio.h"

#include "apiv3.h"
#include "userdata.h"
#include "usermessage.h"
#include "zkacrypto.h"
#include "settings.h"
#include "sgxverification.h"
#include "vpnitem.h"
#include "settingsitem.h"
#include "ovpnprocess.h"
#include "licenceswidget.h"
#include "licencedialog.h"
#include "utils.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   static int const EXIT_CODE_REBOOT = -123456789;

   static bool ping_sort(UserData::ovpnProfile l_profile,UserData::ovpnProfile r_profile);
   licencesWidget *m_licenceWidget;
   SettingsItem *m_settingCheckUpdate;
   SettingsItem *m_settingShowIp;


   static void on_settingCheckUpdateButtonClicked();
   void on_settingShowIpButtonClicked();

signals:
   void requestReboot();
   void requestSGXVerification(QByteArray encrypted_token_message);
   void logoutWarning();
   void setOnOffButton(bool botton_on);
   void setPing();


public slots:
    void slotReboot();

    void checkVPNProfile();

    void setPublicIP(bool, QString, QString);
    void logout();
    void on_labelGetSubscriptionClicked();
    void on_labelLogoutClicked();
    void on_labelSettingsClicked();
    void on_labelbackClicked();
    void on_labelonoffClicked();

    void labelOnOffSetToConnected();
    void labelOnOffSetToDisonnected();

    void checkIpForActiveConnection();
    void checkActiveConnection();


    void setActiveConnection(bool set_flag, QString id ="");

    void showWaitingPage();
    void showVPNPage();

private slots:
    void on_loginButton_clicked();

    void on_loginButton_mastersecret_clicked();

    void setLoginPage();

    void getVPNProfiles();

    void setVPNPage();

    void on_SGXVerificationButton_clicked();

    void on_refreshButton_clicked();

private:
    Ui::MainWindow *ui;
    APIV3 api;
    UserData user;
    UserMessage message_box;
    Settings app_settings;
    sgxVerification sgxV;
    OVPNProcess ovpnProcess;


    bool VPNProfilesStored(Settings settings);
    void setVPNItemWidgets();
    void setupSettingpage();

};
#endif // MAINWINDOW_H
