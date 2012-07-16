#ifndef KDATA_H
#define KDATA_H

#include <QString>
#include <QStringList>

class Kdata
{
private:
    QString         filename;
    bool            error;
    QStringList     errors;

    void            resetError() { this->error = false; this->errors.clear(); }
    void            addError(QString error) { this->error = true; this->errors.append(error); }

public:
                    Kdata(QString filename) { this->error = false; this->filename = filename; }

    bool            load();
    bool            dump();

    // manage errors
    bool            hasError() { return this->error; }
    QString         getErrors() { return this->errors.join("\n"); }
};

#endif // KDATA_H
