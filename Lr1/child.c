#include "utils.h"
#include <unistd.h>

int main() {
    char buf[1024];
    while (read_line(STDIN_FILENO, buf, sizeof(buf)) > 0) {
        float sum = parse_floats(buf);
        char out_buf[128];
        int len = float_to_str(sum, out_buf, sizeof(out_buf));
        out_buf[len] = '\n';
        out_buf[len + 1] = '\0';
        write_str(STDOUT_FILENO, out_buf);
    }
    return 0;
}