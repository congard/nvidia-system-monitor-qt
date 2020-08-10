#ifndef QNVSM_MAINWINDOW_H
#define QNVSM_MAINWINDOW_H

#include <QMainWindow>

#include "core/WorkerThread.h"
#include "core/Worker.h"

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event) override;

public:
    WorkerThread *workerThread;
    QTabWidget *tabs;

private:
    QString getSplitterStylesheet();

private slots:
    static void about();
    static void help();
};

#endif
