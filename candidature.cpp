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
                  "VALUES (:idCandidat, :idOffre, :dateCandidature, :statut)");

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
    model->setQuery("SELECT * FROM candidature ORDER BY id_candidat, id_offre");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Candidat"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Offre"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Candidature"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));

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
