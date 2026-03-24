#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <QObject>
#include <QQuickItem>
#include <QPainter>
#include <QPainterPath>
#include <QElapsedTimer>
#include <QQuickPaintedItem>

class GraphCanvas : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal minY READ minY WRITE setMinY)
    Q_PROPERTY(qreal maxY READ maxY WRITE setMaxY)
    Q_PROPERTY(qreal speedMinY READ speedMinY WRITE setSpeedMinY)
    Q_PROPERTY(qreal speedMaxY READ speedMaxY WRITE setSpeedMaxY)
    Q_PROPERTY(qreal torqueMinY READ torqueMinY WRITE setTorqueMinY)
    Q_PROPERTY(qreal torqueMaxY READ torqueMaxY WRITE setTorqueMaxY)
    Q_PROPERTY(qreal xDivisions READ xDivisions WRITE setXDivisions)
    Q_PROPERTY(int step READ step WRITE setStep)
    Q_PROPERTY(QList<int> redLineValues READ redLineValues WRITE setRedLineValues)
    Q_PROPERTY(QList<int> blueLineValues READ blueLineValues WRITE setBlueLineValues)
    Q_PROPERTY(int startIndex READ startIndex WRITE setStartIndex)
    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)

public:
    GraphCanvas(QQuickItem *parent = Q_NULLPTR);
    void paint(QPainter *painter);

    qreal minY() const
    {
        return m_minY;
    }

    qreal maxY() const
    {
        return m_maxY;
    }
    qreal speedMinY() const
    {
        return m_speedMinY;
    }

    qreal speedMaxY() const
    {
        return m_speedMaxY;
    }
    qreal torqueMinY() const
    {
        return m_torqueMinY;
    }

    qreal torqueMaxY() const
    {
        return m_torqueMaxY;
    }

    qreal xDivisions() const
    {
        return m_xDivisions;
    }

    int step() const
    {
        return m_step;
    }

    bool dirty() const
    {
        return m_dirty;
    }

    QList<int> redLineValues()
    {
        return m_redLineValues;
    }

    QList<int> blueLineValues()
    {
        return m_blueLineValues;
    }

    int startIndex()
    {
        return m_startIndex;
    }

protected:

private:
    qreal m_minY = -100;
    qreal m_maxY = 100;
    qreal m_speedMinY = -100;
    qreal m_speedMaxY = 100;
    qreal m_torqueMinY = -100;
    qreal m_torqueMaxY = 100;
    qreal m_xDivisions = 300;
    QList<int> m_redLineValues;
    QList<int> m_blueLineValues;
    int m_step = 0;
    int m_startIndex = 0;
    bool m_dirty = false;

    inline qreal calcXposition(qreal value, qreal xdiv) {
        Q_UNUSED(xdiv)
        return value * width() / xDivisions();
    }

    inline qreal calcYposition(qreal value, bool isSpeed = true) {
        auto yDivisions = (isSpeed == true) ? (m_speedMaxY - m_speedMinY) : m_torqueMaxY - m_torqueMinY;
        auto step = height() / yDivisions;
        return (height() / 2) + ((value * -1) * step);
    }
    void updatePoints(QList<int> values, QPainter *painter, QColor color, bool isSpeed = true);

public slots:
    void setMinY(qreal minY)
    {
        m_minY = minY;
    }

    void setMaxY(qreal maxY)
    {
        m_maxY = maxY;
    }

    void setSpeedMinY(qreal speedMinY)
    {
        m_speedMinY = speedMinY;
    }

    void setSpeedMaxY(qreal speedMaxY)
    {
        m_speedMaxY = speedMaxY;
    }

    void setTorqueMinY(qreal torqueMinY)
    {
        m_torqueMinY = torqueMinY;
    }

    void setTorqueMaxY(qreal torqueMaxY)
    {
        m_torqueMaxY = torqueMaxY;
    }

    void setXDivisions(qreal xDivisions)
    {
        m_xDivisions = xDivisions;
    }

    void setRedLineValues(QList<int> redLineValues)
    {
        m_redLineValues = redLineValues;
        update();
    }

    void setBlueLineValues(QList<int> blueLineValues)
    {
        m_blueLineValues = blueLineValues;
        update();
    }

    void setStep(int step)
    {
        m_step = step;
        update();
    }

    void setStartIndex(int startIndex)
    {
        m_startIndex = startIndex;
    }

    void setDirty(bool dirty)
    {
        if (m_dirty == dirty)
            return;

        m_dirty = dirty;
        if (m_dirty == true) update();
        emit dirtyChanged(m_dirty);
    }

signals:
    void dirtyChanged(bool dirty);
};

#endif // GRAPHCANVAS_H
