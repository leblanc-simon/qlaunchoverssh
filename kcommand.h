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
