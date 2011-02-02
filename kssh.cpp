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

#include "kssh.h"
#include "klog.h"
#include "kconfig.h"
#include "libssh/libssh.h"

#include <QByteArray>
#include <QChar>

/* Constructor and destructor */

/**
 * Init the SSH connection and reset the value of the result of command
 */
Kssh::Kssh()
{
    this->init();
    this->m_session = ssh_new();
}


/**
 * Close the SSH connection
 */
Kssh::~Kssh()
{
    if (this->m_session != NULL) {
        ssh_free(this->m_session);
    }
}


/**
 * Initialize the values of this class
 */
void Kssh::init()
{
    this->m_command = QString("");
    this->m_return  = QString("");
}


/* Connection method */

/**
 * Connect in the SSH server
 * @param   QString server        the address of the SSH server
 * @param   QString login         the login for the SSH server
 * @param   QString private_key   the private key to use for connect in the SSH server
 * @param   QString password      the password to use for connect in the SSH server (password or passphrase when you use private key)
 * @param   int     port          the port of the SSH server
 * @return  bool                  true if it's ok, false else
 */
bool Kssh::connect(QString server, QString login, QString private_key, QString password, int port)
{
    if (this->launch_connect(server, login, port) == false) {
        return false;
    }

    if (this->check_known_host() == false) {
        return false;
    }

    if (this->authenticate(private_key, password) == false) {
        return false;
    }

    return true;
}


/**
 * Disconnect of the SSH server
 * @return  bool        true if it's ok, false else
 */
bool Kssh::disconnect()
{
    if (ssh_get_status(this->m_session) != SSH_CLOSED) {
        ssh_disconnect(this->m_session);
    }

    return true;
}


/**
 * Initialize the connection of the server
 * @param   QString server        the address of the SSH server
 * @param   QString login         the login for the SSH server
 * @param   int     port          the port of the SSH server
 * @return  bool                  true if it's ok, false else
 */
bool Kssh::launch_connect(QString server, QString login, int port)
{
    #ifdef DEBUG
    int log = SSH_LOG_FUNCTIONS;
    #else
    int log = SSH_LOG_NOLOG;
    #endif
    
    // define the log level
    if (ssh_options_set(this->m_session, SSH_OPTIONS_LOG_VERBOSITY, &log) < 0) {
        Klog::debug("impossible to set SSH_OPTIONS_LOG_VERBOSITY");
        return false;
    }

    // define the login
    if (ssh_options_set(this->m_session, SSH_OPTIONS_USER, login.toStdString().data()) < 0) {
        Klog::debug("impossible to set SSH_OPTIONS_USER");
        return false;
    }

    // define the host
    if (ssh_options_set(this->m_session, SSH_OPTIONS_HOST, server.toStdString().data()) < 0) {
        Klog::debug("impossible to set SSH_OPTIONS_HOST");
        return false;
    }

    // define the port
    if (ssh_options_set(this->m_session, SSH_OPTIONS_PORT, &port) < 0) {
        Klog::debug("impossible to set SSH_OPTIONS_PORT");
        return false;
    }

    // define the knownhosts filename
    QString known_hosts = Kconfig::getKnownHosts();
    if (ssh_options_set(this->m_session, SSH_OPTIONS_KNOWNHOSTS, known_hosts.toStdString().data()) < 0) {
        Klog::debug("impossible to set SSH_OPTIONS_KNOWNHOSTS");
        return false;
    }

    // launch the connection
    if (ssh_connect(this->m_session) == SSH_ERROR) {
        Klog::error(QString("Connection failed : ") + QString(ssh_get_error(this->m_session)));
        this->disconnect();
        return false;
    }

    return true;
}


/**
 * Check if the host doesn't change
 * @return  bool                  true if it's ok, false else
 */
