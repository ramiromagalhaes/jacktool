#include "imagemarker.h"
#include <iostream>

#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>



ImageMarker::ImageMarker(QWidget *parent) :
    QLabel(parent)
{
    //TODO check page 109-110

    setAttribute(Qt::WA_StaticContents);

    //the pen we'll use while writing the ruber bands and boxes
    exclusionsPen.setWidth(1);
    exclusionsPen.setColor(Qt::magenta);

    rubberbandPen.setWidth(1);
    rubberbandPen.setColor(Qt::yellow);
}

void ImageMarker::setImageFromAbsolutePath(QString &path)
{
    imageFilePath = path;

    QImage theImage = QImage(path);
    currentImage = QPixmap::fromImage(theImage);
    setPixmap(currentImage.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    exclusions.clear();
    updateBufferDisplayRatio();
}

std::vector<Rectangle> * ImageMarker::getExclusions()
{
    std::vector<Rectangle> * returnMe = new std::vector<Rectangle>(exclusions);
    return returnMe;
}

void ImageMarker::setExclusions(const std::vector<Rectangle> &exclusions_)
{
    exclusions.clear();
    for (std::vector<Rectangle>::const_iterator it = exclusions_.begin(); it != exclusions_.end(); ++it)
    {
        exclusions.push_back(*it);
    }
}

void ImageMarker::mousePressEvent(QMouseEvent *evt)
{
    if (!isReady()) { return; }

    if (evt->button() == Qt::LeftButton)
    {
        handleLeftClick(evt);
    }
}

void ImageMarker::mouseMoveEvent(QMouseEvent *evt)
{
    if (!isReady()) { return; }

    if (marking) {
        updateRubberBandRegion();
        rubberband.setBottomRight(evt->pos());
        updateRubberBandRegion();
    }
}

void ImageMarker::mouseReleaseEvent(QMouseEvent *evt)
{
    if (!isReady()) { return; }

    if(evt->button() == Qt::RightButton)
    {
        handleRightRelease(evt);
    }
    else if (evt->button() == Qt::LeftButton)
    {
        handleLeftRelease(evt);
    }
}

void ImageMarker::paintEvent(QPaintEvent *evt)
{
    QLabel::paintEvent(evt);

    QPainter painter(this);
    if (marking) {
        painter.setPen(rubberbandPen);
        painter.drawRect(rubberband);
    }

    painter.setPen(exclusionsPen);
    for(std::vector<Rectangle>::iterator it = exclusions.begin(); it != exclusions.end(); ++it)
    {
        painter.drawRect(
            (int)(((float)it->x)      / widthRatio),
            (int)(((float)it->y)      / heightRatio),
            (int)(((float)it->width)  / widthRatio),
            (int)(((float)it->height) / heightRatio) );
    }
}

void ImageMarker::resizeEvent(QResizeEvent *evt)
{
    QLabel::resizeEvent(evt);
    if (!currentImage.isNull())
    {
        setPixmap(currentImage.scaled(
                  size(),
                  Qt::KeepAspectRatio,
                  Qt::FastTransformation));
        updateBufferDisplayRatio();
    }
}

bool ImageMarker::isReady()
{
    if (!pixmap()) return false;
    if (pixmap()->isNull()) return false;

    return true;
}

void ImageMarker::handleLeftClick(QMouseEvent *evt)
{
    marking = true;

    rubberband.setTopLeft(evt->pos());
    rubberband.setBottomRight(evt->pos());
    updateRubberBandRegion();
}

void ImageMarker::handleRightRelease(QMouseEvent *evt)
{
    const int x = evt->x() * widthRatio;
    const int y = evt->y() * heightRatio;
    for(std::vector<Rectangle>::iterator it = exclusions.begin(); it != exclusions.end();) {
        Rectangle r = *it;
        if ( r.contains(x, y) ) {
            exclusions.erase(it);
        } else {
            ++it;
        }
    }

    update();
}

void ImageMarker::handleLeftRelease(QMouseEvent *)
{
    marking = false;

    rubberband = rubberband.normalized();
    Rectangle newRect;
    newRect.x      = rubberband.left()   * widthRatio;
    newRect.y      = rubberband.top()    * heightRatio;
    newRect.height = rubberband.height() * widthRatio;
    newRect.width  = rubberband.width()  * heightRatio;
    exclusions.push_back(newRect);

    update();
}

void ImageMarker::updateRubberBandRegion()
{
    QRect rect = rubberband.normalized();
    update(rect.left(),  rect.top() - 1,    rect.width(), 3);
    update(rect.left(),  rect.top() - 1,    3,            rect.height());
    update(rect.left(),  rect.bottom() - 1, rect.width(), 3);
    update(rect.right(), rect.top(),        3,            rect.height() - 1);
}

void ImageMarker::updateExcludedRegion(Rectangle &rect)
{
    update(rect.x,  rect.y - 1,        rect.width,     3);
    update(rect.x,  rect.y - 1,        3,              rect.height);
    update(rect.x,  rect.bottom() - 1, rect.width, 3);
    update(rect.right(), rect.y,       3,              rect.height - 1);
}

void ImageMarker::updateBufferDisplayRatio()
{
    //TODO same aspect ratio!
    heightRatio = ((float)currentImage.size().height()) / ((float)pixmap()->size().height());
    widthRatio  = ((float)currentImage.size().width())  / ((float)pixmap()->size().width());
}
