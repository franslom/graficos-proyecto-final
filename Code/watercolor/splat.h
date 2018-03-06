#ifndef SPLAT_H
#define SPLAT_H
#include "humidity.h"

class Splat
{
private:
    const float alpha = 0.33f;
    QPolygonF vertices;
    QList<QPointF> velocities;
    int life;
    qreal roughness;
    qreal flow;
    QPointF motionBias;
    QTime startTime;
    qreal initSize;
    QColor Color;
    int fix;
    QPointF center;

public:
    enum SplatState {
        Flowing,
        Fixed,
        Dried
    };

    Splat(QPointF offset, float width, QColor splatColor);
    Splat(QPointF offset, QPointF velocityBias, int width, int life, qreal roughness, qreal flow, qreal radialSpeed, QColor splatColor);
    Splat(const Splat &obj);
    Splat &operator =(const Splat &);
    int Update(Humidity *);
    int ReHumidity(Water *, Humidity *, QPointF &);
    double f_random(qreal , qreal );
    QPointF get_center(){return center;}
    QPolygonF get_vertices(){return vertices;}
    QColor getColor();
    QTime get_startTime();
    qreal CalcSize();

};

#endif // SPLAT_H
