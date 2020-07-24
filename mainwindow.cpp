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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , api(this)
    , user()
    , message_box()
    , app_settings(user)
    , sgxV()
    , ovpnProcess()
{
    ui->setupUi(this);
    ui->statusbar->hide();
    ui->label_snowhazeLogo_1->setScaledContents(true);
    ui->label_snowhazeLogo_1->setAlignment(Qt::AlignCenter);
    ui->label_waiting_spinner->setScaledContents(true);
    QMovie *spinner_movie = new QMovie(":/img/img/waiting_spinner.gif");
    ui->label_waiting_spinner->setMovie(spinner_movie);
    ui->label_waiting_spinner->show();
    spinner_movie->start();
    ui->stackedWidget->setCurrentWidget(ui->waiting_page);
    ui->refreshButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->SGXVerificationButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->loginButton_mastersecret->setCursor(QCursor(Qt::PointingHandCursor));

    ui->label_currentVPNFlag->setScaledContents(true);
    setActiveConnection(false, "");

    ui->label_on_off->setPixmap(QPixmap(":/img/img/off_button.svg"));
    //set the setup the settings page
    setupSettingpage();

    if(user.m_check_dependencies){
        app_settings.setCheckDependencies( message_box.missingDependecyWarning("OpenVPN Gui"));
    }

    ovpnProcess.initializePaths(user.vpn_profiles_path);

    //Error signals from mainwindow to message box
    connect(this, &MainWindow::logoutWarning, &message_box, &UserMessage::logoutWarning);
    //Error signals from api to message box
    connect(&api, SIGNAL(wrongCredentials()), &message_box, SLOT(wrongCredentials()));
    connect(&api, &APIV3::networkError, &message_box, &UserMessage::networkError);
    connect(&api, SIGNAL(invalidSubscription()), &message_box, SLOT(invalidSubscription()));
    connect(&api, &APIV3::publicKeyNotRegistered, &message_box, &UserMessage::publicKeyNotRegistered);
    //Error signals from user (UserData) to message box
    connect(&user, SIGNAL(QKeychainError()), &message_box, SLOT(QKeychainError()));
    connect(&user, SIGNAL(userMasterSecretError(int)), &message_box, SLOT(userMasterSecretError(int)));
    connect(&user, SIGNAL(loadVPNProfilesFailed()), &message_box, SLOT(loadVPNProfilesFailed()));
    connect(&user, &UserData::overusedTokensWarning, &message_box, &UserMessage::overusedTokensWarning);
    connect(&user, &UserData::memLockErrorWarning, &message_box, &UserMessage::memLockErrorWarning);
    //Error signals from sgxVerification to message box
    connect(&sgxV, SIGNAL(processError(QString)), &message_box, SLOT(processError(QString)));
    //Error signals from ovpnprocess to message box
    connect(&ovpnProcess, SIGNAL(vpnProcessTimeoutError()), &message_box, SLOT(vpnProcessTimeoutError()));

    //Signals to the settings
    connect(&user,SIGNAL(QKeychainMasterSecretStored(bool)), &app_settings, SLOT(updateBoolMasterSecret(bool)));
    connect(&user, SIGNAL(setEncryptedTokensMessage(QByteArray)), &app_settings, SLOT(setEncryptedTokensMessage(QByteArray)));
    connect(&user, &UserData::clearSettingsRequest, &app_settings, &Settings::clearSettings);
    connect(&user, SIGNAL(setVPNProfileTimeStamp(QDateTime)), &app_settings, SLOT(setVPNProfileTimeStamp(QDateTime)));
    connect(&user, &UserData::setLastVPNProfileUpdate, &app_settings, &Settings::setLastVPNProfileUpdate);
    connect(&user, &UserData::setVPNIds, &app_settings, &Settings::setVPNIds);
    connect(&user, &UserData::settingsUpdateIpIdMap, &app_settings, &Settings::updateIpIdMap);

    //Signals to UserData (user)
    connect(&api,SIGNAL(serverResponseMasterSecretReady(QByteArray)),&user,SLOT(setMastersecretFromCredentials(QByteArray)));
    connect(&api, &APIV3::serverResponseTokensReady, &user, &UserData::setTokens);
    connect(&api, SIGNAL(openVPNObjectReady(QJsonValue)), &user, SLOT(setVPNProfiles(QJsonValue)));
    connect(&api, &APIV3::updateIp, &user, &UserData::updateIp);
    connect(this, &MainWindow::setPing, &user, &UserData::setPing, Qt::QueuedConnection);

    //Signals to this (MainWindow)
    connect(&message_box,SIGNAL(goToLoginPage()),this, SLOT(setLoginPage()));
    connect(&message_box, &UserMessage::requestLogout, this, &MainWindow::logout);

    connect(&app_settings, SIGNAL(masterSecretFinished()), this, SLOT(checkVPNProfile()));
    connect(&user, SIGNAL(tokensListSet()), this, SLOT(getVPNProfiles()));
    connect(&api, SIGNAL(repeateVPNProfileRequest()), this, SLOT(getVPNProfiles()));
    connect(&user, &UserData::vpnDataReady, this, &MainWindow::setVPNPage);
    connect(&user, &UserData::IpUpdated, this, &MainWindow::setPublicIP);
    connect(&ovpnProcess, &OVPNProcess::checkActiveConnectionSignal, this, &MainWindow::checkActiveConnection);
    connect(&ovpnProcess, &OVPNProcess::setActiveConnection, this, &MainWindow::setActiveConnection);
    connect(&ovpnProcess, &OVPNProcess::setToConnected, this, &MainWindow::labelOnOffSetToConnected);
    connect(&ovpnProcess, &OVPNProcess::setToDisconnected, this , &MainWindow::labelOnOffSetToDisonnected);
    connect(&ovpnProcess, &OVPNProcess::setVPNPage, this, &MainWindow::setVPNPage);
    //Connection from the clickableQlabel labels in the ui to this
    connect(ui->label_getSubscription, &ClickableQLabel::leftClicked, this, &MainWindow::on_labelGetSubscriptionClicked);
    connect(ui->label_logout, &ClickableQLabel::leftClicked, this, &MainWindow::on_labelLogoutClicked);
    connect(ui->label_settings, &ClickableQLabel::leftClicked, this, &MainWindow::on_labelSettingsClicked);
    connect(ui->label_backarrow, &ClickableQLabel::leftClicked, this, &MainWindow::on_labelbackClicked);
    connect(ui->label_on_off, &ClickableQLabel::leftClicked, this, &MainWindow::on_labelonoffClicked);


    //SGXVerification
    connect(this, SIGNAL(requestSGXVerification(QByteArray)), &sgxV, SLOT(verifyTokenResponse(QByteArray)));

    //Rebooting the application
    connect(this, &MainWindow::requestReboot, this, &MainWindow::slotReboot);

    //Checking if the userer was logged in before and whether vpn profiles exist or not
    if(user.m_bool_master_secret_set){
        checkVPNProfile();
    } else {
        ui->stackedWidget->setCurrentWidget(ui->login_page);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotReboot()
{
    qApp->exit( MainWindow::EXIT_CODE_REBOOT );
}

void MainWindow::setPublicIP(bool , QString current_ipv4, QString current_ipv6)
{
    ui->label_currentIPv4->setText( current_ipv4 );
    ui->label_currentIPv6->setText( current_ipv6 );
}

void MainWindow::logout()
{
    ui->stackedWidget->setCurrentWidget(ui->waiting_page);
    user.logout();
    ovpnProcess.disconnect(true);
    emit requestReboot();
}

void MainWindow::checkVPNProfile()
{
    QDateTime vpn_profile_time_stamp = app_settings.getVPNProfileTimeStamp();

    if(UserData::checkVPNProfileActive(vpn_profile_time_stamp) and !user.checkProfileUpdateOverdue()){
        if(user.loadVPNProfiles(app_settings.getVPNIds())){
            setVPNPage();
        } else {
            //If loading the profiles failed clear the vpn data and load them again from the mastersecret
            user.clearLocalVPNData();
            user.loadMasterSecret();
            api.tokensRequest(user);
        }
    }
    else {
        //Check subscription status
        user.loadMasterSecret();
        api.tokensRequest(user);
    }
}

void MainWindow::on_loginButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->waiting_page);
    QString email = ui->lineEdit_email->text();
    QString pw = ui->lineEdit_PW->text();
    ui->lineEdit_email->clear();
    ui->lineEdit_PW->clear();
    ui->lineEdit_masterSecret->clear();

    ZKACrypto::password_hash_t pw_hash;
    QHash<QString, QString> getmastersecret_parameters = ZKACrypto::getMasterSecretparameters(email, pw, pw_hash);
    user.setPasswordHash(pw_hash);

    api.mastersecretRequest(getmastersecret_parameters);
}

