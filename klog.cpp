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

#include "klog.h"
#include "kconfig.h"

#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDateTime>

/**
 * Write the message into the log file
 * @param   QString     msg     the message to write
 * @param   QString     type    the type of log
 */
void Klog::write(QString msg, QString type)
{
    QDateTime date = QDateTime(QDateTime::currentDateTime());
    QFile file(Kconfig::getLog());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        return;
    }

    QTextStream out(&file);

    out << "-------------------------------------------------------------------------------\n";
    out << type << " - " << date.toString(QString("yyyy-MM-dd hh:mm:ss")) << " - " << msg << "\n";

    file.close();
}


#ifdef DEBUG
/**
 * Write a debug message into the log file (only if the program is make with -DDEBUG)
 * @param   QString     msg     the messageto write
 */
void Klog::debug(QString msg)
{
    Klog::write(msg, "debug");
}
#endif


/**
 * Write a warning message into the log file
 * @param   QString     msg     the message to write
 */
void Klog::warning(QString msg)
{
    Klog::write(msg, "warning");
}


/**
 * Write an error message into the log file
 * @param   QString     msg     the message to write
 */
void Klog::error(QString msg)
{
    Klog::write(msg, "ERROR");
}


/**
 * Write an info message into the log file
 * @param   QString     msg     the message to write
 */
void Klog::info(QString msg)
{
    Klog::write(msg, "INFO");
}
