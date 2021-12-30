#include "hist.h"
#include <QtWidgets>

Bar::Bar(){}

QPainterPath Bar::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}
void Bar::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(QColor(color_)));

    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->length_));
    painter->setBrush(b);
}

QRectF Bar::boundingRect() const
{
    return QRectF(x_, y_, width_, length_);
}


Histogram::~Histogram()
{
    for(short i = 0; i < capacity_; i++)
    {
        delete bars_[i];
    }
}

Bar*  Histogram::get_bar(short i)
{
    if(i >= capacity_)
    {
        qDebug() << "Out of bounds in get_bar";
        return nullptr;
    }
    return bars_[i];
}

void Histogram::insert(short x, short y, short length,QColor c)
{
    if(size_ < capacity_)
    {
        bars_.push_back(new Bar(x,y,length,c));
        size_++;
        return;
    }
    //start at beginning of bars list
    //shift all bars to left by overwriting
    for(short i = 0; i < capacity_-1; i++)
    {
        bars_[i]->set_y(bars_[i+1]->get_y());
        bars_[i]->set_length(bars_[i+1]->get_length());
        bars_[i]->set_color(bars_[i+1]->get_color());
    }
    //put in the new bar at the end of the vector
    bars_[capacity_-1]->set_x(x);
    bars_[capacity_-1]->set_y(y);
    bars_[capacity_-1]->set_length(length);
    bars_[capacity_-1]->set_color(c);
}
