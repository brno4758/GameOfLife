#ifndef HIST_H
#define HIST_H

#include <QColor>
#include <QGraphicsItem>
#include <vector>

class Bar: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Bar();
    Bar(short x, short y, short length, QColor c) : x_(x), y_(y), length_(length),color_(c){};
    //Return x value of Bar
    short get_x() const {return x_;}

    //Return y value of Bar
    short get_y() const {return y_;}

    //Return length value of Bar
    short get_length() const {return length_;}

    //Return color of Bar
    QColor get_color() const {return color_;}

    //Set the x value of Bar
    void set_x(short x) {x_ = x;}

    //Set the y value of Bar
    void set_y(short y) {y_ = y;}

    //Set the length value of Bar
    void set_length(short length) {length_ = length;}

    //Set the color of Bar
    void set_color(QColor c) {color_ = c;}

    //Return the width of the Bar
    static short get_width() {return width_;}
private:
    const static short width_ = 20;
    short x_;
    short y_;
    short length_;
    QColor color_;
    QRectF boundingRect() const override; //adds clickable area to the object of the ui
    QPainterPath shape() const override; //allows us to draw standard shapes
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override; //allows us to
};


class Histogram
{
public:
    Histogram(short sceneWidth) : capacity_(sceneWidth/Bar::get_width()){}; //initialize the capacity based on the scene's width
    ~Histogram();

    //Return true if Histogram is full, false else
    bool is_full() const {return size_==capacity_;}

    //Return the current number of Bars in the Histogram
    short get_size() const {return size_;}

    //Return the capacity number of Bars for the Histogram
    short get_capacity() const {return capacity_;}

    //Return the Bar in the Histogram at index i
    Bar* get_bar(short i);

    //Increment the current size of the Histogram by 1
    void incr_size() {size_++;}

    //Insert a new parameterized Bar into the Histogram
    void insert(short x, short y, short length, QColor c);
private:
    short const capacity_;
    std::vector<Bar*> bars_;
    short size_ = 0;
};

#endif // HIST_H
