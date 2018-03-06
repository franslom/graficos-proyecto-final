#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "manager.h"
#include "humidity.h"
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText("10");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QColor getColor;
    getColor = QColorDialog::getColor(Qt::white, this);
    ui->pushButton->setStyleSheet("background-color: " + getColor.name() + ";");
    manager->set_color(getColor);

}

void MainWindow::on_comboBox_activated(int index)
{
    switch (index) {
    case 1:
        manager->set_brushtype(Simple);
        break;
    case 2:
        manager->set_brushtype(WetOnDry);
        break;
    case 3:
        manager->set_brushtype(Cruncy);
        break;
    case 4:
        manager->set_brushtype(WetOnWet);
        break;
    case 5:
        manager->set_brushtype(Blobby);
        break;
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    int tmp=ui->lineEdit->text().toInt();
    manager->set_brushwidth(tmp);
}
