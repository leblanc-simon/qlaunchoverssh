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

#include "kconfig.h"
#include "klog.h"

#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QDomNode>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

QString Kconfig::m_database     = QString(KCONFIG_DEFAULT_DATABASE);
QString Kconfig::m_log          = QString(KCONFIG_DEFAULT_LOG);
QString Kconfig::m_known_hosts  = QString(KCONFIG_DEFAULT_KNOWNHOSTS);
QString Kconfig::m_private_key  = QString(KCONFIG_DEFAULT_PRIVATEKEY);
bool    Kconfig::m_passphrase   = KCONFIG_DEFAULT_PASSPHRASE;

/**
 * Load the config file in the program
 * @param   QString     filename    the filename of the config
 */
bool Kconfig::load(QString filename)
{
    QDomDocument doc;
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text) == false) {
        Klog::error(QString("Kconfig Exception : (load) unable to open the config file !"));
        return false;
    }

    if (doc.setContent(&file) == false) {
        file.close();
        Klog::error(QString("Kconfig Exception : (load) unable to load the config file in xml !"));
        return false;
    }

    file.close();

    QDomElement root;
    root = doc.documentElement();

    if (root.tagName() != "config") {
        Klog::error(QString("Kconfig Exception : (load) the config file isn't correct (no config Element for root) !"));
        return false;
    }

    // Select the parameters
    QDomNodeList items;
    QDomElement item;
    QString id;
    int nb_node;

    items = doc.elementsByTagName("item");
    nb_node = items.length();

    for(int i = 0; i < nb_node; i++) {
        item = items.item(i).toElement();
        id = item.attribute("id", "");
        if (id == "database") {
            Kconfig::m_database = item.text();
        } else if (id == "known_hosts") {
            Kconfig::m_known_hosts = item.text();
        } else if (id == "log") {
            Kconfig::m_log = item.text();
        } else if (id == "private_key") {
            Kconfig::m_private_key = item.text();
        } else if (id == "passphrase") {
            Kconfig::m_passphrase = (bool)item.text().toInt();
        }
    }

    return true;
}
