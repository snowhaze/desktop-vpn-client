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

#include "vpnitem.h"
#include "ui_vpnitem.h"

vpnItem::vpnItem(QString flag_path, QString name, QString profile_path, QString id, QString ping, OVPNProcess &ovpn_process, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vpnItem)
{
    stylesheet_connected = "font: 8pt; background-color: rgb(191, 150, 89); color: rgb(240, 240, 240); border: 1px solid black; border-radius: 5px 3px;"; //padding: 0.28em 1.09em;
    stylesheet_disconnected = "font: 8pt; background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); border: 1px solid black; border-radius: 5px 3px;";

    ui->setupUi(this);
    ui->flag->clear();
    ui->flag->setPixmap(QPixmap(flag_path));
    ui->flag->setScaledContents(true);
    ui->name->setText(name);
    ui->ConnectDisconnectButton->setText(QCoreApplication::translate("vpnItem: connect disconnect button: connect","Connect" ));
    ui->ConnectDisconnectButton->setStyleSheet(stylesheet_disconnected);

    ui->ConnectDisconnectButton->setCursor(QCursor(Qt::PointingHandCursor));

    //set the ping
    setPing(ping);

    m_profile_path = profile_path;
    m_id = id;
    m_name = name;
    bool_connected = false;
    connect(this, SIGNAL(showWaitingPage()), parent, SLOT(showWaitingPage()), Qt::DirectConnection);
    connect(this, SIGNAL(ovpnConnect(QString, QString, QString)), &ovpn_process, SLOT(connect(QString, QString, QString)));
    connect(this, SIGNAL(ovpnDisonnect()), &ovpn_process, SLOT(disconnect()));
    connect(&ovpn_process, SIGNAL(setToConnected(QString)), this, SLOT(setToConnected(QString)));
    connect(&ovpn_process, SIGNAL(setToDisconnected()), this, SLOT(setToDisconnected()));
    connect(this, SIGNAL(showVPNPage()), parent, SLOT(showVPNPage()));

}

vpnItem::~vpnItem()
{
    delete ui;
}

void vpnItem::setConnectDisconnectButton(QString status)
{
    if(status == "connect"){
        ui->ConnectDisconnectButton->setText(QCoreApplication::translate("vpnItem: connect disconnect button: connect", "Connect" ));
        ui->ConnectDisconnectButton->setStyleSheet(stylesheet_disconnected);
    }
    if(status == "disconnect"){
        ui->ConnectDisconnectButton->setText(QCoreApplication::translate("vpnItem: connect disconnect button: disconnect", "Disconnect" ));
        ui->ConnectDisconnectButton->setStyleSheet(stylesheet_connected);
    }
}

void vpnItem::setPing(QString ping)
{
    if(ping == "-"){
        ui->ping->setText("<span style=\" color:rgb(255,0,0);\">" + ping + "</span>");
    } else {
        int ping_int = ping.toInt();
        QString red = QString::fromStdString(std::to_string(std::min(std::max(ping_int-20, 0), 255)));
        QString green = QString::fromStdString(std::to_string(std::max(std::min(255-ping_int+20, 255), 0)));
        ui->ping->setText("<span style=\" color:rgb("+ red +", " + green + ",0);\">" + ping + "</span>");
    }
}

void vpnItem::on_ConnectDisconnectButton_clicked()
{
    emit showWaitingPage();

    if(bool_connected){
        // disconntect
        bool_connected = false;
        setConnectDisconnectButton("connect");
        emit ovpnDisonnect();
    } else {
        emit ovpnConnect(m_profile_path, m_id, m_name);
    }
    emit showVPNPage();
}

void vpnItem::setToConnected(QString id)
{
    if(m_id == id){
        bool_connected = true;
        setConnectDisconnectButton("disconnect");
    }
}

void vpnItem::setToDisconnected()
{
    bool_connected = false;
    setConnectDisconnectButton("connect");
}
