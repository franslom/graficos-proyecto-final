#include "humidity.h"

Humidity::Humidity(const Humidity &obj)
{
    w = obj.w;
    h = obj.h;
    this->humidityMap = new unsigned char[obj.w * obj.h];
    this->waterVelX = obj.waterVelX;
    this->waterVelY = obj.waterVelY;
}


Humidity::Humidity(int width, int height) : w(width), h(height)
{
    this->humidityMap = new unsigned char[width * height];
    this->waterVelX = new float[width * height];
    this->waterVelY = new float[width * height];
}


void Humidity::Update()
{
    int i;
    uchar w_;
    int x, y;
    #pragma omp for
    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++) {
            i = x + y * w;
            w_ = humidityMap[i];
            if (w_ > 0) {
                humidityMap[i]--;
            } else {
                waterVelX[i] = 0;
                waterVelY[i] = 0;
            }
        }
}

void Humidity::Fill(Water *Place, QPointF pos)
{
    foreach (QPoint wetDot, *Place) {
        int x = wetDot.x() + pos.x();
        int y = wetDot.y() + pos.y();
        if (x < 0 || w <= x || y < 0 || h <= y)
            continue;

        int pixIndex = x + y * w;
        humidityMap[pixIndex] = 255;
        waterVelX[pixIndex] = 4.f * wetDot.x() / wetDot.manhattanLength();
        waterVelY[pixIndex] = 4.f * wetDot.y() / wetDot.manhattanLength();
    }
}

char Humidity::GetHumidity(int x, int y)
{
    if (x < 0 || w <= x || y < 0 || h <= y)
                    return 0;
    int pixelIndex = x + y * w;
    return humidityMap[pixelIndex];
}

QPointF Humidity::GetVelocity(int x, int y)
{
    if (x < 0 || w <= x || y < 0 || h <= y)
                    return QPointF(0,0);
    int pixelIndex = x + y * w;
    return QPointF(waterVelX[pixelIndex], waterVelY[pixelIndex]);
}

