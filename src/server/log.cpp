#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "log.hpp"

void Log::get_time() {
    // TODO: handle errors here
    time_t timer;
    struct tm *tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);
    this->pos += strftime(this->buf+pos, 9, "%H:%M:%S", tm_info);
}

void Log::println(int color, const char *func, const char *format, ...) {
    this->mtx.lock();
    va_list args;
    va_start(args, format);
    this->pos = 0;
    this->pos = sprintf(this->buf+pos, "\033[1;%dm[", color);
    this->get_time();
    this->pos += sprintf(this->buf+pos, "]\033[0m ");
    this->pos += sprintf(this->buf+pos, "\033[2;35m{%s}\033[0m \033[1;36m<%d>\033[0m ", func, getpid());
    this->pos += vsprintf(this->buf+pos, format, args);
    this->buf[pos] = '\n';
    write(STDOUT_FILENO, this->buf, this->pos+1);
    va_end(args);
    this->mtx.unlock();
}