bool Kssh::check_known_host()
{
    QString error, server_public_key;
    int state, hlen;
    unsigned char *hash = NULL;

    // get the public key of the server
    state = ssh_is_server_known(this->m_session);
    hlen = ssh_get_pubkey_hash(this->m_session, &hash);

    if (hlen < 0) {
        Klog::debug("no public key for this server");
        return false;
    }

    server_public_key = QString(ssh_get_hexa(hash, hlen));

    delete[] hash;

    switch (state) {
        case SSH_SERVER_KNOWN_OK:
            break;

        case SSH_SERVER_KNOWN_CHANGED:
            Klog::warning(QString("Host key for server changed, now it's : ") + server_public_key);
            return false;

        case SSH_SERVER_FOUND_OTHER:
            error = "The host key for this server was not found but an other type of key exists.\n"
                    "into thinking the key does not exist.\n";
            Klog::warning(error);
            return false;

        case SSH_SERVER_FILE_NOT_FOUND:
            Klog::warning("Could not find known host file. This file will be automatically created.");
            if (ssh_write_knownhost(this->m_session) < 0) {
                Klog::debug("impossible to write the knowhosts");
            }
            break;

        case SSH_SERVER_NOT_KNOWN:
            Klog::warning("The server is unknown. This new key will be written on disk for further usage.");
            if (ssh_write_knownhost(this->m_session) < 0) {
                Klog::debug("impossible to write the knowhosts");
            }
            break;

        case SSH_SERVER_ERROR:
            Klog::error(QString("Error while check the knowhosts : ") + QString(ssh_get_error(this->m_session)));
            return false;
      }

    return true;
}


/**
 * Authenticate in the SSH server
 * @param   QString private_key   the private key to use for connect in the SSH server
 * @param   QString password      the password to use for connect in the SSH server (password or passphrase when you use private key)
 * @return  bool                  true if it's ok, false else
 */
bool Kssh::authenticate(QString private_key, QString password)
{
    if (private_key != "") {
        return this->authenticateKey(private_key, password);
    } else {
        return this->authenticatePassword(password);
    }

}


/**
 * Authenticate in the SSH server with key
 * @param   QString private_key   the private key to use for connect in the SSH server
 * @param   QString password      the password to use for connect in the SSH server (passphrase of the private key)
 * @return  bool                  true if it's ok, false else
 * @todo    test with a private key with passphrase
 */
bool Kssh::authenticateKey(QString private_key, QString password)
{
    ssh_string public_key_to_use;
    ssh_private_key private_key_to_use;
    int auth;

    private_key_to_use = privatekey_from_file(this->m_session, private_key.toStdString().c_str(), 0, password.toStdString().c_str());
    if (private_key_to_use == NULL) {
        Klog::error(QString("Fatal error while get the private key : ") + QString(ssh_get_error(this->m_session)));
        return false;
    }

    // get the public key
    public_key_to_use = publickey_to_string(publickey_from_privatekey(private_key_to_use));

    // try to authenticate
    auth = ssh_userauth_pubkey(this->m_session, NULL, public_key_to_use, private_key_to_use);

    if (auth == SSH_AUTH_SUCCESS) {
        // clear the keys of the memory
        privatekey_free(private_key_to_use);
        string_free(public_key_to_use);

        // it's ok
        return true;
    } else if (auth == SSH_AUTH_DENIED || auth == SSH_AUTH_PARTIAL) {
        Klog::warning(QString("Authenticated denied with this key"));
        return false;
    } else if (auth == SSH_AUTH_ERROR) {
        Klog::error(QString("Fatal error in authenticated with key : ") + QString(ssh_get_error(this->m_session)));
        return false;
    }

    return false;
}


/**
 * Authenticate in the SSH server with password
 * @param   QString password      the password to use for connect in the SSH server (password or passphrase when you use private key)
 * @return  bool                  true if it's ok, false else
 */
