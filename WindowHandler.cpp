#include <QApplication>
#include <QMessageBox>
#include <QTableView>
#include <QDebug>

#include "main.h"
#include "WindowHandler.h"

void WindowHandler::toggleTrackAction_toggled(bool checked) {
    mainWidget->paintTrace = checked;
    mainWidget->update();
}

void WindowHandler::toggleTargetAction_toggled(bool checked) {
    mainWidget->targetMode = checked;
    mainWidget->update();
}

void WindowHandler::statisticsAction_triggered(bool) {
    auto stats = mainWidget->getStatistics();

    QString text(tr(
        "Total: %1 clicks\n"            "Total time: %2 sec\n\n"
        "Maximum speed: %3 click/sec\n" "Minimum speed: %4 click/sec\n"  "Average speed: %5 click/sec\n\n"
        "Maximum spread: %6 pixel\n"    "Minimum spread: %7 pixel\n"     "Average spread: %8 pixel\n\n"
        "Maximum local spread: %9 pixel\nMinimum local spread: %10 pixel\nAverage local spread: %11 pixel"
    ));

    text = text.arg(stats.totalClicks)   .arg((float) stats.totalTimeMsec / 1000)
               .arg(stats.maxSpeed)      .arg(stats.minSpeed)      .arg(stats.avgSpeed)
               .arg(stats.maxSpread)     .arg(stats.minSpread)     .arg(stats.avgSpread)
               .arg(stats.maxLocalSpread).arg(stats.minLocalSpread).arg(stats.avgLocalSpread);

    auto action = QMessageBox::information(
        mainWindow,
        tr("Statistics"),
        text,
        QMessageBox::Ok,
        QMessageBox::Reset
    );

    switch (action) {
    case QMessageBox::Reset:
        {
            auto confirmation = QMessageBox::question(
                mainWindow,
                tr("Statistics"),
                tr("Do you want to reset statistics?"),
                QMessageBox::Yes,
                QMessageBox::No
            );

            if (confirmation == QMessageBox::Yes) {
                resetStatisticsAction_triggered(false);
            }
        }
    default: break;
    }
}

void WindowHandler::resetStatisticsAction_triggered(bool) {
    MainWidget * prevMainWidget = mainWidget;

    mainWidget = makeMainWidget(mainWindow);
    mainWidget->paintTrace = prevMainWidget->paintTrace;
    mainWidget->targetMode = prevMainWidget->targetMode;

    mainWindow->setCentralWidget(mainWidget);

    delete prevMainWidget;
}

void WindowHandler::quitAction_triggered(bool) {
    QApplication::exit(0);
}

void WindowHandler::aboutAction_triggered(bool) {
    QMessageBox::about(mainWindow, tr("About " PROJECT_NAME), PROJECT_NAME " " PROJECT_VERSION);
}

void WindowHandler::aboutQtAction_triggered(bool) {
    QMessageBox::aboutQt(mainWindow, tr("About Qt"));
}