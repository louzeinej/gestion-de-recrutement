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
    Candidat(int id, QString nom, QString email, QString telephone, QString experience);
    Candidat(QString nom, QString email, QString telephone, QString experience);

    // Getter methods
    int getID() const;
    QString getNom() const;
    QString getEmail() const;
    QString getTelephone() const;

    QString getExperience() const;

    // Setter methods
    void setID(int id);
    void setNom(QString nom);
    void setEmail(QString email);
    void setTelephone(QString telephone);

    void setExperience(QString experience);

    // Database operations
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier(int id);
    static QSqlQueryModel* rechercherParNom( QString nom);
   QSqlQueryModel* Tri(QString cls, QString champ);
    QBarSeries* getStatSpecialites();


private:
    int id;
    QString nom;
    QString email;
    QString telephone;

    QString experience;
};
#endif // CANDIDAT_H
