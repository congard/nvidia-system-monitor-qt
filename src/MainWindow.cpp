#include "MainWindow.h"

#include <QApplication>
#include <QSettings>
#include <QScreen>
#include <QPushButton>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QSplitter>
#include <QMessageBox>

#include "core/SettingsManager.h"

#include "processes/ProcessesView.h"

#include "utilization/gpu/GPUUtilizationWorker.h"
#include "utilization/memory/MemoryUtilizationWorker.h"

#include "utilization/gpu/GPUUtilizationContainer.h"
#include "utilization/memory/MemoryUtilizationContainer.h"

#include "resources/MainWindowResources.h"
#include "resources/QSplitterResources.h"

#include "SettingsDialog.h"

#define constant inline constexpr auto

namespace SessionSettings {
constant MainWindow_geometry = "MainWindow/geometry";
constant MainWindow_windowState = "MainWindow/windowState";
constant MainWindow_utilizationSplitter = "MainWindow/utilizationSplitter";
}

using namespace SessionSettings;

constant gpuUtilizationContainerName = "gpuUtilizationContainer";
constant memoryUtilizationContainerName = "memoryUtilizationContainer";
constant utilizationSplitterName = "utilizationSplitter";

MainWindow::MainWindow(QWidget*) {
    setWindowTitle("NVIDIA System Monitor");
    setWindowIcon(QIcon(ICON_PATH));

    auto layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    // menu bar
    auto menuBar = new QMenuBar;

    auto fileMenu = new QMenu("&File");
    fileMenu->addAction("&Settings", this, SLOT(settings()));
    fileMenu->addSeparator();
    fileMenu->addAction("&Exit", this, SLOT(quit()));

    auto helpMenu = new QMenu("&Help");
    helpMenu->addAction("&About NVSM", this, SLOT(about()), Qt::CTRL + Qt::Key_A);
    helpMenu->addAction("&Help", this, SLOT(help()), Qt::CTRL + Qt::Key_H);

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(helpMenu);

    // tabs
    auto processes = new ProcessesView();

    auto gpuUtilizationContainer = new GPUUtilizationContainer();
    gpuUtilizationContainer->setObjectName(gpuUtilizationContainerName);

    auto memoryUtilizationContainer = new MemoryUtilizationContainer();
    memoryUtilizationContainer->setObjectName(memoryUtilizationContainerName);

    auto utilizationSplitter = new QSplitter();
    utilizationSplitter->setObjectName(utilizationSplitterName);
    utilizationSplitter->setOrientation(Qt::Vertical);
    utilizationSplitter->setStyleSheet(getSplitterStylesheet());
    utilizationSplitter->addWidget(gpuUtilizationContainer);
    utilizationSplitter->addWidget(memoryUtilizationContainer);

    auto utilizationLayout = new QVBoxLayout();
    utilizationLayout->addWidget(utilizationSplitter);
    utilizationLayout->setContentsMargins(32, 32, 32, 32);

    auto utilizationWidget = new QWidget();
    utilizationWidget->setLayout(utilizationLayout);

    // add tabs
    auto tabs = new QTabWidget();
    tabs->addTab(processes, "Processes");
    tabs->addTab(utilizationWidget, "Utilization");

    layout->addWidget(menuBar);
    layout->addWidget(tabs);
    
    auto window = new QWidget();
    window->setLayout(layout);
    setCentralWidget(window);

    // configure
    connect(processes->worker,
            &ProcessesWorker::dataUpdated, processes, &ProcessesView::onDataUpdated);
    connect(gpuUtilizationContainer->getWorker(),
            &GPUUtilizationWorker::dataUpdated, gpuUtilizationContainer, &GPUUtilizationContainer::onDataUpdated);
    connect(memoryUtilizationContainer->getWorker(),
            &MemoryUtilizationWorker::dataUpdated, memoryUtilizationContainer, &MemoryUtilizationContainer::onDataUpdated);

    workerThread = new WorkerThread;
    workerThread->workers[0] = processes->worker;
    workerThread->workers[1] = gpuUtilizationContainer->getWorker();
    workerThread->workers[2] = memoryUtilizationContainer->getWorker();
    workerThread->start();

    // load MainWindow settings
    loadSettings();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    quit();
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
    QSettings settings(NVSM_SETTINGS);
    settings.setValue(MainWindow_geometry, saveGeometry());
    settings.setValue(MainWindow_windowState, saveState());
    settings.setValue(MainWindow_utilizationSplitter, findChild<QSplitter *>(utilizationSplitterName)->saveState());
}

inline QSize defaultWindowSize(const QRect &screenGeometry) {
    return {
        screenGeometry.width() / 2,
        static_cast<int>(static_cast<float>(screenGeometry.height()) * 0.85f)
    };
}

void MainWindow::loadSettings() {
    QSettings settings(NVSM_SETTINGS);

    auto geometryByteArray = settings.value(MainWindow_geometry).toByteArray();

    if (!geometryByteArray.isEmpty()) {
        restoreGeometry(geometryByteArray);
    } else {
        resize(defaultWindowSize(QGuiApplication::primaryScreen()->geometry()));
    }

    restoreState(settings.value(MainWindow_windowState).toByteArray());
    findChild<QSplitter *>(utilizationSplitterName)->restoreState(settings.value(MainWindow_utilizationSplitter).toByteArray());
}

void MainWindow::quit() {
    hide();

    saveSettings();

    workerThread->running = false;
    while (workerThread->isRunning()); // waiting for all workers to be safely removed

    QApplication::quit();
}

void MainWindow::settings() {
    auto oldGraphLength = SettingsManager::getGraphLength();

    SettingsDialog dialog;
    dialog.exec();

    auto gpuUtilizationContainer = findChild<GPUUtilizationContainer *>(gpuUtilizationContainerName);
    auto memoryUtilizationContainer = findChild<MemoryUtilizationContainer *>(memoryUtilizationContainerName);

    gpuUtilizationContainer->updateData();
    memoryUtilizationContainer->updateData();

    // we need to reset graphs if graph length has been changed
    if (oldGraphLength != SettingsManager::getGraphLength()) {
        auto resetGraph = [](UtilizationContainer *container)
        {
            auto worker = container->getWorker();

            QMutexLocker locker(&worker->mutex);

            for (uint i = 0; i < SettingsManager::getGPUCount(); i++) {
                worker->gpoints[i] = {};
            }
        };

        resetGraph(gpuUtilizationContainer);
        resetGraph(memoryUtilizationContainer);
    }
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
