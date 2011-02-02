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

#ifndef KCOMMAND_H
#define KCOMMAND_H

#include "kparameter.h"

#include <QList>
#include <QString>
#include <QSqlQuery>

class Kcommand
{
protected:
    bool                m_new;
    int                 m_id;
    QString             m_name;
    QString             m_server;
    QString             m_login;
    bool                m_use_key;
    QString             m_command;
    QString             m_passwd;
    QList<Kparameter>   m_parameters;

    bool        update();
    bool        insert();
    bool        load(QSqlQuery query);

    void        init();

    void        setParameters();
    void        clearParameter();

public:
                                Kcommand()      { this->init(); }

            void                setId(int value)            { this->m_id = value; }
            void                setName(QString value)      { this->m_name = value; }
            void                setServer(QString value)    { this->m_server = value; }
            void                setLogin(QString value)     { this->m_login = value; }
            void                setUseKey(bool value)       { this->m_use_key = value; }
            void                setUseKey(int value)        { if (value == 0) { this->m_use_key = false; } else { this->m_use_key = true; } }
            void                setCommand(QString value)   { this->m_command = value; }
            void                setPasswd(QString value)    { this->m_passwd = value; }

            int                 getId()         { return this->m_id; }
            QString             getName()       { return this->m_name; }
            QString             getServer()     { return this->m_server; }
            QString             getLogin()      { return this->m_login; }
            bool                getUseKey()     { return this->m_use_key; }
            QString             getCommand()    { return this->m_command; }
            QString             getPasswd()     { return this->m_passwd; }
            QList<Kparameter>   getParameters() { return this->m_parameters; };

            bool                load(int id);

            bool                save()          { if (this->m_new == true) { return this->insert(); } else { return this->update(); } }

            int                 getNbParameter();
            Kparameter          getParameter(int i);

    static  QList<Kcommand>     doSelect();
};

#endif // KCOMMAND_H
