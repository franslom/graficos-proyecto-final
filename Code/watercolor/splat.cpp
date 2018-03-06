#include "splat.h"




Splat::Splat(QPointF offset, float width, QColor splatColor)
    : life(30), roughness(1.f), flow(1.f),
      motionBias(QPointF(0.f, 0.f)), Color(splatColor)
{
    center=offset;
    fix = 225;
    Color.setAlpha(50);

    float r = width / 2.0;
    int n = 128;

    qreal dt = 2.f * M_PI / n;
    QPointF p;

    for (int i = 0; i < n; i++)
    {
        p.setX(cos(i * dt));
        p.setY(sin(i * dt));
        vertices.push_back(QPointF(static_cast <qreal> (r * p.x()) + offset.x(),
                                     static_cast <qreal> (r * p.y()) + offset.y()));
        velocities.push_back(QPointF(2.f * p.x(),
                                       2.f * p.y()));
    }
    initSize = CalcSize();
    startTime = QTime::currentTime();
}

Splat::Splat(QPointF offset, QPointF velocityBias, int width, int life, qreal roughness, qreal flow, qreal radialSpeed, QColor splatColor)
    : life(life), roughness(roughness), flow(flow),
      motionBias(velocityBias), Color(splatColor)
{
    center=offset;
    fix = 225;
    Color.setAlpha(50);
    int r = width / 2;
    int n = 128;

    qreal dt = 2.f * M_PI / n;
    QPointF p;
    for (int i = 0; i < n; i++)
    {
        p.setX(cos(i * dt));
        p.setY(sin(i * dt));
        vertices.push_back(QPointF(static_cast <qreal> (r * p.x()) + offset.x(),
                                     static_cast <qreal> (r * p.y()) + offset.y()));
        velocities.push_back(QPointF(radialSpeed * p.x(),
                                       radialSpeed * p.y()));
    }
    initSize = CalcSize();
    startTime = QTime::currentTime();
}

Splat::Splat(const Splat &obj)
{
    vertices = obj.vertices;
    velocities = obj.velocities;
    life = obj.life;
    fix = obj.fix;
    roughness = obj.roughness;
    flow = obj.flow;
    motionBias = obj.motionBias;

    startTime = obj.startTime;

    initSize = obj.initSize;
    Color = obj.Color;
}

Splat &Splat::operator =(const Splat &obj)
{
    Splat ret(QPoint(0,0),0, QColor(Qt::red));
    ret.vertices = obj.vertices;
    ret.velocities = obj.velocities;
    ret.life = obj.life;
    ret.fix = obj.fix;
    ret.roughness = obj.roughness;
    ret.flow = obj.flow;
    ret.motionBias = obj.motionBias;

    ret.startTime = obj.startTime;

    ret.initSize = obj.initSize;
    ret.Color = obj.Color;

    return ret;
}

int Splat::Update(Humidity *humi)

{
    if (life <= 0) {
        if (fix <= 0) {
            return Splat::Dried;
        } else {
            fix--;
            return Splat::Fixed;
        }
    }

    life--;


    //#pragma omp parallel
    {
        //#pragma omp for
        for (int i = 0; i < vertices.length(); i++) {
            QPointF x = vertices[i];
            QPointF v = velocities[i];
            QPointF d = (1.f - alpha) * motionBias + alpha / f_random(1.f, 1.f + roughness) * v;

            QPointF x1 = x + flow * d; + QPointF(f_random(-roughness, roughness),
                                                   f_random(-roughness, roughness));
            /*uchar hu = humi->GetHumidity((int)x1.x(), (int)x1.y());

            if ((int)hu > 0)*/
               vertices[i] = x1;
        }
    }

    if (!life) {
        for (int i = 0; i < vertices.length(); i++) {
            velocities[i] = QPointF(0, 0);
        }
    }

    return Splat::Flowing;
}




QTime Splat::get_startTime()
{
    return startTime;
}


int Splat::ReHumidity(Water *Place, Humidity *humi, QPointF &pos)
{
    int howMuch = 0;
    int i;
        #pragma omp parallel for
        for (i = 0; i < vertices.length(); i++) {
            if (Place->contains((vertices[i] - pos).toPoint())) {
                velocities[i] = humi->GetVelocity(vertices[i].toPoint().x(),
                                                      vertices[i].toPoint().y());
                howMuch++;
            }
        }

    if (howMuch) {
        life = 30;
        fix = 225;
        return Splat::Flowing;
    } else
        return Splat::Fixed;
}


QColor Splat::getColor() {
    QColor current;
    current = Color;
    qreal multiply = initSize / CalcSize();
    if (multiply < 0.f || multiply > 1.f)
        multiply = 1;

    current.setAlpha(current.alpha() * multiply);
    return current;
}

double Splat::f_random(qreal min, qreal max)
{
    return (qreal)rand() / RAND_MAX*(max - min) + min;
}

qreal Splat::CalcSize()
{
    if (vertices.length() < 3)
        return 0.f;

    QPointF v0 = vertices[0];
    qreal v0x = v0.x();
    qreal v0y = v0.y();
    QPointF e0 = vertices[1] - v0;
    qreal e0x = e0.x();
    qreal e0y = e0.y();
    qreal s = 0.f;
    int length = vertices.length();

    for (int i = 2; i < length; i++) {
        QPointF v2 = vertices[i];
        qreal e1x = v2.x() - v0x;
        qreal e1y = v2.y() - v0y;
        s += e0x * e1y - e0y * e1x;
        e0x = e1x;
        e0y = e1y;
    }

    return s >= 0 ? s : -s;
}
