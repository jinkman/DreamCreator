#ifndef COMMON_H
#define COMMON_H

#include <Config.h>
#include <string>

namespace DM {
std::string getLocalPath(const std::string &path) {
    return std::string(DATA_PATH) + path;
}

} // namespace DM

#endif // COMMON_H