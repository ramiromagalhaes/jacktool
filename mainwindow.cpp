#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "PatchExtractorConfiguration.h"
#include "extract_patch.h"

#include <string>
#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QStringList>
#include <QDir>

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

    QStringList filter;
    filter.append(".jpg");
    filter.append(".png");
    filter.append(".bpm");
    QDir directory(path);
    QStringList images = directory.entryList(filter);

    for(QStringList::Iterator it = images.begin(); it != images.end(); ++it) {
        QString s = *it;
        imagesInSourceFolder.push_back(s.toAscii().constData());
    }

    currentImage = 0;

    displayNextImage();
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
    //TODO
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
    displayNextImage();
    extract_patches("/home/ramiro/Imagens/adhikari-preotimizacao-2.png", exclusions, cfg);
}

void MainWindow::setMarkerTool()
{
    this->marking = true;
}

void MainWindow::setEraserTool()
{
    this->marking = false;
}

void MainWindow::setPatchSize19x19()
{
    cfg.patchHeight = cfg.patchWidth = 19;

    this->ui->action20x20->setChecked(false);
    this->ui->action24x24->setChecked(false);
}

void MainWindow::setPatchSize20x20()
{
    cfg.patchHeight = cfg.patchWidth = 20;

    this->ui->action19x19->setChecked(false);
    this->ui->action24x24->setChecked(false);
}

void MainWindow::setPatchSize24x24()
{
    cfg.patchHeight = cfg.patchWidth = 24;

    this->ui->action19x19->setChecked(false);
    this->ui->action20x20->setChecked(false);
}

void MainWindow::displayNextImage()
{
    //cut the image and save the patches
    QImage qimg = QImage(imagesInSourceFolder.at(currentImage++));
    ui->image->setPixmap(QPixmap::fromImage(qimg));
    ui->image->resize(ui->image->pixmap()->size());
}
