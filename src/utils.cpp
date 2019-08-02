#include "utils.h"

#include <stdexcept>
#include <chrono>
#include <sstream>

#define BUFFER_SIZE 256

std::string exec(const std::string& cmd) {
    char buffer[BUFFER_SIZE];
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

Iterator range(const std::string& line, const std::string key, const size_t& n) {
    size_t pos = 0;
    Iterator it;
    it.begin = it.end = std::string::npos;

    if ((it.begin = line.find(key, n)) == std::string::npos) return it;

    it.end = it.begin + key.length();

    while ((pos = line.find(key, it.end)) != std::string::npos) {
        if (pos == it.end) it.end = pos + key.length();
        else return it;
    }

    return it;
}

std::vector<std::string> split(std::string in, const std::string& delimiter) {
    std::vector<std::string> out;
    size_t pos = 0;
    std::string token;
    while ((pos = in.find(delimiter)) != std::string::npos) {
        token = in.substr(0, pos);
        out.push_back(token);
        in.erase(0, pos + delimiter.length());
    }
    out.push_back(in);
    return out;
}

std::string streamline(const std::string& in) {
    std::vector<std::string> lines = split(in, "\n");

    Iterator it;
    for (size_t i = 0; i < lines.size(); i++) {
        it = range(lines[i], " ");
        if (it.begin == 0) lines[i].erase(0, it.end);
        it.end = 0;

        while ((it = range(lines[i], " ", it.end)).begin != std::string::npos) {
            if ((it.end - it.begin) > 1) {
                lines[i].erase(it.begin, it.end - it.begin - 1);
                it.end = 0; // because we call erase
            }
        }
    }
    
    std::string s;
    for (size_t i = 0; i < lines.size(); i++)
        s += lines[i] + "\n";

    return s;
}

long getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string toString(const float val, const int n) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << val;
    return out.str();
}

size_t startsWith(const std::vector<std::string> &lines, const std::string &s) {
    size_t tmp;
    for (size_t i = 0; i < lines.size(); i++) {
        tmp = lines[i].find(s);
        if (tmp == 0)
            return i;
    }

    return std::string::npos;
}
