#include "MainWindow.h"

#include <QApplication>
#include <QSettings>
#include <QScreen>
#include <QPushButton>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QScrollArea>

#include "core/SettingsManager.h"

#include "processes/ProcessesView.h"

#include "utilization/gpu/GPUUtilizationContainer.h"
#include "utilization/memory/MemoryUtilizationContainer.h"

#include "core/InfoProvider.h"

#include "resources/MainWindowResources.h"
#include "resources/QSplitterResources.h"

#include "SettingsDialog.h"

namespace {
namespace SessionSettings {
constexpr auto MainWindow_geometry = "MainWindow/geometry";
constexpr auto MainWindow_windowState = "MainWindow/windowState";
constexpr auto MainWindow_utilizationSplitter = "MainWindow/utilizationSplitter";
}

constexpr auto processesViewName = "processesView";
constexpr auto gpuUtilizationContainerName = "gpuUtilizationContainer";
constexpr auto memoryUtilizationContainerName = "memoryUtilizationContainer";
constexpr auto utilizationSplitterName = "utilizationSplitter";
}

using namespace SessionSettings;

MainWindow::MainWindow(QWidget*) {
    setWindowTitle("NVIDIA System Monitor");
    setWindowIcon(QIcon(ICON_PATH));

    const auto layout = new QVBoxLayout{};
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    // menu bar
    const auto menuBar = new QMenuBar{};

    const auto fileMenu = new QMenu{"&File"};
    fileMenu->addAction("&Settings", this, SLOT(settings()));
    fileMenu->addSeparator();
    fileMenu->addAction("&Exit", this, SLOT(quit()));

    const auto helpMenu = new QMenu{"&Help"};
    helpMenu->addAction("&About NVSM", &MainWindow::about, Qt::CTRL + Qt::Key_A);
    helpMenu->addAction("&About Qt", &QApplication::aboutQt, Qt::CTRL + Qt::Key_Q);
    helpMenu->addSeparator();
    helpMenu->addAction("&Help", &MainWindow::help, Qt::CTRL + Qt::Key_H);

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(helpMenu);

    // tabs
    const auto processes = new ProcessesView{};
    processes->setObjectName(processesViewName);

    const auto gpuUtilizationContainer = new GPUUtilizationContainer{};
    gpuUtilizationContainer->setObjectName(gpuUtilizationContainerName);

    const auto memoryUtilizationContainer = new MemoryUtilizationContainer{};
    memoryUtilizationContainer->setObjectName(memoryUtilizationContainerName);

    const auto utilizationSplitter = new QSplitter{};
    utilizationSplitter->setObjectName(utilizationSplitterName);
    utilizationSplitter->setOrientation(Qt::Vertical);
    utilizationSplitter->setStyleSheet(getSplitterStylesheet());
    utilizationSplitter->addWidget(gpuUtilizationContainer);
    utilizationSplitter->addWidget(memoryUtilizationContainer);

    const auto utilizationLayout = new QVBoxLayout{};
    utilizationLayout->addWidget(utilizationSplitter);
    utilizationLayout->setContentsMargins(32, 32, 32, 32);

    const auto utilizationWidget = new QWidget{};
    utilizationWidget->setLayout(utilizationLayout);

    const auto utWidgetScroll = new QScrollArea{};
    utWidgetScroll->setWidget(utilizationWidget);
    utWidgetScroll->setWidgetResizable(true);

    // add tabs
    const auto tabs = new QTabWidget{};
    tabs->addTab(processes, "Processes");
    tabs->addTab(utWidgetScroll, "Utilization");

    layout->addWidget(menuBar);
    layout->addWidget(tabs);
    
    const auto window = new QWidget{};
    window->setLayout(layout);
    setCentralWidget(window);

    workerThread.workers[0] = InfoProvider::getWorker();
    workerThread.start();

    // load MainWindow settings
    loadSettings();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    quit();
}

QString MainWindow::getSplitterStylesheet() {
    QColor background{palette().color(QPalette::Window)};
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

    QString splitterStylesheet{Resources::QSplitter::Stylesheet};
    splitterStylesheet.replace("/* QSplitter_border_color */", splitterColor.name());
    splitterStylesheet.replace("/* QSplitter_hover_border_color */", hoverSplitterColor.name());
    splitterStylesheet.replace("/* QSplitter_pressed_border_color */", pressedSplitterColor.name());

    return splitterStylesheet;
}

void MainWindow::saveSettings() {
    QSettings settings{NVSM_SETTINGS};
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
    const QSettings settings{NVSM_SETTINGS};

    const auto geometryByteArray = settings.value(MainWindow_geometry).toByteArray();

    if (!geometryByteArray.isEmpty()) {
        restoreGeometry(geometryByteArray);
    } else {
        resize(defaultWindowSize(QGuiApplication::primaryScreen()->geometry()));
    }

    restoreState(settings.value(MainWindow_windowState).toByteArray());
    findChild<QSplitter*>(utilizationSplitterName)->restoreState(settings.value(MainWindow_utilizationSplitter).toByteArray());
}

void MainWindow::quit() {
    hide();

    saveSettings();

    workerThread.running = false;
    workerThread.wait(); // waiting for all workers to be safely removed

    InfoProvider::destroy();

    QApplication::quit();
}

void MainWindow::settings() {
    const auto oldGraphLength = SettingsManager::getGraphLength();

    SettingsDialog dialog;
    dialog.exec();

    const auto gpuUtilizationContainer = findChild<GPUUtilizationContainer*>(gpuUtilizationContainerName);
    const auto memoryUtilizationContainer = findChild<MemoryUtilizationContainer*>(memoryUtilizationContainerName);

    gpuUtilizationContainer->updateLegend();
    memoryUtilizationContainer->updateLegend();

    // we need to reset graphs if graph length has been changed
    if (oldGraphLength != SettingsManager::getGraphLength()) {
        auto resetGraph = [](UtilizationContainer *container) {
            for (int i = 0; i < InfoProvider::getGPUCount(); i++) {
                container->getUtilizationWidget()->graphPoints[i] = {};
            }
        };

        QMutexLocker locker(&InfoProvider::getWorker()->mutex);
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
