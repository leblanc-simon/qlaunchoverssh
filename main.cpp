#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"

#include "kconfig.h"
#include "kcommand.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    Kconfig::load(CONFIG_FILE);

    Kcommand command = Kcommand();
    command.load(2);


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
