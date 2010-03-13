#include "kparameter.h"
#include "ksql.h"

#include <QString>
#include <QSqlQuery>
#include <QVariant>


/**
 * initialize the values of the object
 */
void Kparameter::init()
{
    this->m_new         = true;
    this->m_id          = 0;
    this->m_name        = QString("");
    this->m_position    = 0;
    this->m_command_id  = 0;
    this->m_explain     = QString("");
    this->m_required    = true;
}


/**
 * update the database with the values of the object
 * @return  bool    true if the database is updated, false else
 */
bool Kparameter::update()
{
    if (this->m_new == true) {
        return false;
    }

    QString sql = QString("UPDATE parameter SET name = '%1', position = '%2', id_command = '%3', explain = '%4', required = '%5'")
                  .arg(Ksql::clean(this->m_name))
                  .arg(this->m_position)
                  .arg(this->m_command_id)
                  .arg(Ksql::clean(this->m_explain))
                  .arg((int)this->m_required);

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
bool Kparameter::insert()
{
    if (this->m_new != true || this->m_command_id == 0) {
        return false;
    }

    if (this->m_position == 0) {
        this->setPosition(this->getMaxPosition(this->m_command_id));
    }

    QString sql = QString("INSERT INTO parameter (name, position, id_command, explain, required) "
                          "VALUES ('%1', '%2', '%3')")
                    .arg(Ksql::clean(this->m_name))
                    .arg(this->m_position)
                    .arg(this->m_command_id)
                    .arg(Ksql::clean(this->m_explain))
                    .arg((int)this->m_required);

    QSqlQuery query = Ksql::exec(sql);

    if (query.numRowsAffected() != 1) {
        return false;
    }

    this->setId(query.lastInsertId().toInt());

    return true;
}


/**
 * get the maximun position for the parameters of the command get in parameter of this function
 * @param   int     command_id  the command for which you want get the maximun position
 * @return  int                 the maximun position
 */
int Kparameter::getMaxPosition(int command_id)
{
    QString sql = QString("SELECT max FROM max_position WHERE id_command = %1").arg(command_id);
    QSqlQuery query = Ksql::exec(sql);

    int max = 0;

    if (query.first()) {
         max = query.value(0).toInt();
    }

    return max;
}


/**
 * load a parameter in this object
 * @param   int     id      the id of the parameter
 * @return  bool            true if the object is loaded, false else
 */
bool Kparameter::load(int id)
{
    QString sql = QString("SELECT id, name, position, id_command, explain, required FROM parameter WHERE id = %1").arg(id);
    QSqlQuery query = Ksql::exec(sql);

    if (query.first()) {
         return this->load(query);
    }

    return false;
}


/**
 * load a parameter in this object
 * @param   QSqlQuery   query   the query to load
 * @return  bool                true if the object is loaded, false else
 */
bool Kparameter::load(QSqlQuery query)
{
    this->init();

    if (query.isValid() == false || query.isActive() == false || query.isSelect() == false) {
        return false;
    }

    try {
        this->setId(query.value(0).toInt());
        this->setName(query.value(1).toString());
        this->setPosition(query.value(2).toInt());
        this->setCommandId(query.value(3).toInt());
        this->setExplain(query.value(4).toString());
        this->setRequired(query.value(5).toInt());

        this->m_new = false;

        return true;
    } catch (...) {
        this->init();
        return false;
    }
}
