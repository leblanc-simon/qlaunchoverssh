DROP TABLE IF EXISTS "command";
CREATE TABLE "command" ("id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "name" VARCHAR(255) NOT NULL , "server" VARCHAR(255) NOT NULL , "login" VARCHAR(255), "use_key" INTEGER NOT NULL , "command" VARCHAR(255) NOT NULL , "passwd" VARCHAR(255));
INSERT INTO "command" VALUES(1,'Installation de Symfony','clovis','root',1,'/root/install_symfony.sh',NULL);
INSERT INTO "command" VALUES(2,'Ajout de dépôt SVN','proliant','root',1,'/root/install_svn.sh',NULL);
DROP TABLE IF EXISTS "parameter";
CREATE TABLE "parameter" ("id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "name" VARCHAR(255) NOT NULL , "position" INTEGER NOT NULL , "id_command" INTEGER NOT NULL , "explain" TEXT, "required" INTEGER);
INSERT INTO "parameter" VALUES(1,'Nom du projet',1,1,'Indiquer le nom du projet',1);
INSERT INTO "parameter" VALUES(2,'Mot de passe',2,1,'Indiquer le mot pour MySQL (facultatif)',0);
INSERT INTO "parameter" VALUES(3,'Nom du projet',1,2,'Indiquer le nom du projet',1);
DROP TABLE IF EXISTS "sqlite_sequence";
CREATE TABLE sqlite_sequence(name,seq);
INSERT INTO "sqlite_sequence" VALUES('command',2);
INSERT INTO "sqlite_sequence" VALUES('parameter',3);
CREATE VIEW "count_parameters" AS SELECT count(id) as nb_parameter, id_command FROM parameter GROUP BY id_command;
CREATE VIEW "get_parameters" AS SELECT parameter.* FROM parameter ORDER BY position ASC;
CREATE VIEW "max_position" AS SELECT id_command, MAX(position) + 1 AS max 
FROM parameter 
GROUP BY id_command;
CREATE INDEX "parameter_id_command" ON "parameter" ("id_command" ASC);
CREATE INDEX "parameter_position" ON "parameter" ("position" ASC);
