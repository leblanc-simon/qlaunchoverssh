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

#ifndef KSSH_H
#define KSSH_H

#include <QString>

#include "libssh/libssh.h"

class Kssh
{
private:
    /* The variable with the SSH session */
    ssh_session m_session;

protected:
    QString m_command;
    QString m_return;

    void    init();

    /* Connection method */
    bool    launch_connect(QString server, QString login, int port = 22);
    bool    check_known_host();
    bool    authenticate(QString private_key = "", QString password = "");
    bool    authenticateKey(QString private_key, QString password = "");
    bool    authenticatePassword(QString password);
    bool    authenticatePasswordInteractive(QString password);

public:
    /* Constructor and destructor */
            Kssh();
            ~Kssh();

    /* Connection method */
    bool    connect(QString server, QString login, QString private_key = "", QString password = "", int port = 22);
    bool    disconnect();

    /* Action method */
    bool    launch(QString command);
    QString getLastCommand();
    QString getReturn();

};

#endif // KSSH_H
