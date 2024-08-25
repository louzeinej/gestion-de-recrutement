#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "candidat.h"
#include "offre.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>



#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QTextStream>




QT_CHARTS_USE_NAMESPACE



Candidat candidat;
Offre offre;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView_candidat->setModel(candidat.afficher());
     ui->tableView_offre->setModel(offre.afficher());
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_ajouter_ca_clicked()
{
    // Récupérer les valeurs des champs de l'interface utilisateur
        QString nom = ui->lineEdit_nom->text();
        QString email = ui->lineEdit_mail->text();
        QString telephone = ui->lineEdit_tel->text();

        QString experience = ui->lineEdit_exp->text();

        // Vérification des champs

        // Vérifier si tous les champs sont remplis
        if (nom.isEmpty() || email.isEmpty() || telephone.isEmpty() ||  experience.isEmpty()) {
            QMessageBox::warning(this, "Champ(s) manquant(s)", "Veuillez remplir tous les champs.");
            return;
        }

        // Vérifier le format de l'email avec une expression régulière
           QRegularExpression emailRegex("^[\\w\\.-]+@[\\w\\.-]+\\.[a-zA-Z]{2,}$");
           if (!emailRegex.match(email).hasMatch()) {
               QMessageBox::warning(this, "Email invalide", "Veuillez entrer une adresse email valide.");
               return;
           }


        // Vérifier que le numéro de téléphone contient uniquement des chiffres, espaces ou '+'
        QRegularExpression phoneRegex("^\\+?[0-9 ]{7,15}$");
        if (!phoneRegex.match(telephone).hasMatch()) {
            QMessageBox::warning(this, "Téléphone invalide", "Veuillez entrer un numéro de téléphone valide (chiffres uniquement, avec ou sans '+').");
            return;
        }

        // Vérifier que le nom contient uniquement des lettres et des espaces
        QRegularExpression nameRegex("^[a-zA-Z\\s]+$");
        if (!nameRegex.match(nom).hasMatch()) {
            QMessageBox::warning(this, "Nom invalide", "Le nom ne doit contenir que des lettres et des espaces.");
            return;
        }


        // Créer une instance de la classe candidat avec les données récupérées
        Candidat candidat(nom, email, telephone,  experience);

        // Appeler la méthode ajouter() pour insérer les données dans la base de données
        bool success = candidat.ajouter();

        // Afficher un message en fonction du résultat de l'opération
        if (success) {


            QMessageBox::information(this, "Succès", "Le candidat a été ajouté avec succès.");
            // Optionnel : Effacer les champs après ajout
            ui->lineEdit_nom->clear();
            ui->lineEdit_mail->clear();
            ui->lineEdit_tel->clear();

            ui->lineEdit_exp->clear();
            // Optionnel : Mettre à jour l'affichage des candidats
            ui->tableView_candidat->setModel(candidat.afficher());

        } else {
            QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de l'ajout du candidat.");
        }
}



