#ifndef IMAGEMARKER_H
#define IMAGEMARKER_H

#include <QWidget>

class ImageMarker : public QWidget
{
    Q_OBJECT
    //Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)

public:
    explicit ImageMarker(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

signals:
    
public slots:

private:
    void beginRectangle(const int x, const int y);
    void endRectangle(const int x, const int y);
    void cancelRectangle();

    bool selectRectangle(const int x, const int y);
    void moveRectangle(const int x, const int y);

    void deleteRectangle(const int x, const int y);



    std::vector<Rectangle> restrictions;
    bool isMarkerSelected; //the marking tool is selected if == true

    //to be used when marking a certain region
    bool marking; //current state of the marking tool
    int clickX, clickY; //origin of the clicked area
    Rectangle * selected;
};

#endif // IMAGEMARKER_H
