/*
Copyright (c) 2010, Simon Leblanc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification
, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice
    , this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
    * Neither the name of the Simon Leblanc nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef KCONFIG_H
#define KCONFIG_H

#define CONFIG_FILE                 "./config.xml"
#define KCONFIG_DEFAULT_DATABASE    "./qlaunchoverssh.sqlite"
#define KCONFIG_DEFAULT_LOG         "./qlaunchoverssh.log"
#define KCONFIG_DEFAULT_KNOWNHOSTS  "./ssh/known_hosts"
#define KCONFIG_DEFAULT_PRIVATEKEY  "./ssh/id_dsa"
#define KCONFIG_DEFAULT_PASSPHRASE  false

#include <QString>

class Kconfig
{
protected:
    static QString  m_database;
    static QString  m_log;
    static QString  m_known_hosts;
    static QString  m_private_key;
    static bool     m_passphrase;

public:
    static bool     load(QString filename);

    static QString  getDatabase()   { return Kconfig::m_database; }
    static QString  getLog()        { return Kconfig::m_log; }
    static QString  getKnownHosts() { return Kconfig::m_known_hosts; }
    static QString  getPrivateKey() { return Kconfig::m_private_key; }
    static bool     hasPassphrase() { return Kconfig::m_passphrase; }
};

#endif // KCONFIG_H
