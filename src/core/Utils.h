#ifndef QNVSM_UTILS_H
#define QNVSM_UTILS_H

#include <QString>

namespace Utils {
QString exec(const QString &cmd);
int linesStartsWith(const QStringList &lines, const QString &str);
long getTime();
}

#endif
