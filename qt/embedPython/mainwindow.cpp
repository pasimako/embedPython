#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(PyRun *pyRun, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->pyRun = pyRun;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->plainTextEdit_2->setPlainText(pyRun->cssmin(ui->plainTextEdit->toPlainText()));
}
