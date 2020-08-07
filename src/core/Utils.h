#ifndef QNVSM_UTILS_H
#define QNVSM_UTILS_H

#include <string>
#include <vector>

namespace Utils {
struct Iterator {
    size_t begin, end;
};

std::string exec(const std::string &cmd);
Iterator range(const std::string &line, const std::string &key, const size_t &n = 0);
std::vector<std::string> split(std::string in, const std::string &delimiter);
std::string streamline(const std::string &in);
std::string toString(float val, int n = 1);
size_t startsWith(const std::vector<std::string> &lines, const std::string &s);
long getTime();
}

#endif
