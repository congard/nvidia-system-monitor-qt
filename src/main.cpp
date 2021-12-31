#include <QApplication>
#include <QMessageBox>

#include <iostream>

#include "MainWindow.h"
#include "core/SettingsManager.h"
#include "core/Utils.h"
#include "core/InfoProvider.h"

using namespace std;

void init() {
    using namespace Utils;

    cout << "Connecting to nvidia-smi...\n";

    if (system("which nvidia-smi > /dev/null 2>&1")) {
        cout << "nvidia-smi not found. Are you have NVIDIA drivers?\n";
        QMessageBox::critical(nullptr, "Critical", "nvidia-smi not found. Are you have NVIDIA drivers?");
        exit(EXIT_FAILURE);
    } else {
        QString nvsmi_out = exec("nvidia-smi");

        if (linesStartsWith(nvsmi_out.split("\n"), "NVIDIA-SMI has failed") != -1) {
            cout << "nvidia-smi was found, but " << qPrintable(nvsmi_out);
            QMessageBox::critical(nullptr, "Critical",
                    nvsmi_out + "If you using laptop with discrete NVIDIA GPU, launch this app with optirun");
            exit(EXIT_FAILURE);
        }
    }

    SettingsManager::init();
    SettingsManager::load();

    InfoProvider::init();

    cout << "GPU Count is " << SettingsManager::getGPUCount() << "\n";
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    init();

    MainWindow w;
    w.show();

    return QApplication::exec();
}
