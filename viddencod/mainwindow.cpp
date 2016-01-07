#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(
        ui->pushButton, SIGNAL(clicked(bool)),
        this,           SLOT(TestSlot())
        );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TestSlot()
{
    qWarning() << "Done yaknow";
}
