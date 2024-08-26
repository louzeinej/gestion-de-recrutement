#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "candidat.h"
#include "offre.h"
#include "candidature.h"

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
Candidature candidature;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView_candidat->setModel(candidat.afficher());
     ui->tableView_offre->setModel(offre.afficher());
      ui->tableView_candidature->setModel(candidature.afficher());
     populatecandidatComboBox();
     populateOffreComboBox();
     populateStautComboBox();
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
            ui->dateEdit_pub->setDate(QDate::currentDate());

            ui->lineEdit_salaire->clear();
            // Optionnel : Mettre à jour l'affichage des candidats
            ui->tableView_offre->setModel(offre.afficher());

        } else {
            QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de l'ajout d'offre.");
        }
}

void MainWindow::on_modfiier_offre_clicked()
{
    int id = ui->idOffre->text().toInt();
    QString titre = ui->lineEdit_titre->text();
    QString description = ui->lineEdit_description->text();
    QDate date_publication  = ui->dateEdit_pub->date();

    double salaire = ui->lineEdit_salaire->text().toDouble();



       Offre offre(id, titre, description, date_publication,  salaire);

        bool success = offre.modifier(id);

        if (success) {

            ui->tableView_offre->setModel(offre.afficher());

            ui->idOffre->clear();
            ui->lineEdit_titre->clear();
            ui->lineEdit_description->clear();
            ui->dateEdit_pub->setDate(QDate::currentDate());

            ui->lineEdit_salaire->clear();


            QMessageBox::information(nullptr, QObject::tr("Modifier un offre"),
                                     QObject::tr("offre modifié avec succès.\n"
                                                 "Cliquez sur Annuler pour quitter."),
                                     QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("ERREUR"),
                                  QObject::tr("Une erreur est survenue.\n"
                                              "Cliquez sur Annuler pour quitter."),
                                  QMessageBox::Cancel);
        }
}

void MainWindow::on_tableView_offre_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
       int id = ui->tableView_offre->model()->data(ui->tableView_offre->model()->index(row, 0)).toInt();
       QString titre = ui->tableView_offre->model()->data(ui->tableView_offre->model()->index(row, 1)).toString();
       QString description = ui->tableView_offre->model()->data(ui->tableView_offre->model()->index(row, 2)).toString();
       QDate date_publication = ui->tableView_offre->model()->data(ui->tableView_offre->model()->index(row, 3)).toDate();

       double salaire = ui->tableView_offre->model()->data(ui->tableView_offre->model()->index(row, 4)).toDouble();

       ui->idOffre->setText(QString::number(id));
       ui->lineEdit_titre->setText(titre);
       ui->lineEdit_description->setText(description);
       ui->dateEdit_pub->setDate(date_publication);

      ui->lineEdit_salaire->setText(QString::number(salaire));
}

void MainWindow::on_supp_offre_clicked()
{
    QModelIndexList selectedIndexes = ui->tableView_offre->selectionModel()->selectedIndexes();

        if (!selectedIndexes.isEmpty()) {
            // get id from selected row
            int id = selectedIndexes.at(0).sibling(selectedIndexes.at(0).row(), 0).data().toInt();

            // Call delete function
            bool supprime = offre.supprimer(id);

            if (supprime) {
                ui->tableView_offre->setModel(offre.afficher());
                QMessageBox::information(this, "Suppression réussie", "Offrez supprimé avec succès.");
            } else {
                QMessageBox::warning(this, "Échec de la suppression", "Échec de la suppression d'offre.");
            }
        } else {
            QMessageBox::warning(this, "Aucun formateur sélectionné", "Veuillez sélectionner un offre à supprimer.");
        }
}

void MainWindow::on_lineEdit_rech_can_textChanged( QString terme)
{

    QSqlQueryModel* resultModel = Candidat::rechercherParTerme(terme);

    if (resultModel) {
        ui->tableView_candidat->setModel(resultModel);
    } else {
        // Gérer l'erreur, par exemple afficher un message d'erreur
        QMessageBox::critical(this, "Erreur de recherche", "Une erreur s'est produite lors de la recherche.");
    }
}

