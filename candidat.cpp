#include "candidat.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>


// Constructeurs
Candidat::Candidat() {}

Candidat::Candidat(int id, QString nom, QString email, QString telephone, QString experience)
    : id(id), nom(nom), email(email), telephone(telephone), experience(experience) {}

Candidat::Candidat(QString nom, QString email, QString telephone, QString experience)
    : nom(nom), email(email), telephone(telephone), experience(experience) {}

// Getters
int Candidat::getID() const {
    return id;
}

QString Candidat::getNom() const {
    return nom;
}

QString Candidat::getEmail() const {
    return email;
}

QString Candidat::getTelephone() const {
    return telephone;
}



QString Candidat::getExperience() const {
    return experience;
}

// Setters
void Candidat::setID(int id) {
    this->id = id;
}

void Candidat::setNom(QString nom) {
    this->nom = nom;
}

void Candidat::setEmail(QString email) {
    this->email = email;
}

void Candidat::setTelephone(QString telephone) {
    this->telephone = telephone;
}



void Candidat::setExperience(QString experience) {
    this->experience = experience;
}

// Méthodes de gestion de la base de données
bool Candidat::ajouter(){
    QSqlQuery query;

    query.prepare("INSERT INTO candidat (nom, email, telephone,  experience) "
                  "VALUES (:nom, :email, :telephone, :experience)");

    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);

    query.bindValue(":experience", experience);

    return query.exec();
}

QSqlQueryModel* Candidat::afficher() {
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM candidat ORDER BY id");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Expérience"));

    return model;
}

bool Candidat::supprimer(int id){
    QSqlQuery query;
    query.prepare("DELETE FROM candidat WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Candidat::modifier(int id){
    QSqlQuery query;

    query.prepare("UPDATE candidat SET nom = :nom, email = :email, telephone = :telephone, "
                  "experience = :experience WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);

    query.bindValue(":experience", experience);

    return query.exec();
}
QSqlQueryModel* Candidat::rechercherParTerme(QString terme) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Préparer la requête pour rechercher par nom, email ou numéro de téléphone
    query.prepare("SELECT * FROM candidat WHERE nom LIKE :terme OR email LIKE :terme OR telephone LIKE :terme");
    query.bindValue(":terme", "%" + terme + "%");

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Failed to execute search query:" << query.lastError().text();
    }

    return model;
}
QSqlQueryModel* Candidat::Tri(QString cls, QString champ) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryString = "SELECT * FROM candidat ORDER BY " + champ + " " + cls;
    QSqlQuery query;
    query.prepare(queryString);
    query.exec();
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));

    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Expérience"));


    return model;
}


