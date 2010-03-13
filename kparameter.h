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
