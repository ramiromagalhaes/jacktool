#ifndef MARKERTOOL_H
#define MARKERTOOL_H

#include "Rectangle.h"
#include <vector>

class MarkerTool
{
public:
    MarkerTool(std::vector<Rectangle> * const restrictions__);

    void useMarkTool();
    void useUnmarkTool();

    void handleClick(const int x, const int y);

private:
    bool marking; //the marking tool is selected if == true
    std::vector<Rectangle> * const restrictions;
    Rectangle * selected;

};

#endif // MARKERTOOL_H
