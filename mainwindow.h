#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_ajouter_ca_clicked();


    void on_modfi_ca_clicked();

    void on_tableView_candidat_doubleClicked(const QModelIndex &index);

    void on_supp_ca_clicked();

    void on_ajouter_offre_clicked();

    void on_modfiier_offre_clicked();

    void on_tableView_offre_doubleClicked(const QModelIndex &index);

    void on_supp_offre_clicked();

    void on_lineEdit_rech_can_textChanged( QString terme);

    void on_tri_ca_clicked();
   void generatePDFca( QString filePath);

   void on_pdf_ca_clicked();
   void populatecandidatComboBox();
   void populateOffreComboBox();
   void populateStautComboBox();

   void on_ajouter_candidature_clicked();



   void on_pushButton_candidat_clicked();

   void on_pushButton_offre_clicked();

   void on_pushButton_candidature_clicked();
   void displaystatoffre();

   void on_pdf_offre_clicked();
   void generatePDFoffre( QString filePath);


   void on_tri_offre_clicked();

    void on_lineEdit_offre_textChanged(QString terme);

    void on_dateEdit_userDateChanged(const QDate &date);
    void sendStatusUpdateEmail(int idOffre, int idCandidat);

    void on_modifier_candidature_clicked();

    void on_tableView_candidature_doubleClicked(const QModelIndex &index);



    void on_supprimer_candidature_clicked();

    void on_uploadCVButton_clicked();
    void displayPNGImage(const QString &imagePath);



    void on_tableView_candidat_clicked(const QModelIndex &index);




    void updateImageDisplay();
    void zoomImage(qreal factor);



private:
    Ui::MainWindow *ui;
    QString currentCVFilePath;
    qreal zoomLevel;
        QPixmap originalPixmap;
        bool isDragging;
           QPoint lastMousePos;
};
#endif // MAINWINDOW_H
