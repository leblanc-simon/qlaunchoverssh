#ifndef KLOG_H
#define KLOG_H

#include <QString>

class Klog
{
protected:
    static void write(QString msg, QString type);
public:
    static void debug(QString msg);
    static void warning(QString msg);
    static void error(QString msg);
};

#endif // KLOG_H
