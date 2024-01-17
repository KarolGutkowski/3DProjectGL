#ifndef REPORT_ERROR_H
#define REPORT_ERROR_H

#include <cstdio>
#include <cstdlib>

void report_and_exit(const char* information,const int line, const char* file);

#define err(text) report_and_exit(text, __LINE__, __FILE__)

#endif
