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
    static void deleteDirectory(const char *dirPath);
    static int eraseFile(const char *path);
    static bool moveFile(const char* src, const char* dst);
};
#endif