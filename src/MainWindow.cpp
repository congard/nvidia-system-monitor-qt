#include "MainWindow.h"

#include <QPushButton>
#include <QMenuBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include <iostream>

#include "processes/ProcessesView.h"
#include "utilization/gpu/GPUUtilizationWidget.h"
#include "utilization/memory/MemoryUtilizationWidget.h"

#include "utilization/gpu/GPUUtilizationWorker.h"
#include "utilization/memory/MemoryUtilizationWorker.h"

#include "resources/MainWindowResources.h"
#include "constants.h"

MainWindow::MainWindow(QWidget*) {
    setWindowIcon(QIcon(ICON_PATH));

    auto *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    
    auto *menuBar = new QMenuBar;
    auto *menu = new QMenu("&Help");

    menu->addAction("&About NVSM", this, SLOT(about()), Qt::CTRL + Qt::Key_A);
    menu->addAction("&Help", this, SLOT(help()), Qt::CTRL + Qt::Key_H);
    menu->addSeparator();
    menu->addAction("&Settings", this, SLOT(help()));
    menu->addSeparator();
    menu->addAction("&Exit", qApp, SLOT(quit()));

    menuBar->addMenu(menu);
    layout->addWidget(menuBar);

    auto *processes = new ProcessesView;

    UtilizationWidget::init();

    auto *utilizationWidget = new QWidget();
    auto *utilizationLayout = new QVBoxLayout();
    auto *gpuUtilizationWidget = new GPUUtilizationWidget();
    auto *memoryUtilizationWidget = new MemoryUtilizationWidget();
    utilizationLayout->addWidget(gpuUtilizationWidget);
    utilizationLayout->addWidget(memoryUtilizationWidget);
    utilizationLayout->setMargin(32);
    utilizationWidget->setLayout(utilizationLayout);
    
    tabs = new QTabWidget();
    tabs->addTab(processes, "Processes");
    tabs->addTab(utilizationWidget, "Utilization");
    layout->addWidget(tabs);
    
    auto *window = new QWidget();
    window->setLayout(layout);
    setCentralWidget(window);
    
    connect(processes->worker,
            &ProcessesWorker::dataUpdated, processes, &ProcessesView::onDataUpdated);
    connect(gpuUtilizationWidget->worker,
            &GPUUtilizationWorker::dataUpdated, gpuUtilizationWidget, &GPUUtilizationWidget::onDataUpdated);
    connect(memoryUtilizationWidget->worker,
            &MemoryUtilizationWorker::dataUpdated, memoryUtilizationWidget, &MemoryUtilizationWidget::onDataUpdated);
    
    workerThread = new WorkerThread;
    workerThread->workers[0] = processes->worker;
    workerThread->workers[1] = gpuUtilizationWidget->worker;
    workerThread->workers[2] = memoryUtilizationWidget->worker;
    workerThread->start();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    hide();
    workerThread->running = false;
    while (workerThread->isRunning()); // waiting for all workers to be safely removed
    event->accept();
}

void MainWindow::about() {
    QMessageBox::information(nullptr, "About", Resources::MainWindow::About);
}

void MainWindow::help() {
    QMessageBox msgBox;
    msgBox.setText("<font size=4><b>Help</b></font>");
    msgBox.setInformativeText(Resources::MainWindow::Help);
    msgBox.exec();
}
