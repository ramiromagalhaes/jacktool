#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <sstream>
#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QStringList>
#include <QDir>
#include <QResizeEvent>
#include <QDesktopServices>
#include <QMessageBox>

#include "PatchExtractorConfiguration.h"
#include "extract_patch.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString destFolder = QDir(QDesktopServices::storageLocation(QDesktopServices::PicturesLocation)).absolutePath();
    destFolder += QDir::separator();
    this->cfg.destinationFolder = destFolder.toAscii().data();

    currentImageIndex = -1;

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
    sourceFolder = QDir(path);

    QStringList filter;
    filter.append("*.jpg");
    filter.append("*.png");
    filter.append("*.bmp");
    filter.append("*.JPG");
    filter.append("*.PNG");
    filter.append("*.BMP");

    imagesInSourceFolder = sourceFolder.entryList(filter);

    {
        QString message;
        message.append("Found ");
        message.append(QString::number(imagesInSourceFolder.size()));
        message.append(" images in folder ");
        message.append(sourceFolder.absolutePath());

        ui->statusBar->showMessage(message);
    }

    currentImageIndex = 0;

    displayNextImage();
}

void MainWindow::changeDestinationFolder()
{
    QString path = QFileDialog::getExistingDirectory(
                this,
                tr("Choose destination folder"),
                cfg.destinationFolder.c_str());
    path.append("/");

    cfg.destinationFolder = path.toAscii().constData();

    {
        QString message;
        message.append("Destination folder now is ");
        message.append(path);

        ui->statusBar->showMessage(message);
    }
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
    if (currentImageIndex == -1 ||
        imagesInSourceFolder.isEmpty() ||
        currentImageIndex >= imagesInSourceFolder.size()) {
        //TODO display error message.
        return;
    }

    QString path = sourceFolder.absoluteFilePath(imagesInSourceFolder.at(currentImageIndex));
    extract_patches(path.toAscii().constData(),
                    imagesInSourceFolder.at(currentImageIndex).toAscii().constData(),
                    exclusions,
                    cfg);

    displayNextImage();
}

void MainWindow::ignore()
{
    if (currentImageIndex == -1 ||
        imagesInSourceFolder.isEmpty() ||
        currentImageIndex >= imagesInSourceFolder.size()) {
        //TODO display error message.
        return;
    }

    displayNextImage();
}

void MainWindow::setMarkerTool()
{
    //this->marking = true;
}

void MainWindow::setEraserTool()
{
    //this->marking = false;
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
    if (currentImageIndex == -1 || imagesInSourceFolder.empty() || currentImageIndex >= imagesInSourceFolder.size()) {
        //TODO warn the user: no more images to display and process
        return;
    }

    //set the image to be shown
    QString filepath = sourceFolder.absoluteFilePath(
                imagesInSourceFolder.at(currentImageIndex++));
    ui->image->setImageFromAbsolutePath(filepath);

    //show the image
    ui->centralWidget->update();
}

//changes the size of the label that holds the image when the window size changes
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    ui->image->resize(centralWidget()->size());
}
