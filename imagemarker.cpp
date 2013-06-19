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
}

void ImageMarker::mousePressEvent(QMouseEvent *evt)
{
    if (!isReady()) { return; }

    if(evt->button() == Qt::RightButton)
    {
        handleRightClick(evt);
    }
    else if (evt->button() == Qt::LeftButton)
    {
        handleLeftClick(evt);
    }
}

void ImageMarker::mouseMoveEvent(QMouseEvent *)
{
    if (!isReady()) { return; }

    if (marking) {
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
    for(std::vector<Rectangle>::iterator it = restrictions.begin(); it != restrictions.end(); ++it)
    {
        Rectangle r = *it;
        painter.setPen(Qt::magenta);
        painter.drawRect(r.x, r.y, r.width, r.height);
    }
}

void ImageMarker::resizeEvent(QResizeEvent *evt)
{
    QLabel::resizeEvent(evt);//TODO I'm just testing!
}

bool ImageMarker::isReady()
{
    if (!pixmap()) return false;
    if (pixmap()->isNull()) return false;

    return true;
}

void ImageMarker::handleRightClick(QMouseEvent *)
{
    //does nothing
}

void ImageMarker::handleLeftClick(QMouseEvent *evt)
{
    marking = true;

    clickX = evt->x();
    clickY = evt->y();
}

void ImageMarker::handleRightRelease(QMouseEvent *evt)
{
    for(std::vector<Rectangle>::iterator it = restrictions.begin(); it != restrictions.end();) {
        Rectangle r = *it;
        if ( r.contains(evt->x(), evt->y()) ) {
            restrictions.erase(it);
        } else {
            ++it;
        }
    }
}

void ImageMarker::handleLeftRelease(QMouseEvent *evt)
{
    marking = false;

    Rectangle newRect;
    newRect.x = clickX;
    newRect.y = clickY;
    newRect.height = evt->x() - clickX;
    newRect.width  = evt->y() - clickY;

    restrictions.push_back(newRect);
    std::cout << "End rectangle: " << newRect.x << " " << newRect.y << " " << newRect.height << " " << newRect.width << std::endl;
}

