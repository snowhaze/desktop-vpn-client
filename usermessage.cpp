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

#include "usermessage.h"


UserMessage::UserMessage()
{

}

void UserMessage::userMasterSecretError(int status_code)
{
    QMessageBox msgBox;
    switch (status_code) {
    case 1:
        msgBox.setText(QCoreApplication::translate("msgBox user set master secret error title", "Invalid master secret"));
        msgBox.setInformativeText(QCoreApplication::translate("msgBox user set master secret error text", "The entered master secret has an incorrect length."));
        break;
    case 2:
        msgBox.setText(QCoreApplication::translate("msgBox user set master secret error title", "Invalid master secret"));
        msgBox.setInformativeText(QCoreApplication::translate("msgBox user set master secret error text", "The entered master secret has an unsupported version."));
        break;
    case 3:
        msgBox.setText(QCoreApplication::translate("msgBox user set master secret error title", "Invalid master secret"));
        msgBox.setInformativeText(QCoreApplication::translate("msgBox user set master secret error text", "Please enter a valid master secret."));
        break;
    default:
        msgBox.setText(QCoreApplication::translate("msgBox user set master secret error title", "Invalid master secret"));
        msgBox.setInformativeText(QCoreApplication::translate("msgBox user set master secret error text", "Please enter a valid master secret."));
        break;
    }
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        emit goToLoginPage();
        break;
    default:
        emit goToLoginPage();
        break;
    }
}

void UserMessage::invalidSubscription()
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox invalide subscription title", "Invalid subscription"));
    msgBox.setInformativeText(QCoreApplication::translate("msgBox invalide subscription text", "This account has no valid subscription."));
    msgBox.setIcon(QMessageBox::Warning);
    QPushButton *getSubscriptionButton = msgBox.addButton(QCoreApplication::translate("msg invalid subscription getSubscription button", "Get subscription"), QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == getSubscriptionButton) {
        QDesktopServices::openUrl(QUrl(QString(SNOWHAZE_DASHBOARD_URL) + "en/select_plan.html"));
        emit requestLogout();
    } else {
        emit requestLogout();
    }
}

void UserMessage::publicKeyNotRegistered()
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox publickey not registered title", "Public key not registered"));
    msgBox.setInformativeText(QCoreApplication::translate("msgBox publickey not registered text", "Log into your dashboard for more information."));
    msgBox.setIcon(QMessageBox::Warning);
    QPushButton *getSubscriptionButton = msgBox.addButton(QCoreApplication::translate("msgpublickey not registered button", "Go to dashboard"), QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == getSubscriptionButton) {
        QDesktopServices::openUrl(QUrl(QString(SNOWHAZE_DASHBOARD_URL) + "en/login.html"));
        emit requestLogout();
    } else {
        emit requestLogout();
    }
}

void UserMessage::processError(QString process)
{
    QMessageBox msgBox;
    if (process == "python"){
        msgBox.setText(QCoreApplication::translate("msgBox python process failed", "Unable to run Python process"));
    } else if(process == "openvpn"){
        msgBox.setText(QCoreApplication::translate("msgBox openvpn process failed", "Unable to run OpenVPN process"));
    } else {
        msgBox.setText(QCoreApplication::translate("msgBox default process failed", "Unable to run process"));
    }
    msgBox.setInformativeText(QCoreApplication::translate("msgBox python process failed text", "SnowHaze VPN was not able to start the verification process. Try to reinstall SnowHaze VPN to solve this problem. If this error message perists reach out to the SnowHaze support."));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QPushButton *contactSupportButton = msgBox.addButton(QCoreApplication::translate("msgBox process failed contact-button", "Contact support"), QMessageBox::ActionRole);
    msgBox.exec();
    if (msgBox.clickedButton() == contactSupportButton) {
        QDesktopServices::openUrl(QUrl(QString(SNOWHAZE_WEBSITE_URL) + "en/support-contact.html"));
    } else {

    }
}

void UserMessage::vpnProcessTimeoutError()
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox ovpnProcess timeout error title", "SnowHaze VPN is unable to detect a successful VPN connection. Make sure you have an active internet connection."));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        break;
    default:
        break;
    }
}

