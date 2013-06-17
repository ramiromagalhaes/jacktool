#include "imagemarker.h"
#include <cmath>

ImageMarker::ImageMarker(QWidget *parent) :
    QWidget(parent)
{
}

ImageMarker::MarkerTool(std::vector<Rectangle> * const restrictions__) : restrictions(restrictions__)
{
    useMarkTool();
}

void ImageMarker::useMarkTool()
{
    isMarkerSelected = true;
}

void ImageMarker::useUnmarkTool()
{
    isMarkerSelected = false;
}

void ImageMarker::handleClick(const int x, const int y)
{
    if (isMarkerSelected) {
        if (marking) {
            endRectangle(x, y);
            return;
        }

        if (!selectRectangle(x, y)) {
            beginRectangle(x, y);
            return;
        }
    } else {
        deleteRectangle(x, y);
    }

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
    newRect.height = clickX - x;
    newRect.width = clickY - y;

    restrictions->push_back(newRect);

    marking = false;
}

void ImageMarker::cancelRectangle()
{
    marking = false;
    selected = 0;
}

bool ImageMarker::selectRectangle(const int x, const int y)
{
    for(std::vector<Rectangle>::iterator it = restrictions->begin(); it != restrictions->end(); ++it) {
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
    for(std::vector<Rectangle>::iterator it = restrictions->begin(); it != restrictions->end();) {
        Rectangle r = *it;
        if (r.contains(x, y)) {
            restrictions->erase(it);
            break;
        } else {
            ++it;
        }
    }
}
