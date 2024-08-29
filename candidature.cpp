#include "candidature.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

// Constructors
Candidature::Candidature() {}

Candidature::Candidature(int idCandidat, int idOffre, QDate dateCandidature, QString statut)
    : idCandidat(idCandidat), idOffre(idOffre), dateCandidature(dateCandidature), statut(statut) {}

// Getters
int Candidature::getIdCandidat() const {
    return idCandidat;
}

int Candidature::getIdOffre() const {
    return idOffre;
}

QDate Candidature::getDateCandidature() const {
    return dateCandidature;
}

QString Candidature::getStatut() const {
    return statut;
}

// Setters
void Candidature::setIdCandidat(int idCandidat) {
    this->idCandidat = idCandidat;
}

void Candidature::setIdOffre(int idOffre) {
    this->idOffre = idOffre;
}

void Candidature::setDateCandidature(const QDate &date) {
    this->dateCandidature = date;
}

void Candidature::setStatut(const QString &statut) {
    this->statut = statut;
}

// CRUD Methods
bool Candidature::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO candidature (id_candidat, id_offre, date_candidature, statut) "
                  "VALUES (:idCandidat, :idOffre, TO_DATE(:dateCandidature, 'YYYY-MM-DD'), :statut)");

    // Debugging output
    qDebug() << "Adding Candidature:";
    qDebug() << "ID Candidat:" << idCandidat;
    qDebug() << "ID Offre:" << idOffre;
    qDebug() << "Date Candidature:" << dateCandidature.toString("yyyy-MM-dd");
    qDebug() << "Statut:" << statut;

    query.bindValue(":idCandidat", idCandidat);
    query.bindValue(":idOffre", idOffre);
    query.bindValue(":dateCandidature", dateCandidature.toString("yyyy-MM-dd"));
    query.bindValue(":statut", statut);

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Candidature::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Prepare the SQL query
    query.prepare("SELECT o.titre AS \"Titre Offre\", c.nom AS \"Nom Candidat\", "
                  "ca.date_candidature AS \"Date Candidature\", ca.statut AS \"Statut\" "
                  "FROM candidature ca "
                  "JOIN offre o ON ca.id_offre = o.id "
                  "JOIN candidat c ON ca.id_candidat = c.id "
                  "ORDER BY ca.id_candidat, ca.id_offre");

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return nullptr;
    } else {
        qDebug() << "Query executed successfully.";
    }

    // Set the model's query and check if it was successful
    model->setQuery(query);
    if (model->query().lastError().isValid()) {
        qDebug() << "Model query error:" << model->query().lastError().text();
    } else {
        qDebug() << "Model query set successfully.";
    }

    // Set header data for columns
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Titre Offre"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Candidat"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Candidature"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));

    // Optional: Print the number of rows and columns to confirm data is loaded
    qDebug() << "Model row count:" << model->rowCount();
    qDebug() << "Model column count:" << model->columnCount();

    return model;
}

bool Candidature::supprimer(int idCandidat, int idOffre) {
    QSqlQuery query;
    query.prepare("DELETE FROM candidature WHERE id_candidat = :idCandidat AND id_offre = :idOffre");
    query.bindValue(":idCandidat", idCandidat);
    query.bindValue(":idOffre", idOffre);
    return query.exec();
}

bool Candidature::modifier(int idCandidat, int idOffre) {
    QSqlQuery query;
    query.prepare("UPDATE candidature SET date_candidature = :dateCandidature, statut = :statut "
                  "WHERE id_candidat = :idCandidat AND id_offre = :idOffre");
    query.bindValue(":idCandidat", idCandidat);
    query.bindValue(":idOffre", idOffre);
    query.bindValue(":dateCandidature", dateCandidature);
    query.bindValue(":statut", statut);

    return query.exec();
}