void MainWindow::on_modfi_ca_clicked()
{
    int id = ui->idCandidat->text().toInt();
        QString nom = ui->lineEdit_nom->text();
        QString email = ui->lineEdit_mail->text();
        QString telephone = ui->lineEdit_tel->text();

        QString experience = ui->lineEdit_exp->text();

        // Validation des champs
        if (nom.isEmpty() || email.isEmpty() || telephone.isEmpty() ) {
            QMessageBox::critical(nullptr, QObject::tr("Validation Error"),
                                  QObject::tr("Veuillez remplir tous les champs."),
                                  QMessageBox::Ok);
            return;
        }

        // Vérifier le format de l'email avec une expression régulière
           QRegularExpression emailRegex("^[\\w\\.-]+@[\\w\\.-]+\\.[a-zA-Z]{2,}$");
           if (!emailRegex.match(email).hasMatch()) {
               QMessageBox::warning(this, "Email invalide", "Veuillez entrer une adresse email valide.");
               return;
           }


        // Vérifier que le numéro de téléphone contient uniquement des chiffres, espaces ou '+'
        QRegularExpression phoneRegex("^\\+?[0-9 ]{7,15}$");
        if (!phoneRegex.match(telephone).hasMatch()) {
            QMessageBox::warning(this, "Téléphone invalide", "Veuillez entrer un numéro de téléphone valide (chiffres uniquement, avec ou sans '+').");
            return;
        }

        // Vérifier que le nom contient uniquement des lettres et des espaces
        QRegularExpression nameRegex("^[a-zA-Z\\s]+$");
        if (!nameRegex.match(nom).hasMatch()) {
            QMessageBox::warning(this, "Nom invalide", "Le nom ne doit contenir que des lettres et des espaces.");
            return;
        }

        Candidat candidat(id, nom, email, telephone,  experience);

        bool success = candidat.modifier(id);

        if (success) {

            ui->tableView_candidat->setModel(candidat.afficher());

            ui->idCandidat->clear();
            ui->lineEdit_nom->clear();
            ui->lineEdit_mail->clear();
            ui->lineEdit_tel->clear();

            ui->lineEdit_exp->clear();


            QMessageBox::information(nullptr, QObject::tr("Modifier un candidat"),
                                     QObject::tr("candidat modifié avec succès.\n"
                                                 "Cliquez sur Annuler pour quitter."),
                                     QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("ERREUR"),
                                  QObject::tr("Une erreur est survenue.\n"
                                              "Cliquez sur Annuler pour quitter."),
                                  QMessageBox::Cancel);
        }
}

void MainWindow::on_tableView_candidat_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
       int id = ui->tableView_candidat->model()->data(ui->tableView_candidat->model()->index(row, 0)).toInt();
       QString nom = ui->tableView_candidat->model()->data(ui->tableView_candidat->model()->index(row, 1)).toString();
       QString email = ui->tableView_candidat->model()->data(ui->tableView_candidat->model()->index(row, 2)).toString();
       QString telephone = ui->tableView_candidat->model()->data(ui->tableView_candidat->model()->index(row, 3)).toString();

       QString experience = ui->tableView_candidat->model()->data(ui->tableView_candidat->model()->index(row, 4)).toString();

       ui->idCandidat->setText(QString::number(id));
       ui->lineEdit_nom->setText(nom);
       ui->lineEdit_mail->setText(email);
       ui->lineEdit_tel->setText(telephone);

       ui->lineEdit_exp->setText(experience);
}

void MainWindow::on_supp_ca_clicked()
{
    QModelIndexList selectedIndexes = ui->tableView_candidat->selectionModel()->selectedIndexes();

        if (!selectedIndexes.isEmpty()) {
            // get id from selected row
            int id = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 0).data().toInt();

            // Call delete function
            bool supprime = candidat.supprimer(id);

            if (supprime) {
                ui->tableView_candidat->setModel(candidat.afficher());
                QMessageBox::information(this, "Suppression réussie", "Formateur supprimé avec succès.");
            } else {
                QMessageBox::warning(this, "Échec de la suppression", "Échec de la suppression du formateur.");
            }
        } else {
            QMessageBox::warning(this, "Aucun formateur sélectionné", "Veuillez sélectionner un formateur à supprimer.");
        }

}

void MainWindow::on_ajouter_offre_clicked()
{
    // Récupérer les valeurs des champs de l'interface utilisateur
        QString titre = ui->lineEdit_titre->text();
        QString description = ui->lineEdit_description->text();
        QDate date_publication  = ui->dateEdit_pub->date();

        double salaire = ui->lineEdit_salaire->text().toDouble();




        // Créer une instance de la classe candidat avec les données récupérées
        Offre offre(titre, description, date_publication,  salaire);

        // Appeler la méthode ajouter() pour insérer les données dans la base de données
        bool success = offre.ajouter();

        // Afficher un message en fonction du résultat de l'opération
        if (success) {


            QMessageBox::information(this, "Succès", "L'offre a été ajouté avec succès.");
            // Optionnel : Effacer les champs après ajout
            ui->lineEdit_titre->clear();
            ui->lineEdit_description->clear();
            ui->dateEdit_pub->clear();

            ui->lineEdit_salaire->clear();
            // Optionnel : Mettre à jour l'affichage des candidats
            ui->tableView_offre->setModel(offre.afficher());

        } else {
            QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de l'ajout d'offre.");
        }
}
