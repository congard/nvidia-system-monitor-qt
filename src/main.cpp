#include <QApplication>
#include <QMessageBox>

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <pwd.h>

#include "MainWindow.h"
#include "Settings.h"
#include "core/Utils.h"

using namespace std;

// set to default
uint Settings::UpdateDelay = 2000; // 2 sec
uint Settings::GraphLength = 60000; // 60 sec
int Settings::GPUCount = -1;

#define colour(r, g, b) QColor(r, g, b)

QColor Settings::GPUColors[8] = {
    colour(0, 255, 0),
    colour(0, 0, 255),
    colour(255, 0, 0),
    colour(255, 255, 0),
    colour(255, 0, 255),
    colour(0, 255, 255),
    colour(255, 255, 255),
    colour(32, 32, 32)
};

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

    cout << "Loading settings\n";

    Settings::GPUCount = atoi(split(exec(NVSMI_CMD_GPU_COUNT), "\n")[1].c_str());
    cout << "GPU Count is " << Settings::GPUCount << "\n";

    string path = getpwuid(getuid())->pw_dir;
    path += "/.config/nvidia-system-monitor/config";

    ifstream conf_stream(path);
    if (!conf_stream.good()) {
        cout << "Config file not found\n";
        return;
    }

    string conf((istreambuf_iterator<char>(conf_stream)), istreambuf_iterator<char>());
    conf_stream.close();

    vector<string> lines = split(conf, "\n");
    size_t lineIndex;

    if ((lineIndex = startsWith(lines, NVSM_CONF_UPDATE_DELAY)) != string::npos) {
        Settings::UpdateDelay = atoi(split(streamline(lines[lineIndex]), " ")[1].c_str());
        lines.erase(lines.begin() + lineIndex);
    }

    if ((lineIndex = startsWith(lines, NVSM_CONF_GRAPH_LENGTH)) != string::npos) {
        Settings::GraphLength = atoi(split(streamline(lines[lineIndex]), " ")[1].c_str());
        lines.erase(lines.begin() + lineIndex);
    }

    lineIndex = 0;
    while (lineIndex != string::npos) {
        if ((lineIndex = startsWith(lines, NVSM_CONF_GCOLOR)) != string::npos) {
            vector<string> line = split(streamline(lines[lineIndex]), " ");
            Settings::GPUColors[atoi(line[1].c_str())] =
                    colour(atoi(line[2].c_str()), atoi(line[3].c_str()), atoi(line[4].c_str()));
            lines.erase(lines.begin() + lineIndex);
        }
    }

    cout << "Done\n";
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    init();

    MainWindow w;
    w.show();

    return QApplication::exec();
}
