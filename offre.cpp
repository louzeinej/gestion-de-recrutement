#include "offre.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>


// Constructeurs
Offre::Offre() {}

Offre::Offre(int id, QString titre, QString description, QDate date_publication,  double salaire)
    : id(id), titre(titre), description(description), date_publication(date_publication), salaire(salaire) {}

Offre::Offre( QString titre, QString description, QDate date_publication,  double salaire)
    :  titre(titre), description(description), date_publication(date_publication), salaire(salaire) {}

// Getters
int Offre::getId()  {
    return id;
}

QString Offre::getTitre()  {
    return titre;
}

QString Offre::getDescription()  {
    return description;
}

QDate Offre::getDatePublication()  {
    return date_publication;
}



double Offre::getSalaire()  {
    return salaire;
}

// Setters
void Offre::setId(int id) {
    this->id = id;
}

void Offre::setTitre(QString titre) {
    this->titre = titre;
}

void Offre::setDescription(QString description) {
    this->description = description;
}

void Offre::setDatePublication(QDate date_publication) {
    this->date_publication = date_publication;
}



void Offre::setSalaire(double salaire) {
    this->salaire = salaire;
}

// Méthodes de gestion de la base de données
bool Offre::ajouter(){
    QSqlQuery query;

    query.prepare("INSERT INTO offre (titre, description, date_publication,  salaire) "
                  "VALUES (:nom, :email, :telephone, :experience)");

    query.bindValue(":nom", titre);
    query.bindValue(":email", description);
    query.bindValue(":telephone", date_publication);

    query.bindValue(":experience", salaire);

    return query.exec();
}

QSqlQueryModel* Offre::afficher() {
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Offre ORDER BY id");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("titre"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("date_publication"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("salaire"));

    return model;
}

bool Offre::supprimer(int id){
    QSqlQuery query;
    query.prepare("DELETE FROM Offre WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Offre::modifier(int id){
    QSqlQuery query;

    query.prepare("UPDATE Offre SET titre = :titre, description = :description, date_publication = :date_publication, "
                  "salaire = :salaire WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", titre);
    query.bindValue(":email", description);
    query.bindValue(":telephone", date_publication);

    query.bindValue(":experience", salaire);

    return query.exec();
}


