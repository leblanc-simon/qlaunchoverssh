#ifndef KSSH_H
#define KSSH_H

#include <QString>

#include "libssh/libssh.h"

class Kssh
{
private:
    /* The variable with the SSH session */
    ssh_session m_session;

protected:
    QString m_command;
    QString m_return;

    void    init();

    /* Connection method */
    bool    launch_connect(QString server, QString login, int port = 22);
    bool    check_known_host();
    bool    authenticate(QString private_key = "", QString password = "");
    bool    authenticateKey(QString private_key, QString password = "");
    bool    authenticatePassword(QString password);
    bool    authenticatePasswordInteractive(QString password);

public:
    /* Constructor and destructor */
            Kssh();
            ~Kssh();

    /* Connection method */
    bool    connect(QString server, QString login, QString private_key = "", QString password = "", int port = 22);
    bool    disconnect();

    /* Action method */
    bool    launch(QString command);
    QString getLastCommand();
    QString getReturn();

};

#endif // KSSH_H
