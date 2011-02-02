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

#ifndef KPARAMETER_H
#define KPARAMETER_H

#include <QString>
#include <QSqlQuery>

class Kparameter
{
protected:
    bool        m_new;
    int         m_id;
    QString     m_name;
    int         m_position;
    int         m_command_id;
    QString     m_explain;
    bool        m_required;

    bool        update();
    bool        insert();

    void        init();

    int         getMaxPosition(int command_id);

public:
                Kparameter()    { this->init(); }

    void        setId(int value)            { this->m_id = value; }
    void        setName(QString value)      { this->m_name = value; }
    void        setPosition(int value)      { this->m_position = value; }
    void        setCommandId(int value)     { this->m_command_id = value; }
    void        setExplain(QString value)   { this->m_explain = value; }
    void        setRequired(bool value)     { this->m_required = value; }
    void        setRequired(int value)      { if (value == 0) { this->m_required = false; } else { this->m_required = true; } }

    int         getId()         { return this->m_id; }
    QString     getName()       { return this->m_name; }
    int         getPosition()   { return this->m_position; }
    int         getCommandId()  { return this->m_command_id; }
    QString     getExplain()    { return this->m_explain; }
    bool        getRequired()   { return this->m_required; }

    bool        load(int id);
    bool        load(QSqlQuery query);

    bool        save()          { if (this->m_new == true) { return this->insert(); } else { return this->update(); } }
};

#endif // KPARAMETER_H
