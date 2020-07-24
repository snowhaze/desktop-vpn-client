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

#ifndef OVPNPROCESS_H
#define OVPNPROCESS_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QProcessEnvironment>
#include <QSysInfo>
#include <QElapsedTimer>
#include <QThread>
#include <QtConcurrent>

class OVPNProcess : public QObject
{
    Q_OBJECT
public:
    explicit OVPNProcess(QObject *parent = nullptr);
    void initializePaths(QString vpn_profiles_path);

    bool m_connection_detected;

signals:
    void vpnProcessTimeoutError();
    void setToConnected(QString id);
    void setToDisconnected();
    void setActiveConnection(bool set_flag, QString id);
    void userSetIpId(QString id);
    void checkActiveConnectionSignal();
    void setVPNPage();


public slots:
    void connect(QString profile_path, QString id, QString profile_name);
    void disconnect(bool logout=false);
    QString getOVPNShowNetReply();
    bool activeConnection();

private:
    QProcess process;
    QString m_openVPN_path;
    QString m_openVPN_GUI_path;
    QString m_operating_system;
    QString m_config_file_path;
    QString m_currently_connected_vpnID;
    bool m_ip_updated;
    bool m_run_loop;

    QStringList m_disconnect_arguments;
    QStringList m_exit_arguments;

    void checkIP(QString host);

};

#endif // OVPNPROCESS_H
