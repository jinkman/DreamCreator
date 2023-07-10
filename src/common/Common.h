#ifndef COMMON_H
#define COMMON_H

#include <Config.h>
#include <string>
#include <QString>

namespace DM {
static std::string getLocalPath(const std::string &path) {
    return std::string(DATA_PATH) + path;
}

static QString getQLocalPath(const QString &path) {
    return QString::fromStdString(DATA_PATH) + path;
}

} // namespace DM

#endif // COMMON_H