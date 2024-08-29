#ifndef CANDIDAT_H
#define CANDIDAT_H

#include <QString>
#include <QSqlQueryModel>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QSqlError>
using namespace QtCharts;

class Candidat {
public:
    Candidat();
    Candidat(int id, QString nom, QString email, QString telephone, QString experience, QString cv);
    Candidat(QString nom, QString email, QString telephone, QString experience, QString cv);

    // Getter methods
    int getID() const;
    QString getNom() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getExperience() const;
    QString getCV() const; // Getter pour cv

    // Setter methods
    void setID(int id);
    void setNom(QString nom);
    void setEmail(QString email);
    void setTelephone(QString telephone);
    void setExperience(QString experience);
    void setCV(QString cv); // Setter pour cv

    // Database operations
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier(int id);
    static QSqlQueryModel* rechercherParTerme(QString terme);
    QSqlQueryModel* Tri(QString cls, QString champ);
    QBarSeries* getStatSpecialites();

private:
    int id;
    QString nom;
    QString email;
    QString telephone;
    QString experience;
    QString cv; // Attribut cv ajouté
};

#endif // CANDIDAT_H
