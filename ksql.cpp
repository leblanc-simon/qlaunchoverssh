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

#include "ksql.h"
#include "kconfig.h"

#include <QFile>
#include <QString>

QString      Ksql::m_filename = "";
QSqlDatabase Ksql::m_db       = QSqlDatabase();
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
