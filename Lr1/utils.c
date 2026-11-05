#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int read_line(int fd, char *buf, int size) {
    if (size <= 0) return -1;
    int count = 0;
    while (count < size - 1) {
        char c;
        ssize_t r = read(fd, &c, 1);
        if (r < 0) return -1;
        if (r == 0) break;
        buf[count++] = c;
        if (c == '\n') break;
    }
    buf[count] = '\0';
    return count;
}

int write_str(int fd, const char *str) {
    int len = 0;
    while (str[len] != '\0') len++;
    if (len == 0) return 0;
    ssize_t written = write(fd, str, len);
    return (written >= 0) ? (int)written : -1;
}

int float_to_str(float val, char *buf, int buf_size) {
    if (buf_size <= 10) return -1;
    int pos = 0;
    if (val < 0) {
        buf[pos++] = '-';
        val = -val;
    }
    int int_part = (int)val;
    if (int_part == 0) {
        buf[pos++] = '0';
    } else {
        char temp[20];
        int temp_pos = 0;
        while (int_part > 0) {
            temp[temp_pos++] = '0' + (int_part % 10);
            int_part /= 10;
        }
        for (int i = temp_pos - 1; i >= 0; i--) {
            buf[pos++] = temp[i];
        }
    }
    buf[pos++] = '.';
    float frac = val - (int)val;
    for (int i = 0; i < 2; i++) {
        frac *= 10;
        int digit = (int)frac;
        buf[pos++] = '0' + digit;
        frac -= digit;
    }
    buf[pos] = '\0';
    return pos;
}

float parse_floats(const char *line) {
    float sum = 0.0f;
    const char *ptr = line;
    while (*ptr != '\0' && *ptr != '\n') {
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        if (*ptr == '\0' || *ptr == '\n') break;
        char *end;
        double val = strtod(ptr, &end);
        if (end == ptr) break;
        sum += (float)val;
        ptr = end;
    }
    return sum;
}