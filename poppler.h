#ifndef POPPLER_H
#define POPPLER_H

#include <QString>
#include <QWidget>
#include <QPdfDocument>
#include <QPdfPageRenderer>
#include <QPixmap>
#include <QLabel>

class PopplerViewer : public QWidget
{
    Q_OBJECT

public:
    explicit PopplerViewer(QWidget *parent = nullptr);
    ~PopplerViewer();

    void loadPdf(const QString &filePath);
    void showPage(int pageNumber);

private:
    QPdfDocument *pdfDocument;
    QPdfPageRenderer *pdfPageRenderer;
    QLabel *pdfLabel;

    void initialize();
};

#endif // POPPLER_H
