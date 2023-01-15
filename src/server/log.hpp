#pragma once

#include <mutex>

#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34

#define PPRINTF(LOGGER, COLOR, FORMAT, args...) ({\
LOGGER.println(COLOR, __func__, FORMAT, ##args);\
})

class Log {
    void get_time();
    char *buf;
    int pos;
    std::mutex mtx;
public:
    Log();
    void println(int color, const char *func, const char *format, ...);
};
