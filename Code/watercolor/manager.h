#ifndef MANAGER_H
#define MANAGER_H
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>
#include <iostream>
#include <QTimer>
#include <QPixmap>
#include <QMutex>
#include <QTime>

#include "splat.h"
#include "openglwindow.h"
#include "brush_type.h"
#include "humidity.h"

using namespace std;

class brush_type;

enum types {
    Simple,
    WetOnDry,
    Cruncy,
    WetOnWet,
    Blobby
};

class Manager: public OpenGLWindow
{
private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    QOpenGLShaderProgram *m_program;


    QList<Splat*> *m_active;
    QList<Splat*> *m_fixed;
    QList<Splat*> *m_dried;
    QColor current_color;
    float brush_width;
    QTimer *timer;
    QTime m_startTime;
    brush_type *generator;

    int window_w;
    int window_h;
    QPointF last_pos;
    Humidity *humidity;

public:
    Manager();

    void set_wh(int w, int h);

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void add_splat(Splat);
    void pushBackSplat(Splat *splatToPush);
    void set_brushtype(int id);

    void fillWetMap(Water *placeForWetting, QPointF pos);
    void RewetSplats(Water *m_water, QPointF pos);


    QColor get_current_color();
    void set_Humidity(Humidity &);
    void set_color(QColor color);
    void set_brushwidth(int);

    float get_brush_width();
    void draw_vector_splat(QList<Splat*> *vector);
};

#endif // MANAGER_H
