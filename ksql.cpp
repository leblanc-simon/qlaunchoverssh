#include "ksql.h"
#include "kconfig.h"

#include <QFile>
#include <QString>

QString      Ksql::m_filename = "";
QSqlDatabase Ksql::m_db       = QSqlDatabase::addDatabase("QSQLITE");
bool         Ksql::m_opened   = false;

Ksql::Ksql()
{
}

Ksql::~Ksql()
{
}

/**
 * Initialize the name of the file to use for database
 * @param   QString filename    The file to use for database
 */
void Ksql::setFilename(QString filename)
{
    // Check if the filename isn't empty
    if (filename == "") {
        close();
        throw QString("KSql Exception : (setFilename) The filename to load can't be empty");
    }

    // Check if the file exist
    QFile file(filename);
    if (file.exists() == false) {
        close();
        throw QString("KSql Exception : (setFilename) The file %1 doesn't exist !").arg(filename);
    }

    m_filename = filename;
}


/**
 * Connect in the database
 */
void Ksql::connect()
{
    m_opened = false;

    // Check if the name of database is OK
    if (m_filename == "") {
        close();
        throw QString("KSql Exception : (connect) The filename is empty !");
    }

    m_db.setDatabaseName(m_filename);

    if (m_db.open() == false) {
        close();
        throw QString("KSql Exception : (connect) unable to connect in the DB !");
    }

    m_opened = true;
}


/**
 * Execute a query
 * @param   QString query   the query to execute
 * @return  QSqlQuery       the result of query
 */
QSqlQuery Ksql::exec(QString query)
{
    if (m_opened == false) {
        Ksql::setFilename(Kconfig::getDatabase());
        Ksql::connect();
    }

    if (query == "") {
        close();
        throw QString("KSql Exception : (exec) The query can't be empty !");
    }

    m_db.transaction();

    QSqlQuery sql_query;
    sql_query.prepare(query);
    sql_query.exec();

    m_db.commit();

    return sql_query;
}


/**
 * Close the connection of the database
 */
void Ksql::close()
{
    if (m_opened == true) {
        m_db.close();
        m_db.removeDatabase(m_db.connectionName());
    }
}
