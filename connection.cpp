#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("db_projet");//inserer le nom de la source de données ODBC
db.setUserName("cutie");//inserer nom de l'utilisateur
db.setPassword("cutie1234");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;
return  test;
}
