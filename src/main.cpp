#include <QApplication>
#include <QMessageBox>

#include <iostream>

#include "MainWindow.h"
#include "Settings.h"
#include "core/Utils.h"
#include "core/NVSMIParser.h"

using namespace std;

void init() {
    using namespace Utils;

    cout << "Connecting to nvidia-smi...\n";

    if (system("which nvidia-smi > /dev/null 2>&1")) {
        cout << "nvidia-smi not found. Are you have NVIDIA drivers?\n";
        QMessageBox::critical(nullptr, "Critical", "nvidia-smi not found. Are you have NVIDIA drivers?");
        exit(EXIT_FAILURE);
    } else {
        string nvsmi_out = exec("nvidia-smi");

        if (startsWith(split(nvsmi_out, "\n"), "NVIDIA-SMI has failed") != string::npos) {
            cout << "nvidia-smi was found, but " << nvsmi_out;
            QMessageBox::critical(nullptr, "Critical",
                    (nvsmi_out + "If you using laptop with discrete NVIDIA GPU, launch this app with optirun").c_str());
            exit(EXIT_FAILURE);
        }
    }

    Settings::load();
    cout << "GPU Count is " << Settings::GPUCount << "\n";
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    init();

    MainWindow w;
    w.show();

    return QApplication::exec();
}
