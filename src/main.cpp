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
        cout << "nvidia-smi not found. Have you installed NVIDIA drivers?\n";
        QMessageBox::critical(nullptr, "Critical", "nvidia-smi not found. Have you installed NVIDIA drivers?");
        exit(EXIT_FAILURE);
    } else {
        QString nvsmi_out = exec("nvidia-smi");

        if (linesStartsWith(nvsmi_out.split("\n"), "NVIDIA-SMI has failed") != -1) {
            cout << "nvidia-smi was found, but " << qPrintable(nvsmi_out);
            QMessageBox::critical(nullptr, "Critical",
                    nvsmi_out + "Possible solution: If you are using a laptop with discrete NVIDIA GPU, launch this program with optirun");
            exit(EXIT_FAILURE);
        }
    }

    InfoProvider::init();

    SettingsManager::load();

    cout << "GPU Count is " << InfoProvider::getGPUCount() << "\n";
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    init();

    MainWindow w;
    w.show();

    return QApplication::exec();
}
