#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

#include "mainwindow.h"

namespace Ui {
    class About;
}

class About : public QDialog {
    Q_OBJECT
public:
    About(MainWindow *parent = 0);
    ~About();

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
    void init();

private:
    Ui::About  *m_ui;
    MainWindow *m_parent;
};

#endif // ABOUT_H
