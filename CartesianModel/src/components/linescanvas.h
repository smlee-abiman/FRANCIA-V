#ifndef LINESCANVAS_H
#define LINESCANVAS_H

#include <QObject>
#include <QQuickItem>
#include <QPainter>
#include <QPainterPath>
#include <QElapsedTimer>
#include <QQuickPaintedItem>

class LinesCanvas : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal maxY READ maxY WRITE setMaxY)
    Q_PROPERTY(qreal xDivisions READ xDivisions WRITE setXDivisions)
    Q_PROPERTY(QList<int> lineValues READ lineValues WRITE setLineValues)

public:
    LinesCanvas(QQuickItem *parent = Q_NULLPTR);
    void paint(QPainter *painter);

    qreal maxY() const
    {
        return m_maxY;
    }

    qreal xDivisions() const
    {
        return m_xDivisions;
    }

    QList<int> lineValues()
    {
        return m_lineValues;
    }

protected:

private:
    qreal m_maxY = 500;
    qreal m_xDivisions = 24;
    QList<int> m_lineValues;

    inline qreal calcXposition(qreal value, qreal xdiv) {
        Q_UNUSED(xdiv)
        return value * width() / (xDivisions() - 1);
    }

    inline qreal calcYposition(qreal value) {
        auto yDivisions = m_maxY;
        return height() - (value * height() / yDivisions);
    }
    void updatePoints(QList<int> values, QPainter *painter, QColor color);

public slots:

    void setMaxY(qreal maxY)
    {
        m_maxY = maxY;
    }

    void setXDivisions(qreal xDivisions)
    {
        m_xDivisions = xDivisions;
    }

    void setLineValues(QList<int> lineValues)
    {
        m_lineValues = lineValues;
        update();
    }
};

#endif // LINESCANVAS_H
