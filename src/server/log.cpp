#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "log.hpp"

#define DEFAULT_BUF_SIZE 2048

void Log::get_time() {
    // TODO: handle errors here
    time_t timer;
    struct tm *tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);
    this->pos += strftime(this->buf+pos, 9, "%H:%M:%S", tm_info);
}

Log::~Log() {
    delete [] this -> buf;
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

    int write_size = vsnprintf(NULL, 0, format, args) + pos; 
    va_end(args);
    va_start(args, format);
    char *new_ptr;
    if (write_size >= DEFAULT_BUF_SIZE) {
        new_ptr = (char*)std::realloc(this->buf, write_size);
        if (new_ptr == NULL) {
            this->pos += sprintf(this->buf+pos, "Reallocation failed, the message could not be displayed\n");
        } else {
            this->buf = new_ptr;
        }
    }

    this->pos += vsprintf(this->buf+pos, format, args);
    va_end(args);
    this->buf[this->pos] = '\n';
    write(STDOUT_FILENO, this->buf, this->pos+1);
    if (write_size >= DEFAULT_BUF_SIZE) {
        new_ptr = (char*)std::realloc(this->buf, DEFAULT_BUF_SIZE);
        if (new_ptr == NULL) {
            perror("How did that happen?");
        } else {
            this->buf = new_ptr;
        }
    }
    this->mtx.unlock();
}

Log::Log() {
    this->buf = new char[DEFAULT_BUF_SIZE];
}
