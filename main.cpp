#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>

#include "main.h"
#include "MainWidget.h"
#include "WindowHandler.h"

MainWidget * makeMainWidget(QWidget * parent) {
    MainWidget * ret = new MainWidget(parent);

    auto palette = ret->palette();
    palette.setColor(QPalette::Background, Qt::white);
    ret->setPalette(palette);

    QColor pressedColor = QColor(255, 0, 0, 150);
    ret->setPressedBrush(pressedColor);
    ret->setPressedPen(QPen(pressedColor, 2));

    QColor releasedColor = QColor(0, 255, 0, 150);
    ret->setReleasedBrush(releasedColor);
    ret->setReleasedPen(QPen(releasedColor, 2));

    QColor trackColor = QColor(Qt::black);
    ret->setTrackBrush(trackColor);
    ret->setTrackPen(QPen(trackColor, 2));

    return ret;
}

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);

    QMainWindow * wnd = new QMainWindow();

    MainWidget * wgt = makeMainWidget(wnd);

    WindowHandler * handler = new WindowHandler(wnd, wgt);

    QMenuBar * menuBar = new QMenuBar(wnd);

    QMenu * mainMenu = new QMenu(QObject::tr(PROJECT_NAME), menuBar);

    QAction * toggleTrackAction = new QAction(QObject::tr("Paint track"), mainMenu);
    toggleTrackAction->setCheckable(true);
    toggleTrackAction->setChecked(false);
    QObject::connect(toggleTrackAction, SIGNAL(toggled(bool)), handler, SLOT(toggleTrackAction_toggled(bool)));
    mainMenu->addAction(toggleTrackAction);

    QAction * toggleTargetAction = new QAction(QObject::tr("Target mode"), mainMenu);
    toggleTargetAction->setCheckable(true);
    toggleTargetAction->setChecked(false);
    QObject::connect(toggleTargetAction, SIGNAL(toggled(bool)), handler, SLOT(toggleTargetAction_toggled(bool)));
    mainMenu->addAction(toggleTargetAction);

    mainMenu->addSeparator();

    QAction * statisticsAction = new QAction(QObject::tr("Statistics"), mainMenu);
    QObject::connect(statisticsAction, SIGNAL(triggered(bool)), handler, SLOT(statisticsAction_triggered(bool)));
    mainMenu->addAction(statisticsAction);

    QAction * resetStatisticsAction = new QAction(QObject::tr("Reset statistics"), mainMenu);
    QObject::connect(resetStatisticsAction, SIGNAL(triggered(bool)), handler, SLOT(resetStatisticsAction_triggered(bool)));
    mainMenu->addAction(resetStatisticsAction);

    mainMenu->addSeparator();

    QAction * quitAction = new QAction(QObject::tr("Quit"), mainMenu);
    QObject::connect(quitAction, SIGNAL(triggered(bool)), handler, SLOT(quitAction_triggered(bool)));
    mainMenu->addAction(quitAction);

    menuBar->addMenu(mainMenu);

    QMenu * aboutMenu = new QMenu(QObject::tr("About"), menuBar);

    QAction * aboutAction = new QAction(QObject::tr("About " PROJECT_NAME), mainMenu);
    QObject::connect(aboutAction, SIGNAL(triggered(bool)), handler, SLOT(aboutAction_triggered(bool)));
    aboutMenu->addAction(aboutAction);

    QAction * aboutQtAction = new QAction(QObject::tr("About Qt"), mainMenu);
    QObject::connect(aboutQtAction, SIGNAL(triggered(bool)), handler, SLOT(aboutQtAction_triggered(bool)));
    aboutMenu->addAction(aboutQtAction);

    menuBar->addMenu(aboutMenu);

    wnd->setMenuBar(menuBar);

    wnd->setWindowTitle(QObject::tr(PROJECT_NAME));
    wnd->setCentralWidget(wgt);
    wnd->resize(640, 480);
    wnd->show();

    return app.exec();
}
