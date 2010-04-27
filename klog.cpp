#include "klog.h"
#include "kconfig.h"

#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDateTime>

/**
 * Write the message into the log file
 * @param   QString     msg     the message to write
 * @param   QString     type    the type of log
 */
void Klog::write(QString msg, QString type)
{
    QDateTime date = QDateTime(QDateTime::currentDateTime());
    QFile file(Kconfig::getLog());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        return;
    }

    QTextStream out(&file);

    out << "-------------------------------------------------------------------------------\n";
    out << type << " - " << date.toString(QString("yyyy-MM-dd hh:mm:ss")) << " - " << msg << "\n";

    file.close();
}


/**
 * Write a debug message into the log file (only if the program is make with -DDEBUG)
 * @param   QString     msg     the messageto write
 */
void Klog::debug(QString msg)
{
#ifdef DEBUG
    Klog::write(msg, "debug");
#endif
}


/**
 * Write a warning message into the log file
 * @param   QString     msg     the message to write
 */
void Klog::warning(QString msg)
{
    Klog::write(msg, "warning");
}


/**
 * Write an error message into the log file
 * @param   QString     msg     the message to write
 */
void Klog::error(QString msg)
{
    Klog::write(msg, "ERROR");
}


/**
 * Write an info message into the log file
 * @param   QString     msg     the message to write
 */
void Klog::info(QString msg)
{
    Klog::write(msg, "INFO");
}
