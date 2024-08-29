#include "poppler.h"
#include <QVBoxLayout>
#include <QPdfPage>
#include <QDebug>

PopplerViewer::PopplerViewer(QWidget *parent)
    : QWidget(parent),
      pdfDocument(new QPdfDocument(this)),
      pdfPageRenderer(new QPdfPageRenderer(this)),
      pdfLabel(new QLabel(this))
{
    initialize();
}

PopplerViewer::~PopplerViewer()
{
    delete pdfDocument;
    delete pdfPageRenderer;
    delete pdfLabel;
}

void PopplerViewer::initialize()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(pdfLabel);
    setLayout(layout);
}

void PopplerViewer::loadPdf(const QString &filePath)
{
    pdfDocument->load(filePath);

    if (pdfDocument->error() != QPdfDocument::NoError) {
        qWarning() << "Failed to load PDF:" << pdfDocument->errorString();
        return;
    }

    showPage(0); // Show the first page by default
}

void PopplerViewer::showPage(int pageNumber)
{
    if (pageNumber < 0 || pageNumber >= pdfDocument->pageCount()) {
        qWarning() << "Invalid page number:" << pageNumber;
        return;
    }

    QPdfPage *page = pdfDocument->page(pageNumber);
    if (!page) {
        qWarning() << "Failed to get page:" << pageNumber;
        return;
    }

    QPixmap pixmap = pdfPageRenderer->render(page, pdfLabel->size());
    pdfLabel->setPixmap(pixmap);
}
