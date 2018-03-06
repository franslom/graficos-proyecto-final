#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    int window_w=1200;
    int window_h=900;

    QSurfaceFormat format;
    format.setSamples(16);

    Humidity humi(window_w - 2, window_h - 2);

    w.manager=new Manager();
    w.manager->set_Humidity(humi);
    w.manager->set_wh(window_w, window_h);
    w.manager->setFormat(format);
    w.manager->resize(window_w, window_h);
    w.manager->show();
    w.manager->setAnimating(true);


    return a.exec();
}