void MainWindow::on_loginButton_mastersecret_clicked()
{
    QString masterSecert_string = ui->lineEdit_masterSecret->text();
    ui->lineEdit_masterSecret->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_PW->clear();
    ui->stackedWidget->setCurrentWidget(ui->waiting_page);
    user.setMasterSecret(masterSecert_string);
}

void MainWindow::setLoginPage()
{
    ui->lineEdit_email->clear();
    ui->lineEdit_PW->clear();
    ui->stackedWidget->setCurrentWidget(ui->login_page);
}

void MainWindow::getVPNProfiles()
{
    api.VPNProfileRequest(user);
}

void MainWindow::setVPNPage()
{
    api.getPublicIP(user.m_show_ip, "ipv4");
    api.getPublicIP(user.m_show_ip, "ipv6");

    //Clear the VPNListWidget if it contains old entries
    if(ui->VPNListWidget->count()>0) ui->VPNListWidget->clear();

    ui->stackedWidget->setCurrentWidget(ui->vpn_page);

    //Creating list widget items which later will contain the vpnItems
    for(int i = 0; i< user.list_ovpnProfiles.count(); i++) {
        //Creating a new list widget item whose parent is the listwidget itself
        QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->VPNListWidget);

        //Adding the item to the listwidget
        ui->VPNListWidget->addItem (listWidgetItem);

        //Creating an object of the designed widget which is to be added to the listwidget
        QFile names_file;
        names_file.setFileName(user.list_ovpnProfiles[i].names_path);
        names_file.open(QIODevice::ReadOnly | QIODevice::Text);
        QJsonDocument names = QJsonDocument::fromJson(names_file.readAll());
        names_file.close();
    }
    MainWindow::setVPNItemWidgets();
}

