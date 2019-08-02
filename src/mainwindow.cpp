#include "mainwindow.h"
#include <QPushButton>
#include <iostream>
#include <QMenuBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "processes.h"
#include "utilization.h"

MainWindow::MainWindow(QWidget*) { 
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menu = new QMenu("&Help");

    menu->addAction("&About NVSM", this, SLOT(about()), Qt::CTRL + Qt::Key_A);
    menu->addAction("&Help", this, SLOT(help()), Qt::CTRL + Qt::Key_H);
    menu->addSeparator();
    menu->addAction("&Settings", this, SLOT(help()));
    menu->addSeparator();
    menu->addAction("&Exit", qApp, SLOT(quit()));

    menuBar->addMenu(menu);
    layout->addWidget(menuBar);
    
    ProcessesTableView *processes = new ProcessesTableView;
    
    QWidget *gwidget = new QWidget();
    QVBoxLayout *glayout = new QVBoxLayout;
    GPUUtilization *gutilization = new GPUUtilization;
    glayout->addWidget(gutilization);
    glayout->setMargin(32);
    gwidget->setLayout(glayout);

    QWidget *mwidget = new QWidget();
    QVBoxLayout *mlayout = new QVBoxLayout;
    MemoryUtilization *mutilization = new MemoryUtilization;
    mlayout->addWidget(mutilization);
    mlayout->setMargin(32);
    mwidget->setLayout(mlayout);
    
    tabs = new QTabWidget();
    tabs->addTab(processes, "Processes");
    tabs->addTab(gwidget, "GPU Utilization");
    tabs->addTab(mwidget, "Memory Utilization");
    layout->addWidget(tabs);
    
    QWidget *window = new QWidget();
    window->setLayout(layout);
    setCentralWidget(window);
    
    connect(processes->worker, &ProcessesWorker::dataUpdated, processes, &ProcessesTableView::onDataUpdated);
    connect(gutilization->worker, &GPUUtilizationWorker::dataUpdated, gutilization, &GPUUtilization::onDataUpdated);
    connect(mutilization->worker, &MemoryUtilizationWorker::dataUpdated, mutilization, &MemoryUtilization::onDataUpdated);
    
    workerThread = new WorkerThread;
    workerThread->workers[0] = processes->worker;
    workerThread->workers[1] = gutilization->worker;
    workerThread->workers[2] = mutilization->worker;
    workerThread->start();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    hide();
    workerThread->running = false;
    while (workerThread->isRunning()); // waiting for all workers to be safely removed
    event->accept();
}

#define _(str) QString(str)
void MainWindow::about() {
    QMessageBox::information(
        nullptr,
        "About",
        _("<font size=4><b>NVIDIA System Monitor</b></font>") +
        _("<br>Version 1.0<br>The app monitors your Nvidia GPU<br><br>Developed by Daniel Bernar") +
        _("<br><a href='dbcongard@gmail.com'>dbcongard@gmail.com</a>") +
        _("<br><br><a href='https://github.com/congard/nvidia-system-monitor-qt/blob/master/DONATE.md'>Donate</a> <a href='https://github.com/congard/nvidia-system-monitor-qt'>GitHub</a> <a href='https://t.me/congard'>Telegram</a>")
    );
}

void MainWindow::help() {
    QMessageBox msgBox;
    msgBox.setText("<font size=4><b>Help</b></font>");
    msgBox.setInformativeText(
        _("<b>Settings</b><br>By default, update delay is 2 seconds (2000 ms). ") +
        _("You most likely want to change this value to, for example, 500 ms. To do this, create file <i>config</i> in the folder") +
        _("<i>~/.config/nvidia-system-monitor</i>") +
        _("<br><b>config values:</b>") +
        _("<ul>") +
            _("<li>updateDelay &lt;time in ms&gt;</li>") +
            _("<li>graphLength &lt;time in ms&gt;</li>") +
            _("<li>gpuColor &lt;gpu index&gt; &lt;red&gt; &lt;green&gt; &lt;blue&gt;</li>") +
        _("</ul>") +
        _("<b>Processes</b>") +
        _("<ul>") +
            _("<li>Name - process name</li>") +
            _("<li>Type - \"C\" for Compute Process, \"G\" for Graphics Process, and \"C+G\" for the process having both Compute and Graphics contexts</li>") +
            _("<li>GPU ID - id of GPU in which process running</li>") +
            _("<li>pid - process id</li>") +
            _("<li>sm [%]</li>") +
            _("<li>mem [%]</li>") +
            _("<li>enc [%]</li>") +
            _("<li>dec [%]</li>") +
        _("</ul>") +
        _("<b>GPU Utilization</b><br>This section displays a graph of gpu utilization.") +
        _("<br><b>Memory Utilization</b><br>This section displays a graph of memory utilization.") +
        _("<br><a href='https://github.com/congard/nvidia-system-monitor-qt/blob/master/DONATE.md'>Donate</a> <a href='https://github.com/congard/nvidia-system-monitor-qt'>GitHub</a> <a href='https://t.me/congard'>Telegram</a>")
    );
    msgBox.exec();
}
