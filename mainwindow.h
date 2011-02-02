/*
Copyright (c) 2010, Simon Leblanc
All rights reserved.

Redistribution and use in source and binary forms, with or without modification
, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice
    , this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
    * Neither the name of the Simon Leblanc nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
