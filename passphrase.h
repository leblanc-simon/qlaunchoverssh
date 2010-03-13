#ifndef PASSPHRASE_H
#define PASSPHRASE_H

#include <QDialog>

#include "mainwindow.h"

namespace Ui {
    class Passphrase;
}

class Passphrase : public QDialog {
    Q_OBJECT
public:
                    Passphrase(MainWindow *parent = 0);
                    ~Passphrase();

    static QString  getPassphrase();
    static void     initPassphrase();

protected:
            void    changeEvent(QEvent *e);

private:
            Ui::Passphrase  *m_ui;

    static  QString         m_passphrase;

private slots:
            void    validPassphrase();
};

#endif // PASSPHRASE_H
