#ifndef BRUSH_TYPE_H
#define BRUSH_TYPE_H

#include "library.h"
#include "manager.h"

class Manager;

class brush_type
{
public:
    virtual ~brush_type() {}
    virtual void Generate(Manager *manager, QMouseEvent *event) = 0;
protected:
    void init_brush (int width);
    Water *water;
};


class simple_brush : public brush_type
{
public:
    void Generate(Manager *manager, QMouseEvent *event);
};

class wetondry_brush : public brush_type
{
public:
    void Generate(Manager *manager, QMouseEvent *event);
};

class cruncy_brush : public brush_type
{
public:
    void Generate(Manager *manager, QMouseEvent *event);
};

class wetOnwet_brush : public brush_type
{
public:
    void Generate(Manager *manager, QMouseEvent *event);
};

class blobby_brush : public brush_type
{
public:
    void Generate(Manager *manager, QMouseEvent *event);
};


#endif // BRUSH_TYPE_H
