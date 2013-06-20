#ifndef EXCLUSION_H
#define EXCLUSION_H

class Rectangle {
public:
    int x, y, width, height;

    bool contains(int x, int y) {
        return this->x <= x && x <= this->x + width &&
                this->y <= y && y <= this->y + height;
    }

    int bottom() {
        return y + height;
    }

    int right() {
        return x + width;
    }
};

#endif // EXCLUSION_H
