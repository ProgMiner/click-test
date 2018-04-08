#include "MainWidget.h"

#include <QVector2D>
#include <QPainter>
#include <cmath>

float getSpeed(QTime start, QTime end) {
    unsigned int diff = end.msec() - start.msec();

    if (diff == 0) {
        return 0;
    }

    return 1000 / diff;
}

float getDistance(QPoint start, QPoint end) {
    return QVector2D(start).distanceToPoint(QVector2D(end));
}

MainWidget::Statistics::Statistics(const QVector<QPoint> & track, const QVector<QTime> & timeline) {
    totalClicks = track.size();

    if (totalClicks < 2) {
        totalTimeMsec = 0;

        minSpeed = maxSpeed = 0;
        minSpread = maxSpread = 0;
        minLocalSpread = maxLocalSpread = 0;

        return;
    }

    totalTimeMsec = timeline.back().msecsSinceStartOfDay() - timeline[0].msecsSinceStartOfDay();

    minSpeed = maxSpeed = getSpeed(timeline[0], timeline[1]);
    for (auto it = timeline.begin() + 1; it != timeline.end(); ++it) {
        float speed = getSpeed(* (it - 1), * it);

        if (minSpeed > speed) {
            minSpeed = speed;
        }

        if (maxSpeed < speed) {
            maxSpeed = speed;
        }

        avgSpeed += speed;
    }
    avgSpeed /= totalClicks;

    minSpread = maxSpread = getDistance(track[0], track[1]);
    for (auto it = track.begin(); it != track.end(); ++it) {
        float spread = getDistance(track[0], * it);

        if (minSpread > spread) {
            minSpread = spread;
        }

        if (maxSpread < spread) {
            maxSpread = spread;
        }

        avgSpread += spread;
    }
    avgSpread /= totalClicks;

    minLocalSpread = maxLocalSpread = getDistance(track[0], track[1]);
    for (auto it = track.begin() + 1; it != track.end(); ++it) {
        float localSpread = getDistance(* (it - 1), * it);

        if (minLocalSpread > localSpread) {
            minLocalSpread = localSpread;
        }

        if (maxLocalSpread < localSpread) {
            maxLocalSpread = localSpread;
        }

        avgLocalSpread += localSpread;
    }
    avgLocalSpread /= totalClicks;
}

MainWidget::~MainWidget() {
    delete track;
    if (release != nullptr) {
        delete release;
    }

    delete pressedBrush;
    delete releasedBrush;
    delete trackBrush;
    delete pressedPen;
    delete releasedPen;
    delete trackPen;
}

MainWidget::Statistics MainWidget::getStatistics() {
    return Statistics(* track, * timeline);
}


void MainWidget::setPressedBrush(const QBrush & brush) {
    delete pressedBrush;
    pressedBrush = new QBrush(brush);
}

void MainWidget::setReleasedBrush(const QBrush & brush) {
    delete releasedBrush;
    releasedBrush = new QBrush(brush);
}

void MainWidget::setTrackBrush(const QBrush & brush) {
    delete trackBrush;
    trackBrush = new QBrush(brush);
}

void MainWidget::setPressedPen(const QPen & pen) {
    delete pressedPen;
    pressedPen = new QPen(pen);
}

void MainWidget::setReleasedPen(const QPen & pen) {
    delete releasedPen;
    releasedPen = new QPen(pen);
}

void MainWidget::setTrackPen(const QPen & pen) {
    delete trackPen;
    trackPen = new QPen(pen);
}

void MainWidget::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), palette().background());

    if (track->isEmpty()) {
        return;
    }

    painter.setPen(* trackPen);
    painter.drawPoint(track->back());

    if (track->size() == 1) {
        return;
    }

    if (paintTrace) {
        for (auto it = track->begin() + 1; it != track->end(); ++it) {
            painter.drawLine(* (it - 1), * it);
        }
    }

    // Last press point
    QPoint lastPoint = track->back();

    // Previous press point
    QPoint prevPoint = * (track->end() - 2);

    // Press
    int dist = getDistance(prevPoint, lastPoint);

    painter.setPen(* pressedPen);
    painter.setBrush(* pressedBrush);
    painter.drawEllipse(prevPoint, dist, dist);
    painter.drawLine(prevPoint, lastPoint);

    // Release
    if (release != nullptr) {
        int dist = getDistance(lastPoint, * release);

        painter.setPen(* releasedPen);
        painter.setBrush(* releasedBrush);
        painter.drawEllipse(lastPoint, dist, dist);
        painter.drawLine(lastPoint, * release);

        painter.setPen(* trackPen);
        painter.drawLine(prevPoint, * release);
    }
}

void MainWidget::mousePressEvent(QMouseEvent * event) {
    if (release != nullptr) {
        * (track->end() - 1) = * release;

        delete release;
        release = nullptr;
    }

    timeline->push_back(QTime::currentTime());
    track->push_back(event->pos());
    update();
}

void MainWidget::mouseReleaseEvent(QMouseEvent * event) {
    release = new QPoint(event->pos());
    update();
}