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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