void MainWindow::on_tri_ca_clicked()
{
    QString champ = "nom";  // Default sort column
    QString order = "asc";  // Default sort order

    if (ui->comboBoxsortchamp_ca->currentText().compare("Email", Qt::CaseInsensitive) == 0) {
        champ = "email";
    } else if (ui->comboBoxsortchamp_ca->currentText().compare("Nom", Qt::CaseInsensitive) == 0) {
        champ = "nom";
    }

    if (ui->comboBox_order_ca->currentText().compare("Descendant", Qt::CaseInsensitive) == 0) {
        order = "desc";
    }

    qDebug() << "Order:" << order;
    qDebug() << "Champ:" << champ;

    ui->tableView_candidat->setModel(candidat.Tri(order, champ));
}
void MainWindow::generatePDFca( QString filePath)
{
    // Get the file path using QFileDialog
    QString fileName = filePath;
    if (fileName.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save PDF", QDir::homePath(), "PDF Files (*.pdf)");
    }

    // Check if the user canceled the dialog or didn't provide a file name
    if (fileName.isNull()) {
        qDebug() << "PDF generation canceled by the user.";
        return;
    }

    // Create a PDF writer
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A1));

    // Create a painter for the PDF
    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Retrieve data from the table view model and write it to the PDF
    int rowCount = ui->tableView_candidat->model()->rowCount();
    int colCount = ui->tableView_candidat->model()->columnCount();
    int cellWidth = pdfWriter.width() / colCount;
    int cellHeight = 500;
    int margin = 100;


    QFont font = painter.font();
    font.setPixelSize(200);
    painter.setFont(font);

    // Draw headers
    for (int col = 0; col < colCount; ++col) {
        QString headerData = ui->tableView_candidat->model()->headerData(col, Qt::Horizontal).toString();
        int x = col * cellWidth + margin;
        int y = 0;
        QRect headerRect(x, y, cellWidth - margin, cellHeight - margin);
        painter.drawText(headerRect, Qt::AlignLeft | Qt::AlignVCenter, headerData);
    }

    // Draw cell data
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            QModelIndex index = ui->tableView_candidat->model()->index(row, col);
            QString cellData = ui->tableView_candidat->model()->data(index, Qt::DisplayRole).toString();

            // Calculate cell position with padding
            int x = col * cellWidth + margin;
            int y = (row + 1) * cellHeight + margin;

            // Draw cell data with padding
            QRect cellRect(x, y, cellWidth - margin, cellHeight - margin);
            painter.drawText(cellRect, Qt::AlignLeft | Qt::AlignVCenter, cellData);
        }
    }

    // Finish painting
    painter.end();

    QMessageBox::information(this, "PDF Generated", "PDF generated successfully at:\n" + fileName);
}

void MainWindow::on_pdf_ca_clicked()
{
    generatePDFca("");
}
void MainWindow::populatecandidatComboBox() {
    QSqlQuery query("SELECT id, nom FROM candidat");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        ui->comboBox_idCandidat->addItem(name, id);
    }
}
void MainWindow::populateOffreComboBox() {
    QSqlQuery query("SELECT id, titre FROM offre");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString titre = query.value(1).toString();
        ui->comboBox_idOffre->addItem(titre, id);
    }
}
void MainWindow::populateStautComboBox() {
    ui->comboBox_statut->addItem("En attente");
    ui->comboBox_statut->addItem("En cours d'examen");

    ui->comboBox_statut->addItem("Accepté");
    ui->comboBox_statut->addItem("Refusé");

}

void MainWindow::on_ajouter_candidature_clicked()
{
    // Retrieve data from UI elements
        int idCandidat = ui->comboBox_idCandidat->currentData().toInt(); // Assuming a QComboBox for candidate IDs
        int idOffre = ui->comboBox_idOffre->currentData().toInt();       // Assuming a QComboBox for offer IDs
        QDate dateCandidature = ui->dateEdit_dateCandidature->date();    // Assuming a QDateEdit for the date
        QString statut = ui->comboBox_statut->currentText();             // Assuming a QComboBox for the status



        // Create a Candidature object
        Candidature candidature(idCandidat, idOffre, dateCandidature, statut);

        // Try to add the new candidature to the database
        if (candidature.ajouter()) {
            QMessageBox::information(this, "Success", "Candidature added successfully.");
            // Optionally, update the UI, such as refreshing a table view
            ui->tableView_candidature->setModel(candidature.afficher()); // Refresh the view
        } else {
            QMessageBox::critical(this, "Error", "Failed to add candidature.");
        }
}
