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
#include "smtp.h"






QT_CHARTS_USE_NAMESPACE



Candidat candidat;
Offre offre;
Candidature candidature;




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) ,zoomLevel(1.0)
{
    ui->setupUi(this);
    ui->tableView_candidat->setModel(candidat.afficher());
     ui->tableView_offre->setModel(offre.afficher());
      ui->tableView_candidature->setModel(candidature.afficher());
     populatecandidatComboBox();
     populateOffreComboBox();
     populateStautComboBox();
     displaystatoffre();
     connect(ui->tableView_candidat, SIGNAL(clicked(QModelIndex)), this, SLOT(onTableViewClicked(QModelIndex)));

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
        QString cvFileName = ui->LineEdit_cv->text();  // Récupérer le nom du fichier CV depuis LineEdit_cv

        // Vérification des champs
        if (nom.isEmpty() || email.isEmpty() || telephone.isEmpty() || experience.isEmpty()) {
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

        // Vérifier si un CV est sélectionné (ici, nous considérons que LineEdit_cv contient le chemin du fichier)
        if (cvFileName.isEmpty()) {
            QMessageBox::warning(this, "CV manquant", "Veuillez sélectionner un fichier CV.");
            return;
        }

        // Créer une instance de la classe candidat avec les données récupérées
        Candidat candidat(nom, email, telephone, experience, cvFileName);

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
            ui->LineEdit_cv->clear();  // Effacer le champ de CV

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
        QString cvFileName = ui->LineEdit_cv->text();  // Récupérer le nom du fichier CV depuis LineEdit_cv

        // Validation des champs
        if (nom.isEmpty() || email.isEmpty() || telephone.isEmpty()) {
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

        // Créer une instance de la classe candidat avec les données récupérées
        Candidat candidat(id, nom, email, telephone, experience, cvFileName);

        // Appeler la méthode modifier() pour mettre à jour les données dans la base de données
        bool success = candidat.modifier(id);

        if (success) {
            // Optionnel : Mettre à jour l'affichage des candidats
            ui->tableView_candidat->setModel(candidat.afficher());

            // Effacer les champs après modification
            ui->idCandidat->clear();
            ui->lineEdit_nom->clear();
            ui->lineEdit_mail->clear();
            ui->lineEdit_tel->clear();
            ui->lineEdit_exp->clear();
            ui->LineEdit_cv->clear();  // Effacer le champ de CV

            QMessageBox::information(nullptr, QObject::tr("Modifier un candidat"),
                                     QObject::tr("Candidat modifié avec succès.\n"
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
       QString cv = ui->tableView_candidat->model()->data(ui->tableView_candidat->model()->index(row, 5)).toString();

       ui->idCandidat->setText(QString::number(id));
       ui->lineEdit_nom->setText(nom);
       ui->lineEdit_mail->setText(email);
       ui->lineEdit_tel->setText(telephone);

       ui->lineEdit_exp->setText(experience);
       ui->LineEdit_cv->setText(cv);
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
        qDebug() << "Candidat added:" << name << id;  // Debug line
    }
}

void MainWindow::populateOffreComboBox() {
    QSqlQuery query("SELECT id, titre FROM offre");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString titre = query.value(1).toString();
        ui->comboBox_idOffre->addItem(titre, id);
        qDebug() << "Offre added:" << titre << id;  // Debug line
    }
}
void MainWindow::populateStautComboBox() {
    ui->comboBox_statut->addItem("En attente");
    ui->comboBox_statut->addItem("En cours d'examen");

    ui->comboBox_statut->addItem("Accepte");
    ui->comboBox_statut->addItem("Refuse");

}





void MainWindow::on_pushButton_candidat_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_offre_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_candidature_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::displaystatoffre() {
    // Vérifiez si ui->frameOffre a déjà un layout, sinon, créez-en un
        if (!ui->frame_offre->layout()) {
            QVBoxLayout *frameLayout = new QVBoxLayout(ui->frame_offre);
            ui->frame_offre->setLayout(frameLayout);
        }

        // Supprimez tous les widgets existants dans le QFrame
        QLayoutItem *item;
        while ((item = ui->frame_offre->layout()->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }

        // Récupérez les données à afficher dans le graphique
        Offre o;
        QBarSeries *series = o.getStatOffresParTitre();

        if (!series) {
            qDebug() << "Failed to retrieve statistics data from the database.";
            return;
        }

        // Créez le graphique à barres
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Nombre d'offres par titre de poste");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // Créez l'axe des catégories (axe X)
        QBarCategoryAxis *axisX = new QBarCategoryAxis;
        axisX->setTitleText("Titre de Poste");
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        // Créez l'axe des valeurs (axe Y)
        QValueAxis *axisY = new QValueAxis;
        axisY->setTitleText("Nombre d'offres");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Créez la vue du graphique
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing, true);

        // Ajustez la taille de la vue du graphique
        chartView->setFixedSize(721, 281);

        // Ajoutez la vue du graphique au layout du widget parent
        ui->frame_offre->layout()->addWidget(chartView);
}


void MainWindow::on_pdf_offre_clicked()
{
generatePDFoffre( "");
}
void MainWindow::generatePDFoffre( QString filePath)
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
    int rowCount = ui->tableView_offre->model()->rowCount();
    int colCount = ui->tableView_offre->model()->columnCount();
    int cellWidth = pdfWriter.width() / colCount;
    int cellHeight = 500;
    int margin = 100;


    QFont font = painter.font();
    font.setPixelSize(200);
    painter.setFont(font);

    // Draw headers
    for (int col = 0; col < colCount; ++col) {
        QString headerData = ui->tableView_offre->model()->headerData(col, Qt::Horizontal).toString();
        int x = col * cellWidth + margin;
        int y = 0;
        QRect headerRect(x, y, cellWidth - margin, cellHeight - margin);
        painter.drawText(headerRect, Qt::AlignLeft | Qt::AlignVCenter, headerData);
    }

    // Draw cell data
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            QModelIndex index = ui->tableView_offre->model()->index(row, col);
            QString cellData = ui->tableView_offre->model()->data(index, Qt::DisplayRole).toString();

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



void MainWindow::on_tri_offre_clicked()
{
    QString champ = "titre";  // Default sort column
    QString order = "asc";  // Default sort order

    if (ui->comboBoxsortchamp_offre->currentText().compare("Titre", Qt::CaseInsensitive) == 0) {
        champ = "titre";
    } else if (ui->comboBoxsortchamp_offre->currentText().compare("Salaire", Qt::CaseInsensitive) == 0) {
        champ = "salaire";
    }

    if (ui->comboBox_order_offre->currentText().compare("Descendant", Qt::CaseInsensitive) == 0) {
        order = "desc";
    }

    qDebug() << "Order:" << order;
    qDebug() << "Champ:" << champ;

    ui->tableView_offre->setModel(offre.Tri(order, champ));
}

void MainWindow::on_lineEdit_offre_textChanged( QString terme)
{
    QSqlQueryModel* resultModel = Offre::rechercherParTerme(terme);

    if (resultModel) {
        ui->tableView_offre->setModel(resultModel);
    } else {
        // Gérer l'erreur, par exemple afficher un message d'erreur
        QMessageBox::critical(this, "Erreur de recherche", "Une erreur s'est produite lors de la recherche.");
    }
}

void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    // Create an instance of the Formation class
       Offre offre;

       // Call the function to search formations by journee
       QSqlQueryModel* model = offre.rechercherParDatePublication(date);

       // Set the model to the QTableView
       ui->tableView_offre->setModel(model);

}


void MainWindow::sendStatusUpdateEmail(int idOffre, int idCandidat) {
    // Préparer la requête pour récupérer les informations du candidat et de la candidature
    QSqlQuery query;
    query.prepare("SELECT c.nom, c.email, can.statut, o.titre "
                  "FROM candidature can "
                  "JOIN candidat c ON can.id_candidat = c.id "
                  "JOIN offre o ON can.id_offre = o.id "
                  "WHERE can.id_offre = :idOffre AND can.id_candidat = :idCandidat");
    query.bindValue(":idOffre", idOffre);
    query.bindValue(":idCandidat", idCandidat);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve candidate and application details:" << query.lastError().text();
        return;
    }

    // Vérifier si la requête a renvoyé un résultat
    if (query.next()) {
        QString candidatNom = query.value("nom").toString();
        QString candidatEmail = query.value("email").toString();
        QString statut = query.value("statut").toString();
        QString titreOffre = query.value("titre").toString();

        // Déterminer le contenu de l'e-mail en fonction du statut de la candidature
        QString subject = QString("Mise à jour du statut de votre candidature pour le poste '%1'").arg(titreOffre);
        QString body;

        if (statut == "accepté") {
            body = QString("Bonjour %1,\n\n"
                           "Nous avons le plaisir de vous informer que votre candidature pour le poste '%2' a été acceptée.\n"
                           "Félicitations !\n\n"
                           "Nous vous contacterons bientôt pour les prochaines étapes.\n\n"
                           "Cordialement,\n"
                           "L'équipe de recrutement.")
                           .arg(candidatNom)
                           .arg(titreOffre);
        } else if (statut == "refusé") {
            body = QString("Bonjour %1,\n\n"
                           "Nous souhaitons vous informer que votre candidature pour le poste '%2' a été refusée.\n"
                           "Nous vous remercions pour votre intérêt et nous vous souhaitons bonne chance dans vos recherches.\n\n"
                           "Cordialement,\n"
                           "L'équipe de recrutement.")
                           .arg(candidatNom)
                           .arg(titreOffre);
        } else {
            body = QString("Bonjour %1,\n\n"
                           "Nous souhaitons vous informer que le statut de votre candidature pour le poste '%2' a changé.\n"
                           "Nouveau statut : %3\n\n"
                           "Merci de votre intérêt pour notre entreprise.\n\n"
                           "Cordialement,\n"
                           "L'équipe de recrutement.")
                           .arg(candidatNom)
                           .arg(titreOffre)
                           .arg(statut);
        }

        // Configurer et envoyer l'e-mail via un serveur SMTP
        Smtp *smtp = new Smtp("jery.wizin@gmail.com", "tqyb pqwu pzzl pchx", "smtp.example.com", 465);
        smtp->sendMail("jery.wizin@gmail.com", candidatEmail, subject, body);

        qDebug() << "Email sent to" << candidatEmail << "about the status change.";
    } else {
        qDebug() << "No candidate found for the provided application IDs.";
    }
}








void MainWindow::on_ajouter_candidature_clicked()
{
    // Récupérer les données depuis les éléments de l'interface utilisateur
        int idCandidat = ui->comboBox_idCandidat->currentData().toInt(); // Assuming a QComboBox for candidate IDs
        int idOffre = ui->comboBox_idOffre->currentData().toInt();       // Assuming a QComboBox for offer IDs
        QDate dateCandidature = ui->dateEdit_dateCandidature->date();    // Assuming a QDateEdit for the date
        QString statut = ui->comboBox_statut->currentText();             // Assuming a QComboBox for the status

        // Créer un objet Candidature
        Candidature candidature(idCandidat, idOffre, dateCandidature, statut);

        // Essayer d'ajouter la nouvelle candidature à la base de données
        if (candidature.ajouter()) {
            QMessageBox::information(this, "Succès", "Candidature ajoutée avec succès.");

            // Optionnel : mettre à jour l'interface utilisateur, par exemple en actualisant une vue de table
            ui->tableView_candidature->setModel(candidature.afficher()); // Actualiser la vue

            // Nettoyer les champs après l'ajout
            ui->comboBox_idCandidat->setCurrentIndex(-1); // Réinitialiser le QComboBox pour ID Candidat
            ui->comboBox_idOffre->setCurrentIndex(-1);    // Réinitialiser le QComboBox pour ID Offre
            ui->dateEdit_dateCandidature->clear();        // Réinitialiser le QDateEdit
            ui->comboBox_statut->setCurrentIndex(-1);     // Réinitialiser le QComboBox pour le statut

        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la candidature.");
        }
}

void MainWindow::on_modifier_candidature_clicked()
{
    int idCandidat = ui->comboBox_idCandidat->currentData().toInt(); // Assuming a QComboBox for candidate IDs
       int idOffre = ui->comboBox_idOffre->currentData().toInt();       // Assuming a QComboBox for offer IDs
       QDate dateCandidature = ui->dateEdit_dateCandidature->date();    // Assuming a QDateEdit for the date
       QString statut = ui->comboBox_statut->currentText();

       // Créer une instance de Candidature et appeler la méthode de modification
       Candidature candidature(idCandidat, idOffre, dateCandidature, statut);
       bool success = candidature.modifier(idCandidat,idOffre);

       if (success) {
          //sendStatusUpdateEmail(idOffre, idCandidat);

           // Actualiser le modèle de la table de candidatures
           ui->tableView_candidature->setModel(candidature.afficher());

           // Afficher un message de succès
           QMessageBox::information(nullptr, QObject::tr("Modifier une candidature"),
                                    QObject::tr("Candidature modifiée avec succès.\n"
                                                "Cliquez sur Annuler pour quitter."),
                                    QMessageBox::Cancel);

           // Nettoyer les attributs après la modification
           ui->comboBox_idCandidat->setCurrentIndex(-1); // Réinitialiser le QComboBox pour ID Candidat
           ui->comboBox_idOffre->setCurrentIndex(-1);    // Réinitialiser le QComboBox pour ID Offre
           ui->dateEdit_dateCandidature->clear();        // Réinitialiser le QDateEdit
           ui->comboBox_statut->setCurrentIndex(-1);     // Réinitialiser le QComboBox pour le statut

       } else {
           // Afficher un message d'erreur en cas d'échec
           QMessageBox::critical(nullptr, QObject::tr("ERREUR"),
                                 QObject::tr("Une erreur est survenue.\n"
                                             "Cliquez sur Annuler pour quitter."),
                                 QMessageBox::Cancel);
       }
}

void MainWindow::on_tableView_candidature_doubleClicked(const QModelIndex &index)
{

    int row = index.row();

        // Retrieve data from the table view
        QString titreOffre = ui->tableView_candidature->model()->data(ui->tableView_candidature->model()->index(row, 0)).toString();
        QString nomCandidat = ui->tableView_candidature->model()->data(ui->tableView_candidature->model()->index(row, 1)).toString();
        QVariant dateCandidatureVariant = ui->tableView_candidature->model()->data(ui->tableView_candidature->model()->index(row, 2));
        QString statut = ui->tableView_candidature->model()->data(ui->tableView_candidature->model()->index(row, 3)).toString();

        // Debug output
        qDebug() << "Selected Titre Offre:" << titreOffre;
        qDebug() << "Selected Nom Candidat:" << nomCandidat;
        qDebug() << "Selected Date Candidature Variant:" << dateCandidatureVariant;
        qDebug() << "Selected Statut:" << statut;

        // Convert QVariant to QDate, handling possible invalid values
        QDate dateCandidature;
        if (dateCandidatureVariant.canConvert<QDate>()) {
            dateCandidature = dateCandidatureVariant.toDate();
        } else {
            dateCandidature = QDate(); // Set to an invalid date if conversion fails
            qDebug() << "Invalid or empty date detected.";
        }

        // Debug output
        qDebug() << "Selected Date Candidature:" << dateCandidature;

        // Find index based on text
        int indexCandidat = ui->comboBox_idCandidat->findText(nomCandidat);
        int indexOffre = ui->comboBox_idOffre->findText(titreOffre);

        // Get IDs based on index
        int idCandidat = ui->comboBox_idCandidat->itemData(indexCandidat).toInt();
        int idOffre = ui->comboBox_idOffre->itemData(indexOffre).toInt();

        qDebug() << "Selected ID Offre:" << idOffre;
        qDebug() << "Selected ID Candidat:" << idCandidat;

        // Set the UI elements if IDs are valid
        if (indexCandidat != -1) {
            ui->comboBox_idCandidat->setCurrentIndex(indexCandidat);
        } else {
            qDebug() << "Candidat ID not found in combo box";
        }

        if (indexOffre != -1) {
            ui->comboBox_idOffre->setCurrentIndex(indexOffre);
        } else {
            qDebug() << "Offre ID not found in combo box";
        }

        ui->dateEdit_dateCandidature->setDate(dateCandidature);
        ui->comboBox_statut->setCurrentText(statut);
    }



void MainWindow::on_supprimer_candidature_clicked()
{
    QModelIndexList selectedIndexes = ui->tableView_candidature->selectionModel()->selectedIndexes();

        if (!selectedIndexes.isEmpty()) {
            int row = selectedIndexes.at(0).row();

            // Retrieve data from the selected row
            QString titreOffre = ui->tableView_candidature->model()->data(ui->tableView_candidature->model()->index(row, 0)).toString();
            QString nomCandidat = ui->tableView_candidature->model()->data(ui->tableView_candidature->model()->index(row, 1)).toString();

            // Find the corresponding indices in the combo boxes
            int indexCandidat = ui->comboBox_idCandidat->findText(nomCandidat);
            int indexOffre = ui->comboBox_idOffre->findText(titreOffre);

            // Check if the candidate and offer are found in the combo boxes
            if (indexCandidat != -1 && indexOffre != -1) {
                // Retrieve IDs from the combo boxes
                int idCandidat = ui->comboBox_idCandidat->itemData(indexCandidat).toInt();
                int idOffre = ui->comboBox_idOffre->itemData(indexOffre).toInt();

                // Debug output
                qDebug() << "Deleting candidature with ID Candidat:" << idCandidat << "and ID Offre:" << idOffre;

                // Call the delete function with the retrieved IDs
                bool supprime = candidature.supprimer(idCandidat, idOffre);

                if (supprime) {
                    ui->tableView_candidature->setModel(candidature.afficher());
                    QMessageBox::information(this, "Suppression réussie", "Candidature supprimée avec succès.");
                } else {
                    QMessageBox::warning(this, "Échec de la suppression", "Échec de la suppression de la candidature.");
                }
            } else {
                // Display an error if either the candidate or offer is not found
                QMessageBox::warning(this, "Erreur", "Candidat ou offre non trouvé dans les listes déroulantes.");
                qDebug() << "Candidat or Offre ID not found in combo box.";
            }
        } else {
            QMessageBox::warning(this, "Aucune candidature sélectionnée", "Veuillez sélectionner une candidature à supprimer.");
        }
}

void MainWindow::on_uploadCVButton_clicked()
{
    QFileDialog dialog(this);
       dialog.setFileMode(QFileDialog::ExistingFiles);

       // Update the filter to allow both PDF and PNG files
       dialog.setNameFilter(tr("PDF Files (*.pdf);;PNG Files (*.png);;All Files (*.*)"));
       dialog.setViewMode(QFileDialog::List);

       if (dialog.exec()) {
           QStringList fileNames = dialog.selectedFiles();
           if (!fileNames.isEmpty()) {
               QString fileName = fileNames.first();  // Get the first selected file
               ui->LineEdit_cv->setText(fileName);    // Display the file path in the QLineEdit
               currentCVFilePath = fileName;          // Store the file path in the variable
           }
       }
}




// Function to display a PNG image in a QLabel
void MainWindow::displayPNGImage(const QString &imagePath)
{
    if (imagePath.isEmpty()) {
           // Handle error if no image path is provided
           qDebug() << "No image path provided.";
           return;
       }

       // Create a QPixmap object and load the image
       QPixmap newImage(imagePath);

       // Check if the image was successfully loaded
       if (newImage.isNull()) {
           // Handle error if the image could not be loaded
           qDebug() << "Failed to load image:" << imagePath;
           return;
       }

       // Clear the previous image from the QLabel
       ui->pdfContainerWidget->clear();

       // Store the new image and reset zoom level
       originalPixmap = newImage;
       zoomLevel = 1.0;

       // Display the new image
       updateImageDisplay();
}
void MainWindow::zoomImage(qreal factor)
{
    zoomLevel *= factor;  // Update zoom level
    updateImageDisplay(); // Update image display with new zoom level
}
void MainWindow::updateImageDisplay()
{
    if (zoomLevel <= 0.1) zoomLevel = 0.1;  // Prevent zooming out too much

       QPixmap scaledPixmap = originalPixmap.scaled(originalPixmap.size() * zoomLevel, Qt::KeepAspectRatio, Qt::SmoothTransformation);
       ui->pdfContainerWidget->setPixmap(scaledPixmap);

       // Adjust the size of the QLabel to match the scaled image
       ui->pdfContainerWidget->resize(scaledPixmap.size());

       // Optionally reset scroll bars to ensure the image fits within view
      // ui->scrollArea->horizontalScrollBar()->setValue(0);
       //ui->scrollArea->verticalScrollBar()->setValue(0);
}





void MainWindow::on_tableView_candidat_clicked(const QModelIndex &index)
{

    // Check if the clicked column is the 'cv' column
       int cvColumn = 5; // Assuming 'cv' is the fifth column, adjust based on your table structure

       if (index.column() == cvColumn) {
           // Retrieve the file path from the model
           QString filePath = index.data().toString();

           // Check if the file is a PNG
            if (filePath.endsWith(".png", Qt::CaseInsensitive) || filePath.endsWith(".jpg", Qt::CaseInsensitive) || filePath.endsWith(".jpeg", Qt::CaseInsensitive)) {
               displayPNGImage(filePath);  // Function to display the image
           } else {
               // Handle other file types or display an error message
               qDebug() << "File is not a PNG: " << filePath;
           }
       }
}




