#ifndef KCONFIG_H
#define KCONFIG_H

#define CONFIG_FILE                 "./config.xml"
#define KCONFIG_DEFAULT_DATABASE    "./qlaunchoverssh.sqlite"
#define KCONFIG_DEFAULT_LOG         "./qlaunchoverssh.log"
#define KCONFIG_DEFAULT_KNOWNHOSTS  "./ssh/known_hosts"
#define KCONFIG_DEFAULT_PRIVATEKEY  "./ssh/id_dsa"
#define KCONFIG_DEFAULT_PASSPHRASE  false

#include <QString>

class Kconfig
{
protected:
    static QString  m_database;
    static QString  m_log;
    static QString  m_known_hosts;
    static QString  m_private_key;
    static bool     m_passphrase;

public:
    static bool     load(QString filename);

    static QString  getDatabase()   { return Kconfig::m_database; }
    static QString  getLog()        { return Kconfig::m_log; }
    static QString  getKnownHosts() { return Kconfig::m_known_hosts; }
    static QString  getPrivateKey() { return Kconfig::m_private_key; }
    static bool     hasPassphrase() { return Kconfig::m_passphrase; }
};

#endif // KCONFIG_H
