#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QStringList>
#include <QDir>
#include <QResizeEvent>

#include "Rectangle.h"
#include "PatchExtractorConfiguration.h"

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
    void changeDestinationFolder();
    void toggleTurn90();
    void toggleTurn180();
    void toggleTurn270();
    void process();
    void ignore();
    void setMarkerTool();
    void setEraserTool();
    void setPatchSize19x19();
    void setPatchSize20x20();
    void setPatchSize24x24();

    void displayNextImage();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;

    QDir sourceFolder; //where images will be taken from
    QStringList imagesInSourceFolder;//images found in the source folder
    int currentImageIndex; //the current image in imagesInSourceFolder being worked on


    PatchExtractorConfiguration cfg; //Holds some configuration for the patch creator.
                                     //Includes the destination folder

    std::vector<Rectangle> exclusions; //all areas that should not be exported
};

#endif // MAINWINDOW_H
