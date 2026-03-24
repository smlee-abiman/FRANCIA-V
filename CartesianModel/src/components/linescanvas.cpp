#include "linescanvas.h"

LinesCanvas::LinesCanvas(QQuickItem *parent) : QQuickPaintedItem(parent)
{
}

void LinesCanvas::updatePoints(QList<int> values, QPainter *painter, QColor color)
{
    QPainterPath path;
    if(values.count() > 0){
        path.moveTo(QPoint(calcXposition(0, m_xDivisions), calcYposition(values[0])));

        for(int i = 1; i <= m_xDivisions; i++ ) {
            if(i < values.count()){
                path.lineTo(QPoint(calcXposition(i, m_xDivisions), calcYposition(values[i])));
            }
        }

        painter->setPen(color);
        painter->drawPath(path);
    }
}

void LinesCanvas::paint(QPainter *painter)
{
    //QElapsedTimer timer;
    //timer.start();

    /* Simplest way */
    painter->setRenderHints(QPainter::Antialiasing, true);

    updatePoints(m_lineValues, painter, QColor("red"));
    //qDebug() << "Rendering time:" << timer.elapsed();

}

