#include "klog.h"
#include "kconfig.h"

#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDateTime>

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


void Klog::debug(QString msg)
{
#ifdef DEBUG
    Klog::write(msg, "debug");
#endif
}


void Klog::warning(QString msg)
{
    Klog::write(msg, "warning");
}


void Klog::error(QString msg)
{
    Klog::write(msg, "ERROR");
}
