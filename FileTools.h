#ifndef FILE_TOOLS_H
#define FILE_TOOLS_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

class FileTools {
public:
    static int createDirectory(const char *path);
    static int checkFile(const char* path);
};
#endif