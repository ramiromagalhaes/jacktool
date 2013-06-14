#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include "PatchExtractorConfiguration.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeSourceFolder()
{
    QString path = QFileDialog::getExistingDirectory(
                this,
                tr("Choose source folder"));
    this->sourceFolder = path.toAscii().constData();
}

void MainWindow::changeDestinationFolder()
{
    QString path = QFileDialog::getExistingDirectory(
                this,
                tr("Choose destination folder"));
    cfg.destinationFolder = path.toAscii().constData();
}

void MainWindow::changePatchSize()
{
}


void MainWindow::toggleTurn90()
{
    cfg.rotate90 = !cfg.rotate90;
}

void MainWindow::toggleTurn180()
{
    cfg.rotate180 = !cfg.rotate180;
}

void MainWindow::toggleTurn270()
{
    cfg.rotate270 = !cfg.rotate270;
}

void MainWindow::process()
{
    //cut the image and save the patches
    QImage qimg = QImage("/home/ramiro/Imagens/adhikari-preotimizacao-2.png");
    ui->image->setPixmap(QPixmap::fromImage(qimg));
    ui->image->resize(ui->image->pixmap()->size());
}

void MainWindow::setMarkerTool()
{
}

void MainWindow::setEraserTool()
{
}