void UserMessage::loadVPNProfilesFailed()
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox load VPN profiles failed", "SnowHaze VPN was unable to load the localy stored profiles."));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        emit requestLogout();
        break;
    default:
        break;
    }
}

void UserMessage::logoutWarning()
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox logout warning", "Do you want to logout from SnowHaze VPN? This will delete all user specific data on your device."));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.addButton(QMessageBox::No);
    msgBox.addButton(QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        emit requestLogout();
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}

void UserMessage::encryptionFailure(QString error)
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox libsodium failure", "An encryption failure occured"));
    msgBox.setInformativeText(QCoreApplication::translate("msgBox libsodium failure text", "Libsodium error message: ") + error);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        break;
    default:
        break;
    }
}

void UserMessage::overusedTokensWarning()
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox oversued tokens warning", "Overused tokens"));
    msgBox.setInformativeText(QCoreApplication::translate("msgBox oversued tokens warning text", "Every subscription has a certain number of tokens which can be used to get the VPN profiles. If all these tokens were used too many times to recieve the VPN profiles they become invalid. This prevents abuse of the subscription. In a few days your tokens renew and you will be able to download the VPN profiles again."));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        emit requestLogout();
        break;
    default:
        emit requestLogout();
        break;
    }
}

bool UserMessage::missingDependecyWarning(QString dependency)
{
    QMessageBox msgbox;
    msgbox.setText(QCoreApplication::translate("msg missing Dependency text 1", "SnowHaze VPN depends on ") + dependency + QCoreApplication::translate("msg missing Dependency text 2", ". If ") + dependency + QCoreApplication::translate("msg missing Dependency text 3", " is not installed, SnowHaze VPN will not be able to establish a VPN connection."));
    msgbox.setIcon(QMessageBox::Icon::Information);
    QPushButton *getButton = msgbox.addButton(QCoreApplication::translate("msg missing Dependency get dependency button", "Get " ) + dependency, QMessageBox::ActionRole);
    QPushButton *alreadyHaveButton = msgbox.addButton(dependency + QCoreApplication::translate("msg missing Dependency get dependency button", " is installed" ) , QMessageBox::ActionRole);

    msgbox.setDefaultButton(alreadyHaveButton);

    msgbox.exec();
    if( msgbox.clickedButton() == getButton){
        if(dependency == "OpenVPN Gui"){
            QDesktopServices::openUrl(QUrl(OPENVPN_DOWNLOADS_URL));
        }
        return false;
    } else if( msgbox.clickedButton() == alreadyHaveButton) {
        return false;
    }
    return true;
}

void UserMessage::memLockErrorWarning()
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox wrong memLock error warning title", "Memory lock error"));
    msgBox.setInformativeText(QCoreApplication::translate("msgBox wrong memLock error warning text", "SnowHaze VPN was not able to lock the memory. Therefore, sensitive data might be swaped to the disk."));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        break;
    default:
        break;
    }
}

void UserMessage::wrongCredentials()
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox wrong login credentials title", "Wrong login credentials"));
    msgBox.setInformativeText(QCoreApplication::translate("msgBox wrong login credentials text", "Your email and/or password is wrong."));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        emit goToLoginPage();
        break;
    default:
        emit goToLoginPage();
        break;
    }
}

void UserMessage::networkError(int status_code)
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox network error title", "Network error ") + QString::number(status_code));
    msgBox.setInformativeText(QCoreApplication::translate("msgBox network error text", "Connecting to server failed. Please check your internet connection."));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        emit requestLogout();
        break;
    default:
        emit requestLogout();
    }
}

void UserMessage::QKeychainError()
{
    QMessageBox msgBox;
    msgBox.setText(QCoreApplication::translate("msgBox QKeychain error title", "Local storage error"));
    msgBox.setInformativeText(QCoreApplication::translate("msgBox QKeychain text", "SnowHaze VPN failed to securely store data on your device."));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        emit requestLogout();
        break;
    default:
        emit requestLogout();
    }
}
