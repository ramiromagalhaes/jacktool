#include "imagemarker.h"
#include <cmath>
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
        rubberband.setBottomLeft(evt->pos());
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
        painter.drawRect(it->x, it->y, it->width, it->height);
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
}

void ImageMarker::handleRightRelease(QMouseEvent *evt)
{
    for(std::vector<Rectangle>::iterator it = exclusions.begin(); it != exclusions.end();) {
        Rectangle r = *it;
        if ( r.contains(evt->x(), evt->y()) ) {
            exclusions.erase(it);
            updateExcludedRegion(r);
        } else {
            ++it;
        }
    }
}

void ImageMarker::handleLeftRelease(QMouseEvent *)
{
    marking = false;
    updateRubberBandRegion();

    rubberband = rubberband.normalized();

    Rectangle newRect;
    newRect.x = rubberband.left();
    newRect.y = rubberband.top();
    newRect.height = rubberband.height();
    newRect.width  = rubberband.width();

    exclusions.push_back(newRect);
}

void ImageMarker::updateRubberBandRegion()
{
    QRect rect = rubberband.normalized();
    update(rect.left(),  rect.top() - 1,    rect.width(), 3);
    update(rect.left(),  rect.top() - 1,    3,            rect.height());
    update(rect.left(),  rect.bottom() - 1, rect.width(), 3);
    update(rect.right(), rect.top(),     3,               rect.height() - 1);
}

void ImageMarker::updateBufferDisplayRatio()
{
    sizeRatio = (float)pixmap()->size().height() /
            (float)currentImage.size().height();
}

void ImageMarker::updateExcludedRegion(Rectangle &rect)
{
    update(rect.x,  rect.y - 1,    rect.width,     3);
    update(rect.x,  rect.y - 1,    3,              rect.height);
    update(rect.x,  rect.bottom() - 1, rect.width, 3);
    update(rect.right(), rect.y,     3,            rect.height - 1);
}

