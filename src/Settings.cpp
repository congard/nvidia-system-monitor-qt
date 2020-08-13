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
QVector<QColor> GPUColors {};
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

    for (int i = 0; i < GPUCount; i++)
        GPUColors.append(defaultGPUColors[i % 8]);

    // TODO: use QSettings instead & add GUI settings

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
                    {atoi(line[2].c_str()), atoi(line[3].c_str()), atoi(line[4].c_str())};
            lines.erase(lines.begin() + lineIndex);
        }
    }
}
}
