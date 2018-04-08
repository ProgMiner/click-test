#pragma once

#include <QPen>
#include <QBrush>
#include <QPoint>
#include <QWidget>
#include <QMouseEvent>
#include <QVector>
#include <QTime>

class MainWidget: public QWidget {
public:
    struct Statistics {
        unsigned long totalClicks;
        int totalTimeMsec;

        float minSpeed, maxSpeed, avgSpeed = 0;
        float minSpread, maxSpread, avgSpread = 0;
        float minLocalSpread, maxLocalSpread, avgLocalSpread = 0;

        Statistics(const QVector < QPoint > & track, const QVector < QTime > & timeline);
    };

    bool paintTrace = false,
         targetMode = false;

    MainWidget(QWidget * parent = nullptr): QWidget(parent) {}

    virtual ~MainWidget();

    Statistics getStatistics();

    QBrush getPressedBrush() { return * pressedBrush; }
    QBrush getReleasedBrush() { return * releasedBrush; }
    QBrush getTrackBrush() { return * trackBrush; }
    QPen getPressedPen() { return * pressedPen; }
    QPen getReleasedPen() { return * releasedPen; }
    QPen getTrackPen() { return * trackPen; }

    void setPressedBrush(const QBrush & brush);
    void setReleasedBrush(const QBrush & brush);
    void setTrackBrush(const QBrush & brush);
    void setPressedPen(const QPen & pen);
    void setReleasedPen(const QPen & pen);
    void setTrackPen(const QPen & pen);

    void paintEvent(QPaintEvent * event) override;

    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent *) override;

protected:
    QVector < QPoint > * track = new QVector < QPoint > ();
    QVector < QTime > * timeline = new QVector < QTime > ();
    QPoint * release = nullptr;

    QBrush * pressedBrush = new QBrush(),
           * releasedBrush = new QBrush(),
           * trackBrush = new QBrush();

    QPen * pressedPen = new QPen(),
         * releasedPen = new QPen(),
         * trackPen = new QPen();
};
