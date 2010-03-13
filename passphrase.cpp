#include "passphrase.h"
#include "ui_passphrase.h"

QString Passphrase::m_passphrase = QString("");

Passphrase::Passphrase(MainWindow *parent) :
    QDialog(parent),
    m_ui(new Ui::Passphrase)
{
    this->m_ui->setupUi(this);

    QObject::connect(this->m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(validPassphrase()));
}

Passphrase::~Passphrase()
{
    delete this->m_ui;
}

void Passphrase::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        this->m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString Passphrase::getPassphrase()
{
    return Passphrase::m_passphrase;
}


void Passphrase::initPassphrase()
{
    Passphrase::m_passphrase = QString("");
}

void Passphrase::validPassphrase()
{
    Passphrase::m_passphrase = this->m_ui->lineEdit->text();
}
