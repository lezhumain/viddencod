#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(
        ui->pushButton, SIGNAL(clicked(bool)),
        this,           SLOT(TestSlot(bool))
        );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TestSlot(bool var)
{
    QString folder = QFileDialog::getExistingDirectory();

    qWarning() << folder;
}
