#ifndef KMESSAGEBOX_H
#define KMESSAGEBOX_H

#include <QMessageBox>
#include <QEvent>

class KMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit KMessageBox(QWidget *parent = 0);

protected:
    bool event(QEvent *e);
};

#endif // KMESSAGEBOX_H
