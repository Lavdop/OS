#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

void read_random_bytes(void *buffer, size_t size);
uint64_t get_time_ns(void);
void write_string(const char *str);
void write_int(int64_t value);
int64_t parse_int(const char *str);

#endif