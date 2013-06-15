#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QMainWindow>
#include <vector>
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
    void changePatchSize();
    void toggleTurn90();
    void toggleTurn180();
    void toggleTurn270();
    void process();
    void setMarkerTool();
    void setEraserTool();
    void setPatchSize19x19();
    void setPatchSize20x20();
    void setPatchSize24x24();

    void displayNextImage();

private:
    Ui::MainWindow *ui;

    std::string sourceFolder; //where images will be taken from
    std::vector<std::string> imagesInSourceFolder; //images found in the source folder
    int currentImage; //the current image in imagesInSourceFolder being worked on

    PatchExtractorConfiguration cfg; //Holds some configuration for the patch creator.
                                     //Includes the destination folder

    std::vector<Rectangle> exclusions; //all areas that should not be exported

    //UI tools here
    bool marking; //the marking tool is selected if == true
};

#endif // MAINWINDOW_H
