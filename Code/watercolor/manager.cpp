#include "manager.h"

#include "manager.h"

Manager::Manager()
    : m_program(0)
{
    m_active = new QList<Splat*>();
    m_fixed = new QList<Splat*>();
    m_dried = new QList<Splat*>();
    m_startTime = QTime::currentTime();
    timer = new QTimer();
    timer->setInterval(33);
    generator = new simple_brush();
    brush_width=10;


}

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

void Manager::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

GLfloat transparent_color[4]={1.0f,1.0f,1.0f,0.0f};
GLfloat tmp_vertex[26][2];
GLfloat tmp_color[26][3];

void Manager::render()
{

    glViewport(0,0,window_w,window_h);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);

    m_program->setUniformValue(m_matrixUniform, matrix);

    int checkSplat;

    foreach (Splat* splat, *m_active) {
        checkSplat = splat->Update(humidity);

        if (checkSplat == Splat::Fixed)
        {
            m_fixed->push_back(splat);
            m_active->removeOne(splat);
        }
    }

    foreach (Splat* splat, *m_fixed) {
        checkSplat = splat->Update(humidity);

        if (checkSplat == Splat::Dried)
        {
            m_dried->push_back(splat);
            m_fixed->removeOne(splat);
            cout<<"Dried splat: "<<m_dried->size()<<endl;
        }
    }

    draw_vector_splat(m_active);
    draw_vector_splat(m_fixed);
    draw_vector_splat(m_dried);
    humidity->Update();


    m_program->release();
}


void Manager::mousePressEvent(QMouseEvent *event)
{
    last_pos=event->pos();
    generator->Generate(this, event);
    if (!timer->isActive())
        timer->start();
}

void Manager::mouseReleaseEvent(QMouseEvent *event)
{
    generator->Generate(this, event);
}

void Manager::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D *m_len = new QVector2D(last_pos - event->pos());
    if (m_len->length() > brush_width * 0.1) {
        last_pos = event->pos();
        generator->Generate(this, event);
    }
}



void Manager::set_color(QColor color)
{
    current_color = color;
}


void Manager::add_splat(Splat)
{
humidity->Update();
}

void Manager::pushBackSplat(Splat *splatToPush)
{
    m_active->push_back(splatToPush);
    cout<<"splat vector size: "<< m_active->size()<<endl;
}

QColor Manager::get_current_color()
{
    return current_color;
}

float Manager::get_brush_width()
{
    return brush_width;
}

void Manager::set_wh(int w, int h)
{
    window_w=w;
    window_h=h;
    current_color=QColor(Qt::blue);
}


void Manager::set_brushtype(int id)
{
    delete generator;
    switch (id) {
    case Simple:
        generator = new simple_brush();
        break;
    case WetOnDry:
        generator = new wetondry_brush();
    case Cruncy:
        generator = new cruncy_brush();
    case WetOnWet:
        generator = new wetOnwet_brush();
    case Blobby:
        generator = new blobby_brush();
    default:
        break;
    }
}

void Manager::draw_vector_splat(QList<Splat*> *vector)
{
    foreach (Splat* splat, *vector)
    {
        GLfloat tmp_vertex[130][2];
        GLfloat tmp_color[130][4];
        QColor color=splat->getColor();
        QPolygonF vertices=splat->get_vertices();
        tmp_color[0][0]=color.red();
        tmp_color[0][1]=color.green();
        tmp_color[0][2]=color.blue();
        tmp_color[0][3]=0.33;
        //tmp_color[0][3]=color.alpha();
        tmp_vertex[0][0]=((splat->get_center().x()*3.0)/window_w)-1.5;
        tmp_vertex[0][1]=1.2-((splat->get_center().y()*2.4)/window_h);
        /*tmp_vertex[0][0]=splat->get_center().x();
        tmp_vertex[0][1]=splat->get_center().y();*/
        for(int i=0;i<vertices.size();i++)
        {
            tmp_color[i+1][0]=color.red();
            tmp_color[i+1][1]=color.green();
            tmp_color[i+1][2]=color.blue();
            tmp_color[i+1][3]=0.33;
            //tmp_color[0][3]=color.alpha();

            tmp_vertex[i+1][0]=((vertices[i].x()*3.0)/window_w)-1.5;
            tmp_vertex[i+1][1]=1.2-((vertices[i].y()*2.4)/window_h);
            /*tmp_vertex[i+1][0]=vertices[i].x();
            tmp_vertex[i+1][1]=vertices[i].y();*/
        }
        tmp_color[129][0]=color.red();
        tmp_color[129][1]=color.green();
        tmp_color[129][2]=color.blue();
        tmp_color[129][3]=0.33;
        //tmp_color[0][3]=color.alpha();

        tmp_vertex[129][0]=((vertices[0].x()*3.0)/window_w)-1.5;
        tmp_vertex[129][1]=1.2-((vertices[0].y()*2.4)/window_h);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, tmp_vertex);
        glVertexAttribPointer(m_colAttr, 4, GL_FLOAT, GL_FALSE, 0, tmp_color);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 130);
        glDisable(GL_BLEND);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }
}

void Manager::fillWetMap(Water *placeForWetting, QPointF pos)
{
    humidity->Fill(placeForWetting, pos);
}

void Manager::RewetSplats(Water *m_water, QPointF pos)
{
    QList<Splat*> buf = *m_fixed;
    Splat* bufSplt;
    QMutex locker;
    int i;

    #pragma omp parallel for
    for (i = 0; i < buf.length(); i++) {
        bufSplt = buf.at(i);
        if (bufSplt->ReHumidity(m_water, humidity, pos) == Splat::Flowing) {
            locker.lock();
            m_active->push_back(bufSplt);
            m_fixed->removeOne(bufSplt);
            locker.unlock();
        }
    }
}

void Manager::set_Humidity(Humidity &humi)
{
    humidity = new Humidity(humi);

}
void Manager::set_brushwidth(int w)
{
    brush_width=w;
}
