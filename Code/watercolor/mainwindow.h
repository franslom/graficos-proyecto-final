#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <manager.h>
#include "humidity.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Manager * manager;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_activated(int index);


    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
