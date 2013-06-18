#ifndef IMAGEMARKER_H
#define IMAGEMARKER_H

#include "Rectangle.h"
#include <vector>
#include <QLabel>
#include <QMouseEvent>
#include <QResizeEvent>



//To use that in the Designer, see pages 117 and 118 of the book
class ImageMarker : public QLabel
{
    Q_OBJECT
    //Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)

public:
    explicit ImageMarker(QWidget *parent = 0);

    void useMarkTool();
    void useUnmarkTool();
    void handleClick(const int x, const int y);

protected:
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);
    void paintEvent(QPaintEvent *evt);
    void resizeEvent(QResizeEvent *evt);

signals:
    
public slots:

private:
    void beginRectangle(const int x, const int y);
    void endRectangle(const int x, const int y);
    void cancelRectangle();

    bool selectRectangle(const int x, const int y);
    void moveRectangle(const int x, const int y);

    void deleteRectangle(const int x, const int y);



    std::vector<Rectangle> restrictions;
    bool isMarkerSelected; //the marking tool is selected if == true

    //to be used when marking a certain region
    bool marking; //current state of the marking tool
    int clickX, clickY; //origin of the clicked area
    Rectangle * selected;
};

#endif // IMAGEMARKER_H
