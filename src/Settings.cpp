#include "Settings.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <pwd.h>

#include "core/NVSMIParser.h"
#include "core/Utils.h"

using namespace std;
using namespace Utils;

#define NVSM_CONF_UPDATE_DELAY "updateDelay"
#define NVSM_CONF_GRAPH_LENGTH "graphLength"
#define NVSM_CONF_GCOLOR "gpuColor"

namespace Settings {
QVarLengthArray<QColor> GPUColors {};
QVarLengthArray<QString> GPUNames {};
uint UpdateDelay = 2000; // 2 sec
uint GraphLength = 60000; // 60 sec
int GPUCount = 0;

void load() {
    QColor defaultGPUColors[8] = {
            {0, 255, 0},
            {0, 0, 255},
            {255, 0, 0},
            {255, 255, 0},
            {255, 0, 255},
            {0, 255, 255},
            {255, 255, 255},
            {32, 32, 32}
    };

    NVSMIParser::init();
    GPUCount = NVSMIParser::getGPUCount();
    GPUNames = NVSMIParser::getGPUNames();

    GPUColors.resize(GPUCount);

    for (int i = 0; i < GPUCount; i++)
        GPUColors[i] = defaultGPUColors[i % 8];

    // TODO: use QSettings instead & add GUI settings

    string path = getpwuid(getuid())->pw_dir;
    path += "/.config/nvidia-system-monitor/config";

    ifstream conf_stream(path);
    if (!conf_stream.good()) {
        cout << "Config file not found\n";
        return;
    }

    string std_conf((istreambuf_iterator<char>(conf_stream)), istreambuf_iterator<char>());
    conf_stream.close();

    QString conf = QString(std_conf.c_str()).replace(
            QRegularExpression(R"(^\h+|\h+$|[ \t]{2,})"), " ").toLocal8Bit().constData();

    auto lines = conf.split("\n");
    int lineIndex;

    if ((lineIndex = linesStartsWith(lines, NVSM_CONF_UPDATE_DELAY)) != -1) {
        Settings::UpdateDelay = lines[lineIndex].split(" ")[1].toInt();
        lines.erase(lines.begin() + lineIndex);
    }

    if ((lineIndex = linesStartsWith(lines, NVSM_CONF_GRAPH_LENGTH)) != -1) {
        Settings::GraphLength = lines[lineIndex].split(" ")[1].toInt();
        lines.erase(lines.begin() + lineIndex);
    }

    lineIndex = 0;
    while (lineIndex != string::npos) {
        if ((lineIndex = linesStartsWith(lines, NVSM_CONF_GCOLOR)) != -1) {
            auto line = lines[lineIndex].split(" ");

            Settings::GPUColors[line[1].toInt()] = {
                    line[2].toInt(),
                    line[3].toInt(),
                    line[4].toInt()
            };

            lines.erase(lines.begin() + lineIndex);
        }
    }
}
}
