# -------------------------------------------------
# Project created by QtCreator 2010-02-06T18:07:51
# -------------------------------------------------
QT += network \
    sql \
    xml \
    widgets
TARGET = QLaunchOverSSH
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    kssh.cpp \
    klog.cpp \
    ksql.cpp \
    kconfig.cpp \
    kcommand.cpp \
    kparameter.cpp \
    about.cpp \
    passphrase.cpp \
    kmessagebox.cpp \
    kdata.cpp
HEADERS += mainwindow.h \
    kssh.h \
    klog.h \
    ksql.h \
    kconfig.h \
    kcommand.h \
    kparameter.h \
    about.h \
    passphrase.h \
    version.h \
    kmessagebox.h \
    kdata.h
FORMS += mainwindow.ui \
    about.ui \
    passphrase.ui
LIBS += -lssh
#DEFINES += DEBUG
RESOURCES += ressources.qrc

# Fichier ressource pour windows (num version, auteur, ...)
RC_FILE     = ressources.rc

OTHER_FILES += README \
    LICENSE \
    AUTHORS \
    logo.png \
    cadenas-ouvert-icone-4601-16.png \
    applet-gnome-sshmenu-icone-6595-16.png \
    config.xml \
    qlaunchoverssh.sqlite \
    ressources.rc
