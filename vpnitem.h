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

#ifndef VPNITEM_H
#define VPNITEM_H

#include <QWidget>
#include <QtConcurrent>
#include "ovpnprocess.h"


namespace Ui {
class vpnItem;
}

class vpnItem : public QWidget
{
    Q_OBJECT

public:
    explicit vpnItem(QString flag_path, QString name, QString profile_path, QString id, QString ping, OVPNProcess &ovpn_process, QWidget *parent = nullptr);
    ~vpnItem();
    void setConnectDisconnectButton(QString status);


signals:
    void ovpnConnect(QString, QString, QString);
    void ovpnDisonnect();
    void showWaitingPage();
    void showVPNPage();
public slots:
    void setPing(QString ping);

private slots:
    void on_ConnectDisconnectButton_clicked();
    void setToConnected(QString profile_path);
    void setToDisconnected();

private:
    Ui::vpnItem *ui;
    QString m_profile_path;
    QString m_id;
    QString m_name;
    bool bool_connected;
    OVPNProcess process;

    QString stylesheet_connected;
    QString stylesheet_disconnected;

};

#endif // VPNITEM_H
