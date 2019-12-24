#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "settings.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <pwd.h>

// set to default
uint UPDATE_DELAY = 2000; // 2 sec
uint GRAPH_LENGTH = 60000; // 60 sec
int GPU_COUNT = -1;

QColor gpuColors[8] = {
    _c(0, 255, 0),
    _c(0, 0, 255),
    _c(255, 0, 0),
    _c(255, 255, 0),
    _c(255, 0, 255),
    _c(0, 255, 255),
    _c(255, 255, 255),
    _c(32, 32, 32)
};

void init() {
    std::cout << "Connecting to nvidia-smi...\n";
    if (system("which nvidia-smi > /dev/null 2>&1")) {
        std::cout << "nvidia-smi not found. Are you have NVIDIA drivers?\n";
        QMessageBox::critical(nullptr, "Critical", "nvidia-smi not found. Are you have NVIDIA drivers?");
        exit(EXIT_FAILURE);
    } else {
        std::string nvsmi_out = exec("nvidia-smi");
        if (startsWith(split(nvsmi_out, "\n"), "NVIDIA-SMI has failed") != std::string::npos) {
            std::cout << "nvidia-smi was found, but " << nvsmi_out;
            QMessageBox::critical(nullptr, "Critical", (nvsmi_out + "If you using laptop with discrete NVIDIA GPU, launch this app with optirun").c_str());
            exit(EXIT_FAILURE);
        }
    }

    std::cout << "Loading settings\n";

    GPU_COUNT = atoi(split(exec(NVSMI_CMD_GPU_COUNT), "\n")[1].c_str());
    std::cout << "GPU Count is " << GPU_COUNT << "\n";

    std::string path = getpwuid(getuid())->pw_dir;
    path += "/.config/nvidia-system-monitor/config";

    std::ifstream conf_stream(path);
    if (!conf_stream.good()) {
        std::cout << "Config file not found\n";
        return;
    }
    std::string conf((std::istreambuf_iterator<char>(conf_stream)), std::istreambuf_iterator<char>());
    conf_stream.close();

    std::vector<std::string> lines = split(conf, "\n");

    size_t lineIndex;
    if ((lineIndex = startsWith(lines, NVSM_CONF_UPDATE_DELAY)) != std::string::npos) {
        UPDATE_DELAY = atoi(split(streamline(lines[lineIndex]), " ")[1].c_str());
        lines.erase(lines.begin() + lineIndex);
    }
    if ((lineIndex = startsWith(lines, NVSM_CONF_GRAPH_LENGTH)) != std::string::npos) {
        GRAPH_LENGTH = atoi(split(streamline(lines[lineIndex]), " ")[1].c_str());
        lines.erase(lines.begin() + lineIndex);
    }

    lineIndex = 0;
    while (lineIndex != std::string::npos) {
        if ((lineIndex = startsWith(lines, NVSM_CONF_GCOLOR)) != std::string::npos) {
            std::vector<std::string> line = split(streamline(lines[lineIndex]), " ");
            gpuColors[atoi(line[1].c_str())] = _c(atoi(line[2].c_str()), atoi(line[3].c_str()), atoi(line[4].c_str()));
            lines.erase(lines.begin() + lineIndex);
        }
    }

    std::cout << "Done\n";
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    init();

    MainWindow w;
    w.resize(764, 700);
    w.setWindowTitle("NVIDIA System Monitor");
    w.show();

    return QApplication::exec();
}
