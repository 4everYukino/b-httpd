#include "utils.h"

#include <fstream>
#include <string.h>

#include <boost/algorithm/string.hpp>

#include <spdlog/spdlog.h>

#define LINUX_CPU_INFO "/proc/cpuinfo"

int get_threads_count()
{
    int ret = 0;

#ifdef __linux__

    std::fstream f(LINUX_CPU_INFO);
    if (!f.is_open()) {
        spdlog::error("Failed to open '{}', error: {}.",
                      LINUX_CPU_INFO, strerror(errno));
        return ret;
    }

    std::string line;
    while (getline(f, line)) {
        if (boost::contains(line, "processor"))
            ++ret;
    }

    f.close();

#endif

    return ret;
}
