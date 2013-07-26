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
    sourceFolder = QDir(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)).absolutePath();

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
    QFileDialog dialog(this, tr("Source folder"), sourceFolder.absolutePath());
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
    filter.append("*.pgm");
    filter.append("*.JPG");
    filter.append("*.PNG");
    filter.append("*.BMP");
    filter.append("*.PGM");
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
    QFileDialog dialog(this, tr("Destination folder"),
                       cfg.destinationFolder.c_str());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    if(!dialog.exec()) {
        return;
    }
    QStringList files = dialog.selectedFiles();
    cfg.destinationFolder = files[0].toAscii().constData();

    {
        QString message;
        message.append("Destination folder now is ");
        message.append(files[0]);

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
    if (markings.save())
    {
        ui->statusBar->showMessage("Saved.");
    }
    else
    {
        //warn user
        QMessageBox msgBox;
        msgBox.setText("Couldn't save.");
        msgBox.setInformativeText("Could not save your markings. Do you have permission to write on the source images directory?");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
    }
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

//TODO FIXME it will only store markings if I move to the previous or next image.
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
