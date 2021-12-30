#include "grid.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>

Cell::Cell()
{
    state_ = false;
    next_state_ = false;
    color_ = QColor(255,255,255);
}

Cell::Cell(const short x, const short y, bool life) : x_(x), y_(y), state_(life), next_state_(life){
    color_ = life ? aliveColor : deadColor;
}
void Cell::flip()
{
    next_state_ = !state_; //set the cells next state
    if(next_state_)
    {
        emit cellLife();
    }
    else
    {
        emit cellDeath();
    }
}

void Cell::apply()
{
    state_ = next_state_;
    color_ = state_ ? aliveColor : deadColor;
}
QRectF Cell::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}
QPainterPath Cell::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}
void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));

    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !state_) //left click on a dead cell
    {
        flip();
        apply();
        update();
    }
    else if(event->button() == Qt::RightButton && state_) //right click on a live cell
    {
        flip();
        apply();
        update();
    }
}


Grid::Grid(short offset_x, short offset_y)
{
    bool roll;
    for(short i = 0; i < numRows_; i++)
    {
        for(short j = 0; j < numCols_; j++)
        {
            roll = rand() % 2;
            grid_[i][j] = new Cell(j*Cell::get_width()+offset_x,i*Cell::get_width()+offset_y,roll); //assign each Cell's x and y, taking into consideration their width and the offset that we want to use to center the Cells in the center of the QGraphicsScene
            numAlive_ = roll ? numAlive_+1 : numAlive_;
        }
    }
    prevNumAlive_ = numAlive_;
}
Grid::~Grid()
{
    for(short i = 0; i < numRows_; i++)
    {
        for(short j = 0; j < numCols_; j++)
        {
            delete grid_[i][j];
        }
    }
}
void Grid::step()
{
    //Run evalCell on all cells
    prevNumAlive_ = numAlive_;

    for(short i = 0; i < numRows_; i++)
    {
        for(short j = 0; j < numCols_; j++)
        {
            evalCell(j, i, grid_[i][j]);
        }
    }
    applyNewState();
}
void Grid::applyNewState()
{
    for(short i = 0; i < numRows_; i++)
    {
        for(short j = 0; j < numCols_; j++)
        {
            grid_[i][j]->apply();
            grid_[i][j]->update();
        }
    }
}
void Grid::evalCell(short x, short y, Cell *c)
{
    //go row by row for the surrounding square of cells
    //get a count of all live neighbors
    //follow write-up logic for result of cell

    //[+] [+] [+]
    //[+] [x] [+]
    //[+] [+] [+]
    //traverse all + cells surrounding the x cell

    short starty = (y-1) < 0 ? numRows_-1 : y-1; //top left cell y
    short startx = (x-1) < 0 ? numCols_-1 : x-1; //top left cell x
    short acc = 0;
    for(short i = 0; i < 3; i++) //count all cells in 3x3 range
    {
        short currx = startx;
        for(short j = 0; j < 3; j++)
        {
            acc += grid_[starty][currx]->get_state();
            currx = (currx+1) % numCols_;
        }
        starty = (starty+1) % numRows_;
    }
    acc -= grid_[y][x]->get_state(); //uncount the original cell

    if(grid_[y][x]->get_state()) //if alive
    {
        if(acc < 2 || acc > 3) //if #neighbors is in dying range
        {
            c->flip(); //die
        }
    }
    else if(acc == 3) //else if dead and 3 neighbors
    {
        c->flip(); //live
    }
}
Cell* Grid::get_cell(int x, int y) const
{
    if(x >= numCols_ || y >= numRows_)
    {
        std::cout << "Attempted to get out of bends cell. Returning null.\n";
        return nullptr;
    }
    return grid_[y][x];
}

void Grid::flushGrid()
{
    for(short i = 0; i < numRows_; i++)
    {
        for(short j = 0; j < numCols_; j++)
        {
            grid_[i][j]->set_state(false);
            grid_[i][j]->set_next_state(false);
        }
    }

    numAlive_ = 0;
}
