#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "about.h"
#include "passphrase.h"

#include <QList>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>

#include "kssh.h"
#include "kconfig.h"
#include "kcommand.h"
#include "klog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    this->m_ui->setupUi(this);
    this->init();
}

MainWindow::~MainWindow()
{
    delete this->m_ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        this->m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::setEnable()
{
    this->m_ui->centralWidget->setEnabled(true);
}


/**
 * initialize the application window
 */
void MainWindow::init()
{
    this->initShow();

    this->initCommands();

    QObject::connect(this->m_ui->combo_command, SIGNAL(currentIndexChanged(int)), this, SLOT(selectCommand(int)));

    QObject::connect(this->m_ui->button_launch, SIGNAL(accepted()), this, SLOT(launchCommand()));

    QObject::connect(this->m_ui->action_about, SIGNAL(triggered()), this, SLOT(showAbout()));

    QObject::connect(this->m_ui->action_close, SIGNAL(triggered()), this, SLOT(close()));
}


/**
 * initialize the combobox with the list of commands
 */
void MainWindow::initCommands()
{
    this->m_list_command = Kcommand::doSelect();

    int max = this->m_list_command.size();

    // add a blank line
    this->m_ui->combo_command->addItem(QString(""), -1);

    // add the commands
    for (int i = 0; i < max; i++) {
        Kcommand command = this->m_list_command.at(i);
        this->m_ui->combo_command->addItem(command.getName(), i);
    }
}


/**
 * initialize the window design
 */
void MainWindow::initShow()
{
    this->m_ui->button_launch->hide();
    this->resize(this->width(), MAINWINDOW_HEIGHT);
    this->m_ui->centralWidget->repaint();
}


/**
 * action to do when the user select a command [slot]
 * @param   int     id      the current index of the combobox
 */
void MainWindow::selectCommand(int id)
{
    int combo_id = this->m_ui->combo_command->itemData(id).toInt();

    // delete the widget and the label
    for (int i = 0; i < this->m_widget.size(); i++) {
        delete this->m_widget.at(i);
        delete this->m_label.at(i);
    }

    this->m_widget.clear();
    this->m_label.clear();

    // return if the combo_id isn't valid
    if (combo_id < 0 || combo_id >= this->m_list_command.size()) {
        this->initShow();
        return;
    }

    // get the current command
    Kcommand command = this->m_list_command.at(combo_id);

    // get the parameters
    int nb_parameter = command.getNbParameter();
    QList<Kparameter> parameters = command.getParameters();

    int height = MAINWINDOW_BEGIN_HEIGHT;

    // add the widget and the label
    for (int i = 0; i < nb_parameter; i++) {
        // add widget
        this->m_widget.append(new QLineEdit(this->m_ui->centralWidget));

        // customize the widget
        Kparameter parameter = parameters.at(i);
        this->m_widget.at(i)->setGeometry(MAINWINDOW_WIDGET_X, height, MAINWINDOW_WIDGET_WIDTH, MAINWINDOW_WIDGET_HEIGHT);
        this->m_widget.at(i)->setToolTip(parameter.getExplain());
        this->m_widget.at(i)->setStatusTip(parameter.getExplain());
        this->m_widget.at(i)->show();

        // add the label
        this->m_label.append(new QLabel(parameter.getName(), this->m_ui->centralWidget));

        // customize the label
        this->m_label.at(i)->setGeometry(MAINWINDOW_LABEL_X, height + MAINWINDOW_LABEL_RECTIF, MAINWINDOW_LABEL_WIDTH, MAINWINDOW_LABEL_HEIGHT);
        this->m_label.at(i)->show();

        height += MAINWINDOW_BETWEEN_HEIGHT;
    }

    // re-paint the button
    this->m_ui->button_launch->setGeometry(this->m_ui->button_launch->x(), height, this->m_ui->button_launch->width(), this->m_ui->button_launch->height());
    this->m_ui->button_launch->show();

    // re-paint the window
    this->resize(this->width(), height + MAINWINDOW_BEGIN_HEIGHT + MAINWINDOW_MARGIN);
    this->m_ui->centralWidget->repaint();
}

/**
 * launch the command over SSH [slot]
 */
void MainWindow::launchCommand()
{
    // get the command
    int current_index = this->m_ui->combo_command->currentIndex();
    int combo_id = this->m_ui->combo_command->itemData(current_index).toInt();

    if (combo_id < 0 || combo_id >= this->m_list_command.size()) {
        // no command
        return;
    }

    Kcommand command = this->m_list_command.at(combo_id);

    // get the parameters
    QString parameters = QString("");
    QLineEdit* edit;
    bool error = false;

    int nb_parameter = command.getNbParameter();
    for (int i = 0; i < nb_parameter; i++) {
        edit = this->m_widget.at(i);

        // check the parameter
        if (command.getParameter(i).getRequired() && edit->text().isEmpty()) {
            if (!error) {
                edit->setFocus();
            }
            error = true;
            edit->setStyleSheet(QString("background-color: rgba(255, 0, 0, 40);"));
        } else {
            edit->setStyleSheet(QString(""));
        }

        // add the parameter
        parameters += QString(" \"%1\"").arg(edit->text().replace(QString("\""), QString("\\\"")));
    }

    if (error) {
        this->m_ui->label_error->setText(tr("Certains paramètres requis sont manquants"));
        return;
    } else {
        this->m_ui->label_error->setText(QString(""));
    }

    // launch command
    Kssh ssh = Kssh();
    QString private_key, password;
    if (command.getUseKey()) {
        private_key = Kconfig::getPrivateKey();
        if (Kconfig::hasPassphrase()) {
            // ask the password
            Passphrase* pass_window = new Passphrase(this);
            pass_window->exec();

            // get the password
            password = Passphrase::getPassphrase();
            Passphrase::initPassphrase();
        } else {
            password = QString("");
        }
    } else {
        private_key = QString("");
        password = command.getPasswd();
    }
    if (ssh.connect(command.getServer(), command.getLogin(), private_key, password)) {
        if (ssh.launch(command.getCommand() + parameters)) {
            // command ok
            Klog::info(ssh.getLastCommand());
            Klog::info(ssh.getReturn());
            QMessageBox::information(this, tr("Commande exécutée"), tr("Votre commande a été exécuté avec succès."));

            // set the combobox to null
            this->m_ui->combo_command->setCurrentIndex(0);
        } else {
            // command nok
            QMessageBox::critical(this, tr("Erreur commande"), tr("Erreur lors de l'exécution de la commande. Veuillez regarder les logs pour plus de détails."));
        }
        ssh.disconnect();
    } else {
        // error to connect
        QMessageBox::critical(this, tr("Erreur connexion"), tr("Erreur lors de la connexion. Veuillez regarder les logs pour plus de détails."));
    }
}


/**
 * show the about dialog [slot]
 */
void MainWindow::showAbout()
{
    this->m_ui->centralWidget->setEnabled(false);

    About* w = new About(this);
    w->show();
}
