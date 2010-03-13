#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAINWINDOW_HEIGHT           105
#define MAINWINDOW_BEGIN_HEIGHT     55
#define MAINWINDOW_BETWEEN_HEIGHT   30
#define MAINWINDOW_MARGIN           35

#define MAINWINDOW_WIDGET_X         130
#define MAINWINDOW_WIDGET_HEIGHT    25
#define MAINWINDOW_WIDGET_WIDTH     250

#define MAINWINDOW_LABEL_X          10
#define MAINWINDOW_LABEL_HEIGHT     15
#define MAINWINDOW_LABEL_WIDTH      110
#define MAINWINDOW_LABEL_RECTIF     3

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QList>

#include "kcommand.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
            MainWindow(QWidget *parent = 0);
            ~MainWindow();
    void    setEnable();

protected:
    void changeEvent(QEvent *e);
    void init();
    void initCommands();
    void initShow();

private:
    Ui::MainWindow*     m_ui;
    QList<Kcommand>     m_list_command;
    QList<QLineEdit*>   m_widget;
    QList<QLabel*>      m_label;

private slots:
    void selectCommand(int id);
    void launchCommand();
    void showAbout();

};

#endif // MAINWINDOW_H
