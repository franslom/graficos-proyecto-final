#include "brush_type.h"


void brush_type::init_brush(int width)
{
    int r = width / 2;

    if (this->water)
        delete water;

    this->water = new Water();
    for (int y = -r; y <= r; y++)
        for (int x = -r; x <= r; x++) {
            int sqr = x*x + y*y;
            if (sqr <= r*r)
                this->water->push_back(QPoint(x, y));
        }
}

void simple_brush::Generate(Manager * manager, QMouseEvent *event)
{
    QColor paintColor = manager->get_current_color();
    float width = manager->get_brush_width();
    if (event->buttons() == Qt::LeftButton) {
        Splat *newsplat = new Splat(event->pos(), width, paintColor);
        manager->pushBackSplat(newsplat);
        cout<<"new slat at: "<<event->x()<<" "<<event->y()<<endl;
        this->init_brush(width);
        manager->fillWetMap(water, event->pos());
        manager->RewetSplats(water, event->pos());
    }
}

void wetondry_brush::Generate(Manager * manager, QMouseEvent *event)
{
    QColor paintColor = manager->get_current_color();
    float width = manager->get_brush_width();
    qreal radialSpeed = 2.f;
    int d = width / 2;
    int r = d / 2;
    QVector2D offset;
    if (event->buttons() == Qt::LeftButton) {
        this->init_brush(width);
        manager->fillWetMap(water, event->pos());

        Splat *newsplat = new Splat(event->pos(), width, paintColor);
        manager->pushBackSplat(newsplat);
        cout<<"new slat at: "<<event->x()<<" "<<event->y()<<endl;
        for (int i = 0; i < 6; i++) {
            qreal theta = i * M_PI / 3;
            offset.setX(r * cos(theta));
            offset.setY(r * sin(theta));

            newsplat = new Splat(event->pos() + offset.toPointF(),
                                 0.1f * radialSpeed * offset.normalized().toPointF(),
                                 d, 30, 0.5f * radialSpeed, 1.f, radialSpeed, paintColor);

            manager->pushBackSplat(newsplat);
            cout<<"new slat at: "<<event->x()<<" "<<event->y()<<endl;
        }

    }
}


void cruncy_brush::Generate(Manager * manager, QMouseEvent *event)
{
    QColor paintColor = manager->get_current_color();
    float width = manager->get_brush_width();
    if (event->buttons() == Qt::LeftButton) {
        this->init_brush(width * 2);
        manager->fillWetMap(water, event->pos());

        Splat *newsplat = new Splat(event->pos(), QPointF(0, 0), width,
                                    15, 5, 0.25f, 2.f, paintColor);

        manager->RewetSplats(water, event->pos());
        manager->pushBackSplat(newsplat);
        cout<<"new slat at: "<<event->x()<<" "<<event->y()<<endl;
    }
}

void wetOnwet_brush::Generate(Manager * manager, QMouseEvent *event)
{
    QColor paintColor = manager->get_current_color();
    float width = manager->get_brush_width();
    int smallD = width / 2;
    int bigD = 3 * width / 2;

    if (event->buttons() == Qt::LeftButton) {
        this->init_brush(width);
        manager->fillWetMap(water, event->pos());

        Splat *newsplat = new Splat(event->pos(), QPointF(0, 0), bigD,
                                    15, 5, 1.f, 2.f, paintColor);
        manager->pushBackSplat(newsplat);
        cout<<"new slat at: "<<event->x()<<" "<<event->y()<<endl;

        newsplat = new Splat(event->pos(), QPointF(0, 0), smallD,
                                    15, 5, 1.f, 2.f, paintColor);
        manager->pushBackSplat(newsplat);
        cout<<"new slat at: "<<event->x()<<" "<<event->y()<<endl;
        manager->RewetSplats(water, event->pos());
    }
}

void blobby_brush::Generate(Manager * manager, QMouseEvent *event)
{
    QColor paintColor = manager->get_current_color();
    float width = manager->get_brush_width();
    qreal firstD = (qreal) width / 3;
    qreal lastD = (qreal) width;
    Splat *newsplat;
    if (event->buttons() == Qt::LeftButton) {
        this->init_brush(width);
        manager->fillWetMap(water, event->pos());
        for (int i = 0; i < 4; i++) {
            qreal size = (qreal)rand() / RAND_MAX*(lastD - firstD) + firstD;
            QPointF pos;
            switch (i) {
            case 0:
                pos = event->pos() + QPointF(0, (width - size) / 2);
                break;
            case 1:
                pos = event->pos() + QPointF((width - size) / 2, 0);
            case 2:
                pos = event->pos() - QPointF(0, (width - size) / 2);
                break;
            case 3:
                pos = event->pos() - QPointF((width - size) / 2, 0);
            default:
                break;
            }

            newsplat = new Splat(pos, QPointF(0, 0), size,
                                 15, 5, 1.f, 2.f, paintColor);
            manager->pushBackSplat(newsplat);
            cout<<"new slat at: "<<event->x()<<" "<<event->y()<<endl;
        }
        manager->RewetSplats(water, event->pos());
    }
}
