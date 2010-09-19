#include <QtGui/QApplication>
#include <QTextCodec>
#include <QString>
#include <QDebug>

#include "mainwindow.h"

#include "kconfig.h"
#include "kcommand.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    Kconfig::load(CONFIG_FILE);

    /*Kcommand command = Kcommand();
    command.load(2);*/

    try {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    } catch (QString& e) {
        qDebug("%s", e.toStdString().c_str());
        return 1;
    }
}
