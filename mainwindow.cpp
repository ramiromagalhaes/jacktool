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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    sourceFolder = QDir(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)).absolutePath();

    QString destFolder = QDir(QDesktopServices::storageLocation(QDesktopServices::PicturesLocation)).absolutePath();
    destFolder += QDir::separator();

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

    markingsSaveLocation.clear();
    markings.reset();
    markings.setBaseDirectory( sourceFolder.absolutePath().toAscii().constData() );
    markings.loadFilenamesFromBaseDirectory();

    {
        QString message;
        message.append("Found ");
        message.append(QString::number(imagesInSourceFolder.size()));
        message.append(" images in folder ");
        message.append(sourceFolder.absolutePath());

        ui->statusBar->showMessage(message);
    }

    if ( reinforceCurrentImageIndexBoundaries() ) {
        return;
    }

    currentImageIndex = 0;
    displayCurrentImage();
}

void MainWindow::save()
{
    if (markingsSaveLocation.isEmpty())
    {
        QFileDialog dialog(this, tr("Save markings"), sourceFolder.absolutePath());
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        if(!dialog.exec()) {
            return;
        }
        QStringList files = dialog.selectedFiles();
        markingsSaveLocation = files[0];
    }

    if ( markings.save(markingsSaveLocation.toAscii().constData()) )
    {
        ui->statusBar->showMessage("Saved.");
    }
    else
    {
        //warn user
        QMessageBox msgBox;
        msgBox.setText("Couldn't save.");
        msgBox.setInformativeText("Could not save your markings. Do you have permission to write on the source images directory? Is there enought free memory there?");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::load()
{
    QFileDialog dialog(this, tr("Load a jacktool data file"), sourceFolder.absolutePath());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::AnyFile);
    if(!dialog.exec()) {
        return;
    }
    QStringList files = dialog.selectedFiles();
    markingsSaveLocation = files[0];

    markings.reset();
    markings.loadFilenamesFromBaseDirectory();
    if ( !markings.load(markingsSaveLocation.toAscii().constData()) )
    {
        //warn user
        QMessageBox msgBox;
        msgBox.setText("Couldn't load file.");
        msgBox.setInformativeText("Could not load markings data.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    sourceFolder = QDir(markings.getBaseDirectory().c_str());
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

    if ( reinforceCurrentImageIndexBoundaries() ) {
        return;
    }

    currentImageIndex = 0;
    displayCurrentImage();

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

void MainWindow::firstImage()
{
    storeMarkings();

    currentImageIndex = 0;
    if (reinforceCurrentImageIndexBoundaries()) {
        return;
    }

    displayCurrentImage();
}

void MainWindow::lastImage()
{
    storeMarkings();

    currentImageIndex = imagesInSourceFolder.size() - 1;
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