bool Kssh::authenticatePassword(QString password)
{
    // Try to authenticate without interactive, if fail then try with interactive
    int auth;

    auth = ssh_userauth_password(this->m_session, NULL, password.toStdString().c_str());

    if (auth == SSH_AUTH_SUCCESS) {
        return true;
    } else if (auth == SSH_AUTH_DENIED || auth == SSH_AUTH_PARTIAL) {
        // try with interactive
        Klog::debug("Authentication fail with password, try with interactive");
        return this->authenticatePasswordInteractive(password);
    } else if (auth == SSH_AUTH_ERROR) {
        Klog::error(QString("Fatal error in authenticated with password : ") + QString(ssh_get_error(this->m_session)));
        return false;
    }

    return false;
}


/**
 * Authenticate in the SSH server with password with interactive mode
 * @param   QString password      the password to use for connect in the SSH server (password or passphrase when you use private key)
 * @return  bool                  true if it's ok, false else
 */
bool Kssh::authenticatePasswordInteractive(QString password)
{
    int auth, nb_prompt;
    QString name;

    auth = ssh_userauth_kbdint(this->m_session, NULL, NULL);

    while (auth == SSH_AUTH_INFO) {
        // enter in the interactive mode
        nb_prompt = ssh_userauth_kbdint_getnprompts(this->m_session);
        for (int i = 0; i < nb_prompt; i++) {
            name = QString(ssh_userauth_kbdint_getprompt(this->m_session, i, NULL));
            if (name == "Password: ") {
                if (ssh_userauth_kbdint_setanswer(this->m_session, i, password.toStdString().c_str()) < 0) {
                    return false;
                }
            }
        }

        auth = ssh_userauth_kbdint(this->m_session, NULL, NULL);
    }

    if (auth == SSH_AUTH_SUCCESS) {
        return true;
    } else if (auth == SSH_AUTH_DENIED || auth == SSH_AUTH_PARTIAL) {
        Klog::warning(QString("Authenticated denied with this method : ") + QString(ssh_get_error(this->m_session)));
        return false;
    } else if (auth == SSH_AUTH_ERROR) {
        Klog::error(QString("Fatal error in authenticated with password interactive : ") + QString(ssh_get_error(this->m_session)));
        return false;
    }

    return false;
}

/* Action method */

/**
 * Launch a command over SSH
 * @param   QString command     The command to launch
 * @return  bool                true if it's ok, false else
 */
bool Kssh::launch(QString command)
{
    this->init();

    this->m_command = command;

    int result;
    ssh_channel channel = channel_new(this->m_session);

    if (channel == NULL) {
        Klog::error(QString("Impossible to open a new channel") + QString(ssh_get_error(this->m_session)));

        ssh_disconnect(this->m_session);
        ssh_finalize();

        return false;
    }

    if (channel_open_session(channel) == SSH_ERROR) {
        Klog::error(QString("Impossible to open a session in the channel") + QString(ssh_get_error(this->m_session)));

        channel_close(channel);
        ssh_disconnect(this->m_session);
        ssh_finalize();

        return false;
    }

    result = channel_request_exec(channel, this->m_command.toStdString().c_str());

    if (result == SSH_ERROR) {
        Klog::error(QString("Impossible to launch command : ") + QString(ssh_get_error(this->m_session)));

        channel_close(channel);
        ssh_disconnect(this->m_session);
        ssh_finalize();

        return false;
    } else {
        Klog::debug(QString("Execution de la commande : ") + this->m_command);

        // read the buffer
        char *buf = NULL;
        buf = new char[512];
        int rc = 0;

        do {
            if (channel_is_open(channel)) {
                rc = channel_read(channel, buf, sizeof(buf), 0);
                if (rc > 0) {
                    this->m_return += QString::fromLatin1(buf, rc);
                }
            }
        } while (rc > 0);

        delete[] buf;

        channel_send_eof(channel);
        channel_close(channel);

        ssh_disconnect(this->m_session);
        ssh_finalize();
    }

    return true;
}


/**
 * Return the latest command execute to the SSH server
 * @return  QString     the latest command execute to the SSH server
 */
QString Kssh::getLastCommand()
{
    return this->m_command;
}


/**
 * Return the latest return of the latest command
 * @return  QString     the latest return of the latest command
 */
QString Kssh::getReturn()
{
    return this->m_return;
}
