#ifndef GRID_H
#define GRID_H

#include <QColor>
#include <QGraphicsItem>


const QColor aliveColor = QColor(255,0,0);
const QColor deadColor = QColor(255,255,255);
class Cell: public QObject, public QGraphicsItem{
    Q_OBJECT
public:
    Cell();

    //create a new cell with the provided x, y, and life value
    Cell(const short x, const short y, bool life);

    //get the x_ value of the cell
    short get_x() const { return x_; }

    //get the y_ value of the cell
    short get_y() const { return y_; }

    //get the width of the cell
    static short get_width() { return width_; }

    //get the state of the cell
    bool get_state() const {return state_;}

    //get the next state of the cell
    bool get_next_state() const {return next_state_;}

    //set the x_ field of the cell
    void set_x(short x) {x_ = x;}

    //set the y_ field of the cell
    void set_y(short y) {y_ = y;}

    //set the state_ field of the Cell
    void set_state(bool state) {state_ = state;}

    //set the next_state_ field of the Cell
    void set_next_state(bool state) {next_state_ = state;};

    //set the cell's next state to the opposite of its current state
    //emit cellLife if cell is now alive
    //emit cellDeath if cell is now dead
    void flip();

    //apply the changes made to the cells next state if any
    void apply();

    QRectF boundingRect() const override; //adds clickable area to the object of the ui
    QPainterPath shape() const override; //allows us to draw standard shapes
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override; //allows us to add color to object we are drawing

private:
    const static short width_ = 25;
    short x_;
    short y_;
    bool state_;
    bool next_state_;
    QColor color_;

signals:
    void cellSelected(Cell *c); //singal when cell is clicked
    void cellLife(); //signal when cell comes to life
    void cellDeath(); //signal when cell dies

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

};

/* Cell End */
/* Grid Begin */

class Grid
{
public:
    Grid(short offset_x, short offset_y);
    ~Grid();

    //returns the cell at grid_[y][x]
    Cell* get_cell(const int x, const int y) const;

    //returns the number of alive cells in the grid
    short get_num_alive() const {return numAlive_;}

    //returns the number of alive cells in the previous state of the grid
    short get_prev_num_alive() const {return prevNumAlive_;}

    //increments the numAlive_ field of the grid
    void incrNumAlive() {numAlive_++;}

    //decrements the numAlive_ field of the grid
    void decrNumAlive() {numAlive_--;}

    //Applys 1 step of life algorithm to grid
    void step();

    //evaluate the call at grid[y][x] using 'Conway's Game of Life' logic
    void evalCell(short x, short y, Cell *c);

    //Sets each cells state_ to its next_state_ in grid
    //Calls each Cell's update method
    void applyNewState();

    //Sets all Cell's state_ in Grid to false
    //Sets all Cell's next_state_ in Grid to false
    void flushGrid();
private:
    const short numRows_ = 10;
    const short numCols_ = 20;
    short numAlive_ = 0;
    short prevNumAlive_ = 0;
    Cell* grid_[10][20]; //ask about the state of these arrays in a dynamic object. Do the cells also need to by dynamic? I.e., what if it was an array of Cell and not Cell*
};
#endif // GRID_H
