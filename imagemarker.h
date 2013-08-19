#ifndef IMAGEMARKER_H
#define IMAGEMARKER_H

#include "../jacktool-common/Rectangle.h"
#include <vector>
#include <QLabel>
#include <QPen>
#include <QString>
#include <QPixmap>



class QMouseEvent;
class QPaintEvent;
class QResizeEvent;



//To use that in the Designer, see pages 117 and 118 of the book
class ImageMarker : public QLabel
{
    Q_OBJECT

public:
    explicit ImageMarker(QWidget *parent = 0);

    void setImageFromAbsolutePath(QString &path);
    std::vector<Rectangle> * getExclusions();
    void setExclusions(const std::vector<Rectangle> &exclusions_);

protected:
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *evt);

    void paintEvent(QPaintEvent *evt);
    void resizeEvent(QResizeEvent *evt);

private:
    bool isReady();

    void handleLeftClick(QMouseEvent *evt);
    void handleRightRelease(QMouseEvent *evt);
    void handleLeftRelease(QMouseEvent *evt);

    void updateRubberBandRegion();

    void updateExcludedRegion(Rectangle &r);

    void updateBufferDisplayRatio();

    QPen rubberbandPen;
    QPen exclusionsPen;
    QRect rubberband; //will show up whenever the user is defining an exclusion area

    std::vector<Rectangle> exclusions; //exclusion markers added by the user

    bool marking; //when a user is marking a face it'll be equal to true

    QString imageFilePath;
    QPixmap currentImage;
    float widthRatio; //relation between what the image is and what is displayed
    float heightRatio; //relation between what the image is and what is displayed
};

#endif // IMAGEMARKER_H
