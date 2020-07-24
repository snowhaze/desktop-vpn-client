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

#include "ovpnprocess.h"

OVPNProcess::OVPNProcess(QObject *parent) : QObject(parent) //UserData &user,
{
    m_disconnect_arguments << "--command" << "disconnect_all";
    m_exit_arguments << "--command" << "exit";
}

void OVPNProcess::initializePaths(QString vpn_profiles_path)
{
    m_openVPN_path = QCoreApplication::applicationDirPath();
    m_openVPN_GUI_path = QCoreApplication::applicationDirPath();
    m_config_file_path = vpn_profiles_path;

    m_operating_system = QSysInfo::productType();

    if(m_operating_system == "windows"){
        m_openVPN_path.append("/binaries/ovpn/openvpn.exe");
        m_openVPN_GUI_path.append("/binaries/ovpn/openvpn-gui.exe");
        process.startDetached(m_openVPN_GUI_path, QStringList());

    } else if (m_operating_system == "osx") {

    } else {

    }
}

void OVPNProcess::connect(QString, QString id, QString profile_name)
{
    if(m_operating_system == "windows"){
        //stop any while loops which may be still running in the background
        m_run_loop = false;

        //Disconnect from possible active connections
        OVPNProcess::disconnect();

        QStringList connect_arguments;
        connect_arguments << "--connect" << profile_name;  // + "_" + id "--config_dir" << m_config_file_path <<

        process.startDetached(m_openVPN_GUI_path, connect_arguments);
        QThread::msleep(1100);
        QElapsedTimer timer;
        timer.start();

        m_ip_updated=false;
        m_connection_detected = false;
        m_run_loop = true;

        while (m_run_loop)
        {
            if(OVPNProcess::activeConnection()){
                break;
            }

            if ( 25000 < timer.elapsed() )
            {
                process.execute(m_openVPN_GUI_path, m_disconnect_arguments);
                emit setToDisconnected();
                emit setActiveConnection(false, id);
                emit vpnProcessTimeoutError();
                break;
            }
        }

    } else if (m_operating_system == "osx"){

    } else {

    }
    emit setVPNPage();
    return;
}

void OVPNProcess::disconnect(bool)
{
    if(m_operating_system == "windows"){

        process.execute(m_openVPN_GUI_path, m_disconnect_arguments);
        process.execute(m_openVPN_GUI_path, m_exit_arguments);
        process.waitForFinished();

        emit setActiveConnection(false, "");
        emit setToDisconnected();

    } else if (m_operating_system == "osx"){

    } else {
        process.terminate();
        process.kill();
        process.waitForFinished();
        emit setActiveConnection(false, "");
        emit setToDisconnected();
    }
    return;
}

QString OVPNProcess::getOVPNShowNetReply()
{
    QProcess ovpn_shownet_process;
    QStringList shownet_arg;
    shownet_arg << "--show-net";
    ovpn_shownet_process.start(m_openVPN_path, shownet_arg);
    ovpn_shownet_process.waitForReadyRead();
    QString shownet_response = ovpn_shownet_process.readAll();
    QElapsedTimer timer;
    timer.start();
    while( ovpn_shownet_process.state() == QProcess::Running){
        ovpn_shownet_process.waitForReadyRead();
        shownet_response += ovpn_shownet_process.readAll();
        if (timer.elapsed()> 1500){
            ovpn_shownet_process.kill();
            return shownet_response;
        }
    }
    return shownet_response;
}

bool OVPNProcess::activeConnection()
{
    QEventLoop loop;
    QTimer timer;
    timer.start(1500);
    loop.connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    emit checkActiveConnectionSignal();
    loop.exec();

    return m_connection_detected;
}



