#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;

    bool marking; //the marking tool is selected if == true

    std::string sourceFolder;
    PatchExtractorConfiguration cfg;
};

#endif // MAINWINDOW_H