bool MainWindow::ping_sort(UserData::ovpnProfile l_profile, UserData::ovpnProfile r_profile)
{
    if(l_profile.ping == "-"){
        return false;
    } else if (r_profile.ping == "-"){
        return true;
    } else {
        return l_profile.ping.toInt() < r_profile.ping.toInt();
    }
}

void MainWindow::setVPNItemWidgets()
{
    QFuture<void> future = QtConcurrent::run(&user, &UserData::setPing);
    //Wait until all pings are available
    QEventLoop loop;
    loop.connect(&user, &UserData::setPingFinished, &loop, &QEventLoop::quit);
    loop.exec();

    //sort the userdata list according to their ping
    std::sort(user.list_ovpnProfiles.begin(), user.list_ovpnProfiles.end(), ping_sort);
    for(int i = 0; i< user.list_ovpnProfiles.count(); i++)
    {
        QListWidgetItem *listWidgetItem = ui->VPNListWidget->item(i);
        //Create a vpnItem for the profile
        user.list_ovpnProfiles[i].vpn_widget_item  = new vpnItem(user.list_ovpnProfiles[i].flag_path, user.list_ovpnProfiles[i].name,
                                                                 user.list_ovpnProfiles[i].profile_path, user.list_ovpnProfiles[i].id,
                                                                 user.list_ovpnProfiles[i].ping ,ovpnProcess, this);

        //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (user.list_ovpnProfiles[i].vpn_widget_item->sizeHint ());

        //Finally adding the itemWidget to the list
        ui->VPNListWidget->setItemWidget (listWidgetItem, user.list_ovpnProfiles[i].vpn_widget_item);

    }
    //check if there is a connection
    MainWindow::checkActiveConnection();
}

void MainWindow::on_settingCheckUpdateButtonClicked()
{
    QCoreApplication::applicationDirPath();
    QProcess process;
    QString product_type = QSysInfo::productType();
    if(product_type == "windows"){
        process.startDetached(QCoreApplication::applicationDirPath()+"/maintenancetool.exe", QStringList());
    }
}

