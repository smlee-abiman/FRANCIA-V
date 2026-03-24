#include "graphcanvas.h"

GraphCanvas::GraphCanvas(QQuickItem *parent) : QQuickPaintedItem(parent)
{
}

void GraphCanvas::updatePoints(QList<int> values, QPainter *painter, QColor color, bool isSpeed)
{
    int index;
    int counter = 1;

    QPainterPath path;
    if(values.count() > 0){
        path.moveTo(QPoint(0, calcYposition(values[m_startIndex], isSpeed)));
        for(index = 1; counter <= values.count(); index+=m_step ) {
            qreal x = calcXposition(counter, m_xDivisions);
            int yValue = 0;
            if(index < values.count()){
                yValue = values[index];
            }
            qreal y = calcYposition(yValue, isSpeed);

            path.lineTo(QPoint(x, y));

            counter++;
        }
        painter->setPen(color);
        painter->drawPath(path);
    }
    m_dirty = false;
}

void GraphCanvas::paint(QPainter *painter)
{
    //QElapsedTimer timer;
    //timer.start();

    /* Simplest way */
    painter->setRenderHints(QPainter::Antialiasing, true);

    updatePoints(m_redLineValues, painter, QColor("red"), false);
    updatePoints(m_blueLineValues, painter, QColor("blue"));
    //qDebug() << "Rendering time:" << timer.elapsed();

}
