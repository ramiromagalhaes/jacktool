#include "markertool.h"

MarkerTool::MarkerTool(std::vector<Rectangle> * const restrictions__) : restrictions(restrictions__)
{
    useMarkTool();
}

void MarkerTool::useMarkTool()
{
    marking = true;
}

void MarkerTool::useUnmarkTool()
{
    marking = false;
}

void MarkerTool::handleClick(const int x, const int y)
{
    if (marking) {
        //marca
        return;
    }

    //deleta

    for(std::vector<Rectangle>::iterator it = restrictions->begin(); it != restrictions->end(); ++it) {
        Rectangle r = *it;
        if (r.x <= x && x <= r.x + r.width &&
                r.y <= y && y <= r.y + r.width) {
            selected = &(*it);
            break;
        }
    }

    selected = 0;
}
