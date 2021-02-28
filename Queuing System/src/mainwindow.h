#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <memory>
#include "simulation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ApplyButton_clicked();

    void on_autoModeButton_clicked();

    void on_pushButtonForward_clicked();

    void on_pushButtonBackward_clicked();

private:
    Ui::MainWindow *ui;
    int numberSources = 3;
    int numberOperators = 2;
    size_t bufSize = 2;
    int kmin = 20;
    double lambda = 3;
    double alpha = 0;
    double beta = 1;

    int actualStep = 0;
    Simulation stepSmo;
};

#endif // MAINWINDOW_H
