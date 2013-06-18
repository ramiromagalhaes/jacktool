#include "imagemarker.h"
#include <cmath>
#include <QPainter>
#include <iostream>

ImageMarker::ImageMarker(QWidget *parent) :
    QLabel(parent)
{
    //TODO check page 109-110

    setAttribute(Qt::WA_StaticContents);

    useMarkTool();
}

void ImageMarker::mousePressEvent(QMouseEvent *evt)
{
    QLabel::mousePressEvent(evt);

    if (!pixmap() || pixmap()->isNull()) {
        return;
    }

    if(evt->button() == Qt::RightButton)
    {
        cancelRectangle();
        return;
    }

    //assume LeftClick

    if (isMarkerSelected) {
        if (marking) {
            endRectangle(evt->x(), evt->y());
            return;
        }

        //if (!selectRectangle(evt->x(), evt->y())) {
            beginRectangle(evt->x(), evt->y());
            return;
        //}
    } else {
        //deleteRectangle(x, y);
    }
}

void ImageMarker::mouseMoveEvent(QMouseEvent *evt)
{
    QLabel::mouseMoveEvent(evt);

    /*
    if (pixmap()->isNull()) {
        return;
    }

    if (evt->button() == Qt::LeftButton && selected) {
        moveRectangle(evt->x(), evt->y());
    }
    */
}

void ImageMarker::paintEvent(QPaintEvent *evt)
{
    QLabel::paintEvent(evt);

    std::cout << "Painting...";

    QPainter painter(this);
    for(std::vector<Rectangle>::iterator it = restrictions.begin(); it != restrictions.end();)
    {
        Rectangle r = *it;
        painter.setPen(Qt::magenta);
        //painter.drawRect(r.x, r.y, r.width, r.height);
        painter.fillRect(r.x, r.y, r.width, r.height, Qt::magenta);
    }

}

void ImageMarker::resizeEvent(QResizeEvent *evt)
{
    QLabel::resizeEvent(evt);//TODO I'm just testing!
}

void ImageMarker::useMarkTool()
{
    isMarkerSelected = true;
}

void ImageMarker::useUnmarkTool()
{
    isMarkerSelected = false;
}

void ImageMarker::beginRectangle(const int x, const int y)
{
    marking = true;

    clickX = x;
    clickY = y;

    selected = 0;
}

void ImageMarker::endRectangle(const int x, const int y)
{
    Rectangle newRect;
    newRect.x = clickX;
    newRect.y = clickY;
    newRect.height = x - clickX;
    newRect.width = y - clickY;

    restrictions.push_back(newRect);

    marking = false;

    std::cout << "End rectangle: " << newRect.x << " " << newRect.y << " " << newRect.height << " " << newRect.width << std::endl;
}

void ImageMarker::cancelRectangle()
{
    marking = false;
    selected = 0;
}

bool ImageMarker::selectRectangle(const int x, const int y)
{
    for(std::vector<Rectangle>::iterator it = restrictions.begin(); it != restrictions.end(); ++it) {
        Rectangle r = *it;
        if (r.contains(x, y)) {
            selected = &(*it);

            clickX = x - r.x;
            clickY = y - r.y;

            return true;
        }
    }

    return false;
}

void ImageMarker::moveRectangle(const int x, const int y)
{
    selected->x = x - clickX;
    selected->y = y - clickY;
}

void ImageMarker::deleteRectangle(const int x, const int y) {
    for(std::vector<Rectangle>::iterator it = restrictions.begin(); it != restrictions.end();) {
        Rectangle r = *it;
        if (r.contains(x, y)) {
            restrictions.erase(it);
            break;
        } else {
            ++it;
        }
    }
}