void MainWindow::on_settingShowIpButtonClicked()
{
    user.m_show_ip = !user.m_show_ip;
    app_settings.setShowIp(user.m_show_ip);
    emit setOnOffButton(user.m_show_ip);
}

void MainWindow::setupSettingpage()
{
    //add settings title
    QListWidgetItem *listWidgetItem_settingsTitle = new QListWidgetItem(ui->settingsListWidget);
    ui->settingsListWidget->addItem(listWidgetItem_settingsTitle);
    m_settingCheckUpdate = new SettingsItem(&on_settingCheckUpdateButtonClicked , "SettingsTitle", "", false, this);
    listWidgetItem_settingsTitle->setSizeHint(m_settingCheckUpdate->sizeHint());
    ui->settingsListWidget->setItemWidget(listWidgetItem_settingsTitle, m_settingCheckUpdate);

    //add check update setting
    QListWidgetItem *listWidgetItem_checkUpdates = new QListWidgetItem(ui->settingsListWidget);
    ui->settingsListWidget->addItem(listWidgetItem_checkUpdates);
    m_settingCheckUpdate = new SettingsItem(&on_settingCheckUpdateButtonClicked,
                                            QCoreApplication::translate("SettingItem check updates: settings name", "Start maintenance tool to update SnowHaze VPN"),
                                            QCoreApplication::translate("SettingItem check updates: settings button text", "Start"),
                                            false,
                                            this);
    listWidgetItem_checkUpdates->setSizeHint(m_settingCheckUpdate->sizeHint());
    ui->settingsListWidget->setItemWidget(listWidgetItem_checkUpdates, m_settingCheckUpdate);

    //add show IP setting
    QListWidgetItem *listWidgetItem_showIp = new QListWidgetItem(ui->settingsListWidget);
    ui->settingsListWidget->addItem(listWidgetItem_showIp);
    m_settingShowIp = new SettingsItem([](){},
                                        QCoreApplication::translate("SettingItem check updates: settings name", "Display the IP"),
                                        "on_off",
                                        user.m_show_ip,
                                        this);
    connect(m_settingShowIp, &SettingsItem::onButtonClicked, this, &MainWindow::on_settingShowIpButtonClicked);
    listWidgetItem_showIp->setSizeHint(m_settingShowIp->sizeHint());
    ui->settingsListWidget->setItemWidget(listWidgetItem_showIp, m_settingShowIp);

    //Add the licences widget.
    QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->settingsListWidget);

    //Adding the item to the listwidget
    ui->settingsListWidget->addItem (listWidgetItem);
    m_licenceWidget  = new licencesWidget(this);
    listWidgetItem->setSizeHint(m_licenceWidget->sizeHint());
    ui->settingsListWidget->setItemWidget(listWidgetItem, m_licenceWidget);

}

void MainWindow::on_SGXVerificationButton_clicked()
{
    emit requestSGXVerification(app_settings.getEncryptedTokenMessage());
    QEventLoop loop;
    loop.processEvents();
}

void MainWindow::on_refreshButton_clicked()
{
    ui->refreshButton->hide();
    ui->label_currentIPv4->setText("");
    ui->label_currentIPv6->setText("");
    ui->stackedWidget->setCurrentWidget(ui->waiting_page);

    api.getPublicIP(user.m_show_ip, "ipv4");
    api.getPublicIP(user.m_show_ip, "ipv6");

    QEventLoop loop;
    loop.processEvents();

    setVPNItemWidgets();

    ui->refreshButton->show();
    ui->stackedWidget->setCurrentWidget(ui->vpn_page);

    return;
}

void MainWindow::on_labelGetSubscriptionClicked()
{
    QDesktopServices::openUrl(QUrl(QString(SNOWHAZE_DASHBOARD_URL) + "en/select_plan.html"));
}

void MainWindow::on_labelLogoutClicked()
{
    emit logoutWarning();
}

void MainWindow::on_labelSettingsClicked()
{
    ui->stackedWidget->setCurrentWidget(ui->settings_page);
}

void MainWindow::on_labelbackClicked()
{
    ui->stackedWidget->setCurrentWidget(ui->vpn_page);
}

