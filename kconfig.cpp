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
