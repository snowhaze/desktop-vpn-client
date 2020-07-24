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

#include "urlencoding.h"


static bool is_safe(const char c) {
    return (c >= 'A' && c <= 'Z')
            || (c >= 'a' && c <= 'z')
            || (c >= '0' && c <= '9')
            || (c == '_')
            || (c == '.')
            || (c == '-');
}

QByteArray url_encode(const QByteArray& str)
{
    QByteArray encoded;
    QTextStream outstream(&encoded);
    outstream.setIntegerBase(16);
    outstream.setNumberFlags(QTextStream::UppercaseDigits);
    outstream.setFieldAlignment(QTextStream::AlignRight);
    outstream.setPadChar('0');

    for (const auto &c : str) {
        outstream.setFieldWidth(0);
        if (is_safe(c)){
            outstream << c;
        }
        else {
            outstream << '%';
            outstream.setFieldWidth(2);
            outstream << static_cast<int>(c);
        }
    }
    return encoded;
}
