#include <QTextEdit>
#include "kmessagebox.h"

KMessageBox::KMessageBox(QWidget *parent) :
    QMessageBox(parent)
{
    this->setSizeGripEnabled(true);
}


bool KMessageBox::event(QEvent *e)
{
    bool result = QMessageBox::event(e);

    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    this->setMinimumSize(0, 0);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QTextEdit *text = findChild<QTextEdit *>();

    if (text) {
        text->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        text->setMinimumSize(0, 0);
        text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    return result;
}
