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

#include "sgxverification.h"

sgxVerification::sgxVerification()
{

}

void sgxVerification::verifyTokenResponse(QByteArray encrypted_token_message)
{
    QString python = QCoreApplication::applicationDirPath();
    QString python_path_lib = QCoreApplication::applicationDirPath();
    QString python_path_home = QCoreApplication::applicationDirPath();
    QString sgx_verify = QCoreApplication::applicationDirPath();
    QString openssl = QCoreApplication::applicationDirPath();

    // Operating system depending variables
    QString operating_system = QSysInfo::productType();
    if(operating_system == "windows"){

        python.append("/binaries/python/python.exe");
        python_path_lib.append("/binaries/python/Lib");
        python_path_home.append("/binaries/python");
        sgx_verify.append("/binaries/sgxVerification/verify.py");
        openssl.append("/binaries/ssl/openssl.exe");

    } else if (operating_system == "osx") {

    } else {

    }

    QProcess sgxVerifyProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PYTHONPATH", python_path_lib);
    env.insert("PYTHONHOME", python_path_home);
    sgxVerifyProcess.setProcessEnvironment(env);

    encrypted_token_message.prepend(1);
    QString verification_blob = encrypted_token_message.toBase64();

    QStringList arguments;
    arguments << sgx_verify << "-o" << openssl << verification_blob;


    sgxVerifyProcess.start(python, arguments);

    if(!sgxVerifyProcess.waitForStarted()){
        emit processError("python");
    }
    if(!sgxVerifyProcess.waitForReadyRead()){
        emit processError("python");
    }

    QByteArray sgx_verify_return = sgxVerifyProcess.readAll();
    sgxVerifyProcess.terminate();

    QString sgx_v_return = sgx_verify_return;
    LicenceDialog *dialog = new LicenceDialog("sgx_response", sgx_v_return);
    dialog->show();
}
