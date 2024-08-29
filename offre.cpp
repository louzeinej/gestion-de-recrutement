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
    query.bindValue(":titre", titre);
    query.bindValue(":description", description);
    query.bindValue(":date_publication", date_publication);

    query.bindValue(":salaire", salaire);

    return query.exec();
}
QBarSeries* Offre::getStatOffresParTitre() {
    QBarSeries* barSeries = new QBarSeries();

    QSqlQuery query;
    query.prepare("SELECT titre, COUNT(*) AS nombre_offres "
                  "FROM offre "
                  "GROUP BY titre "
                  "ORDER BY nombre_offres DESC;");

    if (query.exec()) {
        while (query.next()) {
            QString titre = query.value(0).toString();
            int nombreOffres = query.value(1).toInt();

            QBarSet *barSet = new QBarSet(titre);
            *barSet << nombreOffres;
            barSeries->append(barSet);
        }

        qDebug() << "Query executed successfully.";
    } else {
        qDebug() << "Query execution failed:" << query.lastError();
    }

    return barSeries;
}
QSqlQueryModel* Offre::Tri(QString cls, QString champ) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryString = "SELECT * FROM offre ORDER BY " + champ + " " + cls;
    QSqlQuery query;
    query.prepare(queryString);
    query.exec();
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("TITRE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DESCRIPTION"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_PUBLICATION"));

    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire"));


    return model;
}
QSqlQueryModel* Offre::rechercherParTerme(QString terme) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Préparer la requête pour rechercher par nom, email ou numéro de téléphone
    query.prepare("SELECT * FROM offre WHERE titre LIKE :terme OR salaire LIKE :terme ");
    query.bindValue(":terme", "%" + terme + "%");

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Failed to execute search query:" << query.lastError().text();
    }

    return model;
}
QSqlQueryModel* Offre::rechercherParDatePublication(QDate date_publication) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Check if the provided date is valid
    if (!date_publication.isValid()) {
        qDebug() << "Invalid date provided.";
        return model;
    }

    // Prepare the query to search by journee with a correct date format
    query.prepare("SELECT * FROM offre WHERE date_publication = :date_publication");

    // Bind the date as a QDate
    query.bindValue(":date_publication", date_publication);

    // Execute the query and set the result model
    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Failed to execute query:" << query.lastError().text();
    }

    return model;
}

