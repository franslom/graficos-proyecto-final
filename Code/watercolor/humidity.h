#ifndef HUMIDITY_H
#define HUMIDITY_H

#include "library.h"

class Humidity
{
private:
    int w;
    int h;
    uchar* humidityMap;
    float* waterVelX;
    float* waterVelY;
public:
    Humidity();

    Humidity(int , int );
    Humidity(const Humidity &);
    ~Humidity() {
        delete[] humidityMap;
    }

    void Update();
    void Fill(Water *, QPointF );
    char GetHumidity(int , int );
    QPointF GetVelocity(int , int );

};

#endif // HUMIDITY_H
