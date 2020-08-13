#include "Utils.h"

#include <QStringList>

#include <stdexcept>
#include <chrono>

#define BUFFER_SIZE 256

using namespace std;

namespace Utils {
QString exec(const QString &cmd) {
    QString result;
    FILE* pipe = popen(cmd.toLocal8Bit().constData(), "r");

    if (!pipe)
        throw runtime_error("popen() failed!");

    try {
        char buffer[BUFFER_SIZE];

        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }

    pclose(pipe);

    return result;
}

int linesStartsWith(const QStringList &lines, const QString &str) {
    for (int i = 0; i < lines.size(); i++)
        if (lines[i].startsWith(str))
            return i;

    return -1;
}

long getTime() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}
}
