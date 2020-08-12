#include "MainWindow.h"

#include <QPushButton>
#include <QMenuBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QScreen>

#include <iostream>

#include "core/NVSMIParser.h"

#include "processes/ProcessesView.h"

#include "utilization/gpu/GPUUtilizationWorker.h"
#include "utilization/memory/MemoryUtilizationWorker.h"

#include "utilization/gpu/GPUUtilizationContainer.h"
#include "utilization/memory/MemoryUtilizationContainer.h"

#include "resources/MainWindowResources.h"
#include "resources/QSplitterResources.h"
#include "constants.h"

namespace SessionSettings {
constexpr char MainWindow_geometry[] = "MainWindow/geometry";
constexpr char MainWindow_windowState[] = "MainWindow/windowState";
constexpr char MainWindow_utilizationSplitter[] = "MainWindow/utilizationSplitter";
}

using namespace SessionSettings;

MainWindow::MainWindow(QWidget*) {
    setWindowTitle("NVIDIA System Monitor");
    setWindowIcon(QIcon(ICON_PATH));

    auto layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    
    auto menuBar = new QMenuBar;
    auto menu = new QMenu("&Help");

    menu->addAction("&About NVSM", this, SLOT(about()), Qt::CTRL + Qt::Key_A);
    menu->addAction("&Help", this, SLOT(help()), Qt::CTRL + Qt::Key_H);
    menu->addSeparator();
    menu->addAction("&Settings", this, SLOT(help()));
    menu->addSeparator();
    menu->addAction("&Exit", qApp, SLOT(quit()));

    menuBar->addMenu(menu);
    layout->addWidget(menuBar);

    auto processes = new ProcessesView();

    auto utilizationWidget = new QWidget();
    auto utilizationLayout = new QVBoxLayout();
    auto gpuUtilizationContainer = new GPUUtilizationContainer();
    auto memoryUtilizationContainer = new MemoryUtilizationContainer();

    utilizationSplitter = new QSplitter();
    utilizationSplitter->setOrientation(Qt::Vertical);
    utilizationSplitter->setStyleSheet(getSplitterStylesheet());
    utilizationSplitter->addWidget(gpuUtilizationContainer->getWidget());
    utilizationSplitter->addWidget(memoryUtilizationContainer->getWidget());

    utilizationLayout->addWidget(utilizationSplitter);
    utilizationLayout->setMargin(32);

    utilizationWidget->setLayout(utilizationLayout);

    tabs = new QTabWidget();
    tabs->addTab(processes, "Processes");
    tabs->addTab(utilizationWidget, "Utilization");
    layout->addWidget(tabs);
    
    auto window = new QWidget();
    window->setLayout(layout);
    setCentralWidget(window);
    
    connect(processes->worker,
            &ProcessesWorker::dataUpdated, processes, &ProcessesView::onDataUpdated);
    connect(gpuUtilizationContainer->getWorker(),
            &GPUUtilizationWorker::dataUpdated, gpuUtilizationContainer, &GPUUtilizationContainer::onDataUpdated);
    connect(memoryUtilizationContainer->getWorker(),
            &MemoryUtilizationWorker::dataUpdated, memoryUtilizationContainer, &MemoryUtilizationContainer::onDataUpdated);

    NVSMIParser::init();

    workerThread = new WorkerThread;
    workerThread->workers[0] = processes->worker;
    workerThread->workers[1] = gpuUtilizationContainer->getWorker();
    workerThread->workers[2] = memoryUtilizationContainer->getWorker();
    workerThread->start();

    loadSettings();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    hide();

    saveSettings();

    workerThread->running = false;
    while (workerThread->isRunning()); // waiting for all workers to be safely removed
    event->accept();
}

QString MainWindow::getSplitterStylesheet() {
    QColor background = palette().color(QPalette::Window);
    QColor splitterColor, hoverSplitterColor, pressedSplitterColor;

    if (background.black() >= 128) {
        // dark theme
        splitterColor = background.lighter(150);
        hoverSplitterColor = background.lighter(225);
        pressedSplitterColor = background.lighter(300);
    } else {
        // light theme
        splitterColor = background.darker(150);
        hoverSplitterColor = background.darker(225);
        pressedSplitterColor = background.darker(300);
    }

    QString splitterStylesheet = Resources::QSplitter::Stylesheet;
    splitterStylesheet.replace("/* QSplitter_border_color */", splitterColor.name());
    splitterStylesheet.replace("/* QSplitter_hover_border_color */", hoverSplitterColor.name());
    splitterStylesheet.replace("/* QSplitter_pressed_border_color */", pressedSplitterColor.name());

    return splitterStylesheet;
}

void MainWindow::saveSettings() {
    QSettings settings("congard", "NVSM");
    settings.setValue(MainWindow_geometry, saveGeometry());
    settings.setValue(MainWindow_windowState, saveState());
    settings.setValue(MainWindow_utilizationSplitter, utilizationSplitter->saveState());
}

inline QSize defaultWindowSize(const QRect &screenGeometry) {
    return {
        screenGeometry.width() / 2,
        static_cast<int>(static_cast<float>(screenGeometry.height()) * 0.85f)
    };
}

void MainWindow::loadSettings() {
    QSettings settings("congard", "NVSM");

    auto geometryByteArray = settings.value(MainWindow_geometry).toByteArray();

    if (!geometryByteArray.isEmpty()) {
        restoreGeometry(geometryByteArray);
    } else {
        resize(defaultWindowSize(QGuiApplication::primaryScreen()->geometry()));
    }

    restoreState(settings.value(MainWindow_windowState).toByteArray());
    utilizationSplitter->restoreState(settings.value(MainWindow_utilizationSplitter).toByteArray());
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
