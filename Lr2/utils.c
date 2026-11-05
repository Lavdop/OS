#include "utils.h"
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

static size_t get_string_length(const char *str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

void read_random_bytes(void *buffer, size_t size) {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        write_string("Error: cannot open /dev/urandom\n");
        _exit(1);
    }
    ssize_t bytes_read = read(fd, buffer, size);
    if (bytes_read != (ssize_t)size) {
        write_string("Error: cannot read random bytes\n");
        close(fd);
        _exit(1);
    }
    close(fd);
}

uint64_t get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

void write_string(const char *str) {
    size_t len = get_string_length(str);
    write(STDOUT_FILENO, str, len);
}

void write_int(int64_t value) {
    char buffer[32];
    int pos = 0;
    
    if (value < 0) {
        buffer[pos++] = '-';
        value = -value;
    }
    
    if (value == 0) {
        buffer[pos++] = '0';
    } else {
        char temp[32];
        int temp_pos = 0;
        while (value > 0) {
            temp[temp_pos++] = '0' + (value % 10);
            value /= 10;
        }
        for (int i = temp_pos - 1; i >= 0; i--) {
            buffer[pos++] = temp[i];
        }
    }
    
    buffer[pos++] = '\n';
    write(STDOUT_FILENO, buffer, pos);
}

int64_t parse_int(const char *str) {
    int64_t result = 0;
    int negative = 0;
    
    if (*str == '-') {
        negative = 1;
        str++;
    }
    
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return negative ? -result : result;
}