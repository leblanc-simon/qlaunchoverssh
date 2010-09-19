#include "about.h"
#include "ui_about.h"

#include <QLabel>
#include <QPixmap>

About::About(MainWindow *parent) :
    QDialog(parent),
    m_ui(new Ui::About)
{
    this->m_ui->setupUi(this);
    this->m_parent = parent;

    this->init();
}

About::~About()
{
    delete m_ui;
}

void About::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void About::init()
{
    QObject::connect(this->m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    QObject::connect(this->m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}


void About::closeEvent(QCloseEvent *event)
{
    this->m_parent->setEnable();
    QDialog::closeEvent(event);
}
