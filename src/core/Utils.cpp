#include "Utils.h"

#include <stdexcept>
#include <chrono>
#include <sstream>

#define BUFFER_SIZE 256

using namespace std;

namespace Utils {
string exec(const string &cmd) {
    char buffer[BUFFER_SIZE];
    string result;
    FILE* pipe = popen(cmd.c_str(), "r");

    if (!pipe)
        throw runtime_error("popen() failed!");

    try {
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

// TODO: maybe use regex instead?

Iterator range(const string& line, const string &key, const size_t& n) {
    size_t pos = 0;
    Iterator it { string::npos, string::npos };

    if ((it.begin = line.find(key, n)) == string::npos)
        return it;

    it.end = it.begin + key.length();

    while ((pos = line.find(key, it.end)) != string::npos) {
        if (pos == it.end) {
            it.end = pos + key.length();
        } else {
            return it;
        }
    }

    return it;
}

vector<string> split(string in, const string& delimiter) {
    vector<string> out;
    size_t pos = 0;
    string token;

    while ((pos = in.find(delimiter)) != string::npos) {
        token = in.substr(0, pos);
        out.push_back(token);
        in.erase(0, pos + delimiter.length());
    }

    out.push_back(in);

    return out;
}

string streamline(const string& in) {
    vector<string> lines = split(in, "\n");
    Iterator it {};

    for (string &line : lines) {
        it = range(line, " ");
        if (it.begin == 0)
            line.erase(0, it.end);
        it.end = 0;

        while ((it = range(line, " ", it.end)).begin != string::npos) {
            if ((it.end - it.begin) > 1) {
                line.erase(it.begin, it.end - it.begin - 1);
                it.end = 0; // because we call erase
            }
        }
    }

    string s;
    for (const string &line : lines)
        s += line + "\n";

    return s;
}

long getTime() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

string toString(const float val, const int n) {
    ostringstream out;
    out.precision(n);
    out << fixed << val;
    return out.str();
}

size_t startsWith(const vector<string> &lines, const string &s) {
    size_t tmp;

    for (size_t i = 0; i < lines.size(); i++) {
        tmp = lines[i].find(s);

        if (tmp == 0) {
            return i;
        }
    }

    return string::npos;
}
}
