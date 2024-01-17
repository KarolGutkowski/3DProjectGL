#include "report_error.h"

void report_and_exit(const char* information,const int line, const char* file) {
    fprintf(stderr, "ERROR[line: %d, file:%s]: %s\n", line, file, information);
    exit(EXIT_FAILURE);
}
