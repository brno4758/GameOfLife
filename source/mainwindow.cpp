#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTimer>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsView *gridView = ui->graphicsView;
    QGraphicsView *histView = ui->histGraphicsView;
    scene = new QGraphicsScene;
    scene2 = new QGraphicsScene;
    timer_ = new QTimer(this);

    gridView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gridView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gridView->setScene(scene);
    gridView->setSceneRect(0,0,gridView->frameSize().width(),gridView->frameSize().height());

    histView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    histView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    histView->setScene(scene2);
    histView->setSceneRect(0,0,histView->frameSize().width(),histView->frameSize().height());

    //Q:Why do I need to decalre this dynamically? When does it go out of scope?
    //A:When the constructor is finished, the grid goes out of scope
    srand(static_cast<unsigned>(QTime::currentTime().msec()));
    int xoffset = (gridView->frameSize().width() - Cell::get_width()*20)/2;
    int yoffset = (gridView->frameSize().height() - Cell::get_width()*10)/2;
    currGrid_ = new Grid(xoffset, yoffset);
    currHist_ = new Histogram(histView->frameSize().width());
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            scene->addItem(currGrid_->get_cell(j,i));
            connect(currGrid_->get_cell(j,i), &Cell::cellLife, this, &MainWindow::on_cell_life);
            connect(currGrid_->get_cell(j,i), &Cell::cellDeath, this, &MainWindow::on_cell_death);
        }
    }

    connect(timer_, &QTimer::timeout, this, &MainWindow::on_timer_fired);

    ui->turnLabel->setText("Turn: 0");
    std::string s = "Population: " + std::to_string(currGrid_->get_num_alive());
    ui->popLabel->setText(s.c_str());
}

MainWindow::~MainWindow()
{
    scene->clear();
    scene2->clear();
    delete ui;
    delete timer_;
}

void MainWindow::on_stepButton_clicked()
{
    currGrid_->step();
    turn_++;
    std::string str = "Turn: " + std::to_string(turn_);
    ui->turnLabel->setText(str.c_str());

    //The bug here is as follows:
    //When I go into the insert function and insert the last bar into the histograms array...
     //... I come out of that fxn with size_ == capacity_, so is_full evaluates to true and we dont...
     //... add the last bar to the scene
     QColor c = currGrid_->get_prev_num_alive() >= currGrid_->get_num_alive() ? QColor(255,0,0) : QColor(0,255,0);
     currHist_->insert(currHist_->get_size()*Bar::get_width(), ui->histGraphicsView->height() - currGrid_->get_num_alive(), ui->histGraphicsView->height(), c); //using full height as length because was running into occasional "gap" between bottom of bar and bottom of scene
     if(currHist_->is_full()){ //if histogram is full
         scene2->update(); //update all the bar that were just shifted;
     }
     else{
         scene2->addItem(currHist_->get_bar(currHist_->get_size()-1)); //add a new bar to the scene
     }

     //how do I just remove and redraw everything onto the scene without deleting theitems fully
     //because scene->clear() does a full delete of the objects
}
void MainWindow::on_playButton_clicked()
{
    //This doesnt feel right when sliding, come back to this********
    short relativeSpeed = 5000 - speed_*100/2;
    relativeSpeed = relativeSpeed >= 0 ? relativeSpeed : 0;
    timer_->start(relativeSpeed);
    running_ = true;
}

void MainWindow::on_pauseButton_clicked()
{
    timer_->stop();
    running_ = false;
}

void MainWindow::on_cell_life()
{
    currGrid_->incrNumAlive();
    std::string str = "Population: " + std::to_string(currGrid_->get_num_alive());
    ui->popLabel->setText(QString(str.c_str()));
}

void MainWindow::on_cell_death()
{
    currGrid_->decrNumAlive();
    std::string str = "Population: " + std::to_string(currGrid_->get_num_alive());
    ui->popLabel->setText(QString(str.c_str()));
}

void MainWindow::on_timer_fired()
{
    on_stepButton_clicked();
}

void MainWindow::on_speedSlider_valueChanged(int value)
{
    std::string as_str = "Speed: " + std::to_string(value);
    ui->speedLabel->setText(QString(as_str.c_str()));
    //set speed to current bar value
    speed_ = value;
    //if its running, stop and start it to update with new speed
    if(running_)
    {
        on_pauseButton_clicked();
        on_playButton_clicked();
    }
}


void MainWindow::on_resetButton_clicked()
{
    scene->clear();
    scene2->clear();
    currGrid_ = new Grid((ui->graphicsView->frameSize().width() - Cell::get_width()*20)/2, (ui->graphicsView->frameSize().height() - Cell::get_width()*10)/2);
    currHist_ = new Histogram(ui->histGraphicsView->frameSize().width());
    turn_ = 0;
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            scene->addItem(currGrid_->get_cell(j,i));
            connect(currGrid_->get_cell(j,i), &Cell::cellLife, this, &MainWindow::on_cell_life);
            connect(currGrid_->get_cell(j,i), &Cell::cellDeath, this, &MainWindow::on_cell_death);
        }
    }
    ui->turnLabel->setText("Turn: 0");
    std::string s = "Population: " + std::to_string(currGrid_->get_num_alive());
    ui->popLabel->setText(s.c_str());
}

void MainWindow::on_emptyButton_clicked()
{
    currGrid_->flushGrid();
    ui->popLabel->setText("Population: 0");
    currGrid_->applyNewState();
}

