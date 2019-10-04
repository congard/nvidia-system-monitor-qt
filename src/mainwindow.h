#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qobjectdefs.h>

#include "worker.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    WorkerThread *workerThread;
    QTabWidget *tabs;
    
    explicit MainWindow(QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event) override;
private slots:
    static void about();
    static void help();
};

#endif
