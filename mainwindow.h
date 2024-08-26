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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