void MainWindow::on_labelonoffClicked()
{
    if (ui->label_on_off->m_bool_on){
        ovpnProcess.disconnect();
        ui->label_on_off->m_bool_on = false;
        ui->label_on_off->setPixmap(QPixmap(":/img/img/off_button.svg"));

    } else {
        MainWindow::showWaitingPage();
        ovpnProcess.connect(user.list_ovpnProfiles[0].profile_path,  user.list_ovpnProfiles[0].id, user.list_ovpnProfiles[0].name);
        MainWindow::showVPNPage();
    }
}

void MainWindow::labelOnOffSetToConnected()
{
    ui->label_on_off->m_bool_on = true;
    ui->label_on_off->setPixmap(QPixmap(":/img/img/on_button.svg"));
}

void MainWindow::labelOnOffSetToDisonnected()
{
    ui->label_on_off->m_bool_on = false;
    ui->label_on_off->setPixmap(QPixmap(":/img/img/off_button.svg"));
}

void MainWindow::checkIpForActiveConnection()
{
    app_settings.getIpIdMap(user);
    if(user.m_ip_id_map.contains(user.m_public_ipv4)){
        ovpnProcess.setToConnected(user.m_ip_id_map.value(user.m_public_ipv4).toString());
        MainWindow::setActiveConnection(true, user.m_ip_id_map.value(user.m_public_ipv4).toString());
    } else {
        ovpnProcess.setToDisconnected();
        MainWindow::setActiveConnection(false);
    }
}

void MainWindow::checkActiveConnection()
{
    app_settings.getIpIdMap(user);
    QString route_4_response;
    QString route_6_response;
    if (QSysInfo::productType() == "windows"){
        QProcess route4_process;
        QStringList route_arg_4;
        route_arg_4 << "print" << "-4";
        route4_process.start("route",route_arg_4);
        route4_process.waitForReadyRead();
        route_4_response = route4_process.readAll();
        route4_process.terminate();
        QProcess route6_process;
        QStringList route_arg_6;
        route_arg_6 << "print" << "-6";
        route6_process.start("route",route_arg_6);
        route6_process.waitForReadyRead();
        route_6_response = route6_process.readAll();
        route6_process.terminate();

    } else {

    }
    QString shownet_response = ovpnProcess.getOVPNShowNetReply();

    //check whether the response contains an ip corresponding to a host
    for(QVariant v_ip: user.m_id_ip_map.values())
    {
        QString ip = v_ip.toString();
        if(route_4_response.contains(ip) or route_6_response.contains(ip))
        {
            //Check if the shownet reponse shows a connection to the firewall bypass server
            if(shownet_response.contains(utils::m_ch_firewall_bypass_ipv4) or shownet_response.contains(utils::m_ch_firewall_bypass_ipv6))
            {
                MainWindow::setActiveConnection(true,utils::m_id_ch_firewall_bypass);
                ovpnProcess.setToConnected(utils::m_id_ch_firewall_bypass);
                ovpnProcess.m_connection_detected = true;
                return;

            } else {
                for (QVariant id: user.m_ip_id_map.values(ip)){
                    // if the found ip does not correspond to the ch_firewall-bypass server -> set connection
                    if(!(utils::m_id_ch_firewall_bypass==id.toString())){
                        ovpnProcess.setActiveConnection(true, id.toString());
                        ovpnProcess.setToConnected(id.toString());
                        ovpnProcess.m_connection_detected = true;
                        return;
                    }
                }
            }
        }
    }
    ovpnProcess.m_connection_detected = false;
}

void MainWindow::setActiveConnection(bool set_flag, QString id)
{
    if(set_flag){
        for (UserData::ovpnProfile profile : user.list_ovpnProfiles){
            if(profile.id == id){
                ui->label_currentVPNFlag->setPixmap(profile.flag_path);
                ui->label_currentVPNFlag->show();
                ui->label_currentVPNName->setText(profile.name);
            }
        }
    } else {
         ui->label_currentVPNFlag->hide();
         ui->label_currentVPNName->setText(QCoreApplication::translate("label_currentVPNName no active connection", "No active connection"));
    }
}

void MainWindow::showWaitingPage()
{
    ui->stackedWidget->setCurrentWidget(ui->waiting_page);
}

void MainWindow::showVPNPage()
{
    ui->stackedWidget->setCurrentWidget(ui->vpn_page);
}
