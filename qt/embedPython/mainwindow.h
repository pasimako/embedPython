#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pyrun.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(PyRun *pyRun, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    PyRun *pyRun;
};

#endif // MAINWINDOW_H
