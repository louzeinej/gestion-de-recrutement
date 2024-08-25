#ifndef OFFRE_H
#define OFFRE_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Offre {


public:
    // Constructors
    Offre();
    Offre(int id, QString titre, QString description, QDate date_publication,
                double salaire);
    Offre(QString titre, QString description, QDate date_publication,
                double salaire);

    // Getters
    int getId() ;
    QString getTitre() ;
    QString getDescription() ;
    QDate getDatePublication() ;

    double getSalaire() ;


    // Setters
    void setId(int id);
    void setTitre(QString titre);
    void setDescription(QString description);
    void setDatePublication(QDate date_publication);

    void setSalaire(double salaire);


    // Database management methods
    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercherParTitre(QString terme);
    QSqlQueryModel* trier(QString cls, QString champ);

private:
    int id;
    QString titre;
    QString description;
    QDate date_publication;

    double salaire;

};

#endif // OFFRE_H
