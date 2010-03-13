#ifndef KSQL_H
#define KSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class Ksql
{
private:
    static QString          m_filename;
    static QSqlDatabase     m_db;
    static bool             m_opened;

public:
                            Ksql();
                            ~Ksql();

    static void             setFilename(QString filename);
    static void             connect();
    static QSqlQuery        exec(QString query);
    static void             close();
    static QString          clean(QString value) { return value.replace(QString("'"), QString("''")); }
};

#endif // KSQL_H
