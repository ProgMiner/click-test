#pragma once

#include <QObject>
#include <QMainWindow>

#include "MainWidget.h"

class WindowHandler: public QObject {
    Q_OBJECT

public:
    WindowHandler(
        QMainWindow * mainWindow,
        MainWidget * mainWidget
    ):
        QObject(mainWindow),
        mainWindow(mainWindow),
        mainWidget(mainWidget)
    {}

    virtual ~WindowHandler() {}

public slots:
    void toggleTrackAction_toggled(bool checked);
    void toggleTargetAction_toggled(bool checked);

    void statisticsAction_triggered(bool);
    void resetStatisticsAction_triggered(bool);
    
    void quitAction_triggered(bool);

    void aboutAction_triggered(bool);
    void aboutQtAction_triggered(bool);

protected:
    QMainWindow * mainWindow;
    MainWidget * mainWidget;

};
