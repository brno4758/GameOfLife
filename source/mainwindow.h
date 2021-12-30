#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "grid.h"
#include "hist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //Simualtes 1 step of the game
    void on_stepButton_clicked();
    //Simulates steps of the game at speed of slider value
    void on_playButton_clicked();
    //Stops simulation
    void on_pauseButton_clicked();

    void on_cell_life();

    void on_cell_death();

    void on_timer_fired();
    //Changes the speed value of simulation
    //Simulates the pause and play button being pressed again
    void on_speedSlider_valueChanged(int value);

    void on_resetButton_clicked();

    void on_emptyButton_clicked();

private:
    Ui::MainWindow *ui = nullptr;
    QGraphicsScene *scene = nullptr;
    QGraphicsScene *scene2 = nullptr;
    Grid* currGrid_ = nullptr;
    QTimer* timer_ = nullptr;
    Histogram* currHist_ = nullptr;
    int turn_ = 0;
    short speed_ = 0;
    bool running_ = false;
};
#endif // MAINWINDOW_H
