#include "kdata.h"
#include "ksql.h"
#include "kcommand.h"
#include "kparameter.h"
#include "klog.h"

#include <QSqlQuery>
#include <QXmlStreamWriter>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QDomNode>
#include <QObject>

/**
 * Dump the database into a file
 *
 * @return  bool    True if it's success, false on failure
 */
bool Kdata::dump()
{
    // reset errors
    this->resetError();

    QString sql = QString("SELECT id FROM command");

    QSqlQuery query = Ksql::exec(sql);
    QString xml = QString();
    QXmlStreamWriter* xml_writer = new QXmlStreamWriter(&xml);

    xml_writer->setAutoFormatting(true);
    xml_writer->writeStartDocument();

    // Create root markup
    xml_writer->writeStartElement("qlaunchoverssh");
    if (xml_writer->hasError()) {
        this->addError(QObject::tr("Unabled to write the root element"));
        delete xml_writer;
        return !this->hasError();
    }

    while (query.next()) {
        Kcommand command = Kcommand();
        if (command.load(query.value(0).toInt())) {
            QString use_key;
            if (command.getUseKey()) {
                use_key = "1";
            } else {
                use_key = "0";
            }

            // write command markup
            xml_writer->writeStartElement("command");

            // set command attributes
            xml_writer->writeAttribute("name", command.getName());
            xml_writer->writeAttribute("login", command.getLogin());
            xml_writer->writeAttribute("server", command.getServer());
            xml_writer->writeAttribute("use_key", use_key);
            xml_writer->writeAttribute("passwd", command.getPasswd());
            xml_writer->writeAttribute("cmdline", command.getCommand());

            QList<Kparameter> parameters = command.getParameters();
            Kparameter parameter;

            foreach (parameter, parameters) {
                // add parameter markup
                QString required;
                QString position;
                if (parameter.getRequired()) {
                    required = "1";
                } else {
                    required = "0";
                }
                xml_writer->writeStartElement("parameter");
                xml_writer->writeAttribute("name", parameter.getName());
                xml_writer->writeAttribute("position", position.setNum(parameter.getPosition()));
                xml_writer->writeAttribute("explain", parameter.getExplain());
                xml_writer->writeAttribute("required", required);
                xml_writer->writeEndElement();
            }

            // close command markup
            xml_writer->writeEndElement();
        }
    }

    xml_writer->writeEndElement();

    xml_writer->writeEndDocument();

    // Write the XML into the dump file
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) == false) {
        this->addError(QObject::tr("Unabled to open the file : %1").arg(filename));
    } else {
        QTextStream out(&file);
        out << xml;
        file.close();
    }

    delete xml_writer;

    return !this->hasError();
}


/**
 * Load a file in the database (no data are delete)
 *
 * @return  bool    True if it's success, false on failure
 */
bool Kdata::load()
{
    // reset errors
    this->resetError();


    QDomDocument doc;
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text) == false) {
        this->addError(QObject::tr("Kdata Exception : (load) unable to open the import file !"));
        return !this->hasError();
    }

    if (doc.setContent(&file) == false) {
        file.close();
        this->addError(QObject::tr("Kdata Exception : (load) unable to load the import file in xml !"));
        return !this->hasError();
    }

    file.close();

    QDomElement root;
    root = doc.documentElement();

    if (root.tagName() != "qlaunchoverssh") {
        this->addError(QObject::tr("Kdata Exception : (load) the import file isn't correct (no qlaunchoverssh Element for root) !"));
        return !this->hasError();
    }

    // Select the parameters
    QDomNodeList items, childs;
    QDomElement item, child;
    int current_item, nb_items, current_child, nb_childs;

    Kcommand* command;
    Kparameter* parameter;

    QString name;
    QString server;
    QString login;
    bool use_key;
    QString passwd;
    QString cmdline;

    QString parameter_name;
    int parameter_position;
    QString parameter_explain;
    bool parameter_required;

    items = root.childNodes();
    nb_items = items.length();

    for (current_item = 0; current_item < nb_items; current_item++) {
        if (items.item(current_item).isElement() == false) {
            continue;
        }

        item = items.item(current_item).toElement();

        if (item.tagName() != "command") {
            continue;
        }

        // get the attributes
        name = item.attribute("name", "");
        server = item.attribute("server", "");
        login = item.attribute("login", "");
        if (item.attribute("use_key", "0") == "1") {
            use_key = true;
        } else {
            use_key = false;
        }
        passwd = item.attribute("passwd", "");
        cmdline = item.attribute("cmdline", "");

        // save command
        command = new Kcommand();
        command->setName(name);
        command->setServer(server);
        command->setLogin(login);
        command->setUseKey(use_key);
        command->setPasswd(passwd);
        command->setCommand(cmdline);
        if (command->save() == false) {
            delete command;
            this->addError(QObject::tr("Unabled to save command : %1").arg(name));
            continue;
        }


        // get parameters
        childs = item.childNodes();
        nb_childs = childs.length();

        for (current_child = 0; current_child < nb_childs; current_child++) {
            if (childs.item(current_child).isElement() == false) {
                continue;
            }

            child = childs.item(current_child).toElement();

            if (child.tagName() != "parameter") {
                continue;
            }

            parameter_name = child.attribute("name", "");
            parameter_position = child.attribute("position", "1").toInt();
            parameter_explain = child.attribute("explain", "");
            if (child.attribute("required", "0") == "1") {
                parameter_required = true;
            } else {
                parameter_required = false;
            }

            parameter = new Kparameter();
            parameter->setCommandId(command->getId());
            parameter->setName(parameter_name);
            parameter->setPosition(parameter_position);
            parameter->setRequired(parameter_required);
            parameter->setExplain(parameter_explain);
            if (parameter->save() == false) {
                delete parameter;
                this->addError(QObject::tr("Unabled to save parameter : %1").arg(parameter_name));
                continue;
            }

            delete parameter;
        }

        delete command;
    }

    return !this->hasError();
}
