#ifndef IMAGEMARKER_H
#define IMAGEMARKER_H

#include "Rectangle.h"
#include <vector>
#include <QLabel>
#include <QPen>
#include <QRubberBand>


class QMouseEvent;
class QPaintEvent;
class QResizeEvent;

//To use that in the Designer, see pages 117 and 118 of the book
class ImageMarker : public QLabel
{
    Q_OBJECT

public:
    explicit ImageMarker(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *evt);

    void paintEvent(QPaintEvent *evt);
    void resizeEvent(QResizeEvent *evt);

private:
    bool isReady();

    void handleRightClick(QMouseEvent *evt);
    void handleLeftClick(QMouseEvent *evt);
    void handleRightRelease(QMouseEvent *evt);
    void handleLeftRelease(QMouseEvent *evt);

    void updateRubberBandRegion();


    QPen rubberbandPen;
    QPen exclusionsPen;

    //will show up whenever the user is defining an exclusion area
    QRect rubberband;

    //exclusion markers added by the user
    std::vector<Rectangle> exclusions;

    //when a user is marking a face it'll be equal to true
    bool marking;

};

#endif // IMAGEMARKER_H
