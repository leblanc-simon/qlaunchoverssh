QLaunchOverSSH
==============

Requirement
-----------

This program require the [libssh library](http://www.libssh.org/) and [Qt](http://qt.digia.com/Product/) (tested on Qt version 4.x). You must install it before use QLaunchOverSSH


Installation
------------

```bash
qmake QLaunchOverSSH.pro
make
sudo make install
```

Configure the program
---------------------

* Edit the file config.xml and set the value you want for each item :
  * database : the path of the database (default value : ./qlaunchoverssh.sqlite)
  * know_hosts : the path of the know host file (default value : ./ssh/known_hosts) It will be create if it doesn't exist
  * log : the path of the log file (default value : ./qlaunchoverssh.log) It will be create if it doesn't exist
  * private_key : the path of the private key in OpenSSH format (default value : ./ssh/id_dsa)
  * passphrase : set 1 if your private key require a passphrase, 0 else (default value : 0)
* Add your command in the database

Database structure
------------------

```sql
CREATE TABLE "command" ("id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "name" VARCHAR(255) NOT NULL , "server" VARCHAR(255) NOT NULL , "login" VARCHAR(255), "use_key" INTEGER NOT NULL , "command" VARCHAR(255) NOT NULL , "passwd" VARCHAR(255));
CREATE TABLE "parameter" ("id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "name" VARCHAR(255) NOT NULL , "position" INTEGER NOT NULL , "id_command" INTEGER NOT NULL , "explain" TEXT, "required" INTEGER);
CREATE VIEW "count_parameters" AS SELECT count(id) as nb_parameter, id_command FROM parameter GROUP BY id_command;
CREATE VIEW "get_parameters" AS SELECT parameter.* FROM parameter ORDER BY position ASC;
CREATE VIEW "max_position" AS SELECT id_command, MAX(position) + 1 AS max FROM parameter GROUP BY id_command;
CREATE INDEX "parameter_id_command" ON "parameter" ("id_command" ASC);
CREATE INDEX "parameter_position" ON "parameter" ("position" ASC);
```

License
-------

* QLaunchOverSSH is release under [BSD license](http://opensource.org/licenses/BSD-3-Clause)
* Logo in the about box : The logo is an original creation of [harrypopof](http://hpfteam.free.fr/graphisme-libre/) published under art libre license modified by Simon Leblanc
  * Original logo (harrypopof) :
    * http://hpfteam.free.fr/ressources/framartlibre/framashirt-logo_mouton.tar.gz
    * http://www.leblanc-simon.fr/framasoft/portable/Utils/FramaGenXML/framashirt-logo_mouton.tar.gz
  * Modified logo (Simon Leblanc) :
    * http://leblanc-simon.fr/framasoft/portable/Utils/FramaGenXML/logo2.svg
* Main icon of the program : The icon is an original creation of [Oliver Scholtz (and others)](http://linux.softpedia.com/developer/Oliver-Scholtz-93.html)
   * http://icones.pro/applet-gnome-sshmenu-image-png.html
* Icon of the passphrase window : The icon is an original creation of [FatCow](http://www.fatcow.com/free-icons/)
   * http://icones.pro/cadenas-ouvert-image-png.html

Authors
-------

* Simon Leblanc <contact@leblanc-simon.eu> : main developper
* Patrick Rutka : contributor (database debug)
