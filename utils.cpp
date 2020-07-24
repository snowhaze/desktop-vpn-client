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

#include "utils.h"
const QRegExp utils::m_ch_firewall_bypass_ipv4 = QRegExp("IP = 10.10.[0-9]{1,3}.[0-9]{1,3}/");
const QRegExp utils::m_ch_firewall_bypass_ipv6 = QRegExp("IP = fdbe:2000:0?a0a:0{0,2}10(:(:|[0-9a-f]{0,4}))+/");
const QString utils::m_id_ch_firewall_bypass = QString("6");

utils::utils()
{

}

