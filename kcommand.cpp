#include "kcommand.h"
#include "ksql.h"

#include <QList>
#include <QString>
#include <QSqlQuery>
#include <QVariant>

/**
 * initialize the values of the object
 */
void Kcommand::init()
{
    this->m_new         = true;
    this->m_id          = 0;
    this->m_name        = QString("");
    this->m_server      = QString("");
    this->m_login       = QString("");
    this->m_use_key     = false;
    this->m_command     = QString("");
    this->m_passwd      = QString("");
    this->m_parameters.clear();
}


/**
 * update the database with the values of the object
 * @return  bool    true if the database is updated, false else
 */
bool Kcommand::update()
{
    if (this->m_new == true) {
        return false;
    }

    QString sql = QString("UPDATE command SET name = '%1', server = '%2', login = '%3', use_key = '%4', command = '%5', passwd = '%6'")
                  .arg(Ksql::clean(this->m_name))
                  .arg(Ksql::clean(this->m_server))
                  .arg(Ksql::clean(this->m_login))
                  .arg((int)this->m_use_key)
                  .arg(Ksql::clean(this->m_command))
                  .arg(Ksql::clean(this->m_passwd));

    QSqlQuery query = Ksql::exec(sql);

    if (query.numRowsAffected() != 1) {
        return false;
    }

    return true;
}


/**
 * insert the values of the object in the database
 * @return  bool    true if the object is inserted, false else
 */
bool Kcommand::insert()
{
    if (this->m_new != true) {
        return false;
    }

    QString sql = QString("INSERT INTO command (name, server, login, use_key, command, passwd) "
                          "VALUES ('%1', '%2', '%3', '%4', '%5', '%6')")
                            .arg(Ksql::clean(this->m_name))
                            .arg(Ksql::clean(this->m_server))
                            .arg(Ksql::clean(this->m_login))
                            .arg((int)this->m_use_key)
                            .arg(Ksql::clean(this->m_command))
                            .arg(Ksql::clean(this->m_passwd));

    QSqlQuery query = Ksql::exec(sql);

    if (query.numRowsAffected() != 1) {
        return false;
    }

    this->setId(query.lastInsertId().toInt());

    return true;
}


/**
 * load a command in this object
 * @param   int     id      the id of the command
 * @return  bool            true if the object is loaded, false else
 */
bool Kcommand::load(int id)
{
    QString sql = QString("SELECT id, name, server, login, use_key, command, passwd FROM command WHERE id = %1").arg(id);
    QSqlQuery query = Ksql::exec(sql);

    if (query.first()) {
         return this->load(query);
    }

    return false;
}


/**
 * load a command in this object
 * @param   QSqlQuery   query   the query to load
 * @return  bool                true if the object is loaded, false else
 */
bool Kcommand::load(QSqlQuery query)
{
    this->init();

    if (query.isValid() == false || query.isActive() == false || query.isSelect() == false) {
        return false;
    }

    try {
        this->setId(query.value(0).toInt());
        this->setName(query.value(1).toString());
        this->setServer(query.value(2).toString());
        this->setLogin(query.value(3).toString());
        this->setUseKey(query.value(4).toInt());
        this->setCommand(query.value(5).toString());
        this->setPasswd(query.value(6).toString());

        this->m_new = false;

        this->setParameters();

        return true;
    } catch (...) {
        this->init();
        return false;
    }
}


/**
 * initialize the parameters for the command
 */
void Kcommand::setParameters()
{
    // init the var
    this->m_parameters.clear();

    // check if the object is new
    if (this->m_new == true) {
        return;
    }

    // add all parameters in the array
    QString sql = QString("SELECT id, name, position, id_command, explain, required FROM get_parameters WHERE id_command = %1").arg(this->m_id);
    QSqlQuery query = Ksql::exec(sql);
    Kparameter parameter;

    while (query.next()) {
        parameter = Kparameter();
        if (parameter.load(query)) {
            this->m_parameters.append(parameter);
        }
    }
}


/**
 * Delete all parameters for the object
 */
void Kcommand::clearParameter()
{
    // check if the object is new
    if (this->m_new == true) {
        return;
    }

    QString sql = QString("DELETE FROM parameter WHERE id_command = %1").arg(this->m_id);
    QSqlQuery query = Ksql::exec(sql);

    // clean the parameters
    this->m_parameters.clear();
}


/**
 * get the number of parameter for the command
 * @return  int     the number of parameter of the command
 */
int Kcommand::getNbParameter()
{
    return this->m_parameters.size();
}


/**
 * get the parameter define by the identifier
 * @param   int         i   the identifier of the parameter
 * @return  Kparameter      the  parameter
 */
Kparameter Kcommand::getParameter(int i)
{
    if (i >= this->m_parameters.size()) {
        throw QString("Impossible to get the parameter %1 in the array of %2 items")
                .arg(i)
                .arg(this->m_parameters.size());
    }

    return this->m_parameters.at(i);
}


/**
 * get the list of the available command
 * @return  QList<Kcommand>     the list of available command
 */
QList<Kcommand> Kcommand::doSelect()
{
    QList<Kcommand> list;
    list.clear();

    QString sql = QString("SELECT id, name, server, login, use_key, command, passwd FROM command ORDER BY name ASC");
    QSqlQuery query = Ksql::exec(sql);

    while (query.next()) {
        Kcommand command = Kcommand();
        if (command.load(query)) {
            list.append(command);
        }
    }

    return list;
}
