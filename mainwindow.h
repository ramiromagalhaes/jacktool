#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QStringList>
#include <QDir>
#include <QResizeEvent>

#include "../jacktool-common/Rectangle.h"
#include "../jacktool-common/markings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void changeSourceFolder();
    void save();
    void previousImage();
    void nextImage();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    bool reinforceCurrentImageIndexBoundaries();
    void storeMarkings();
    void displayCurrentImage();

    Ui::MainWindow *ui;

    QDir sourceFolder; //where images will be taken from
    QStringList imagesInSourceFolder;//images found in the source folder
    int currentImageIndex; //the current image in imagesInSourceFolder being worked on

    Markings markings;
};

#endif // MAINWINDOW_H
