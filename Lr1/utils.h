#ifndef UTILS_H
#define UTILS_H

int read_line(int fd, char *buf, int size);
int write_str(int fd, const char *str);
int float_to_str(float val, char *buf, int buf_size);
float parse_floats(const char *line);

#endif