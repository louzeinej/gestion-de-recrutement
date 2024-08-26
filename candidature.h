#ifndef CANDIDATURE_H
#define CANDIDATURE_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Candidature {
public:
    // Constructeurs
    Candidature();
    Candidature(int idCandidature, int idCandidat, int idOffre, QDate dateCandidature, QString statut);
    Candidature(int idCandidat, int idOffre, QDate dateCandidature, QString statut);

    // Getters
    int getIdCandidature() const;
    int getIdCandidat() const;
    int getIdOffre() const;
    QDate getDateCandidature() const;
    QString getStatut() const;

    // Setters
    void setIdCandidature(int idCandidature);
    void setIdCandidat(int idCandidat);
    void setIdOffre(int idOffre);
    void setDateCandidature(const QDate &date);
    void setStatut(const QString &statut);

    // Méthodes CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int idCandidat, int idOffre);
    bool modifier(int idCandidat, int idOffre);

    // Méthodes supplémentaires
    QSqlQueryModel* rechercherParTerme(QString terme);
    QSqlQueryModel* trier(QString champ, QString ordre);

private:
    int idCandidature;
    int idCandidat;
    int idOffre;
    QDate dateCandidature;
    QString statut;
};

#endif // CANDIDATURE_H
