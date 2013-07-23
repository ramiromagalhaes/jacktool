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
    QFileDialog dialog(this, tr("Source folder"),
                       QDir(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)).absolutePath());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    if(!dialog.exec()) {
        return;
    }
    QStringList files = dialog.selectedFiles();
    sourceFolder = QDir(files[0]);

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

    markings.changeBaseDirectory(sourceFolder.absolutePath().toAscii().constData());

    if ( reinforceCurrentImageIndexBoundaries() ) {
        return;
    }

    displayCurrentImage();
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
    //TODO display a wait screen
    markings.processAll(cfg);
}

void MainWindow::save()
{
    markings.save();

    ui->statusBar->showMessage("Saved.");
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

void MainWindow::previousImage()
{
    storeMarkings();

    currentImageIndex--;
    if (reinforceCurrentImageIndexBoundaries()) {
        return;
    }

    displayCurrentImage();
}

void MainWindow::nextImage()
{
    storeMarkings();

    currentImageIndex++;
    if (reinforceCurrentImageIndexBoundaries()) {
        return;
    }

    displayCurrentImage();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ui->image->resize(centralWidget()->size());
}

bool MainWindow::reinforceCurrentImageIndexBoundaries()
{
    if (imagesInSourceFolder.empty())
    {
        currentImageIndex = -1;
        return true;
    }

    if (currentImageIndex < 0) {
        currentImageIndex = 0;
        return false;
    }

    if (currentImageIndex >= imagesInSourceFolder.size())
    {
        currentImageIndex = imagesInSourceFolder.size() - 1;
        return false;
    }

    return false;
}

void MainWindow::storeMarkings()
{
    if (imagesInSourceFolder.empty())
    {
        return;
    }

    markings.set(
                imagesInSourceFolder.at(currentImageIndex).toAscii().constData(),
                ui->image->getExclusions());
}

void MainWindow::displayCurrentImage()
{
    //set the image to be shown
    QString filepath = sourceFolder.absoluteFilePath(
                imagesInSourceFolder.at(currentImageIndex));
    ui->image->setImageFromAbsolutePath(filepath);

    const std::vector<Rectangle> * m = markings.get( imagesInSourceFolder.at(currentImageIndex).toAscii().constData() );
    ui->image->setExclusions(*m);

    //show the image
    ui->centralWidget->update();
}
