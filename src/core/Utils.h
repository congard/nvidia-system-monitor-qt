#ifndef QNVSM_UTILS_H
#define QNVSM_UTILS_H

#include <QString>

#ifdef FLATPAK_BUILD
#define CMD_PREFIX "flatpak-spawn --host "
#else
#define CMD_PREFIX ""
#endif

#define exec_cmd(cmd) exec(CMD_PREFIX cmd)
#define exec_cmd_s(cmd) exec(CMD_PREFIX + (cmd))

namespace Utils {
QString exec(const QString &cmd);
int linesStartsWith(const QStringList &lines, const QString &str);
long getTime();
}

#endif
