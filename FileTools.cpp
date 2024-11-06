#include "FileTools.h"
#include <errno.h>
int FileTools::createDirectory(const char *path)
{
    if (path == NULL || strlen(path) >= 1000) {
        return -1;
    }
    // 分割路径字符串
    char realPath[500];
    strcpy(realPath, path);
    if (path[0] != '/' && path[0] != '.') {
        sprintf(realPath, "/home/huwei/ftp/%s", path);
        printf("realPath = %s\n", realPath);
    }

    printf("path = %s\n", realPath);
    char pathStr[1000];
    char pathSub[1000];
    struct stat st;
    memset(pathSub, 0, 1000);
    strcpy(pathStr, realPath);
    char *token = pathStr;
    char *saveptr = NULL;
    // 第一次调用 strtok_r，传入完整字符串
    token = strtok_r(token, "/", &saveptr);
    sprintf(pathSub, "/%s%s/", pathSub, token);
    printf("pathSub = %s\n", pathSub);
    while (token != NULL) {
        // 接下来调用 strtok_r，传入 NULL
        token = strtok_r(NULL, "/", &saveptr);
        //重新构建当前路径
        if (token == NULL) {
            break;
        }
        sprintf(pathSub, "%s%s/", pathSub, token);
        // //检查目录是否已存在
        if (stat(pathSub, &st) == -1) {
            // 如果目录不存在，创建它
            if (mkdir(pathSub, 0755) == -1) {
                printf("mkdir failed %s  %d\n", pathSub, errno);
                return -1; // 创建目录失败
            }
            printf("Created directory: %s\n", pathSub);
        }
    }

    return 0; // 成功
}

int FileTools::checkFile(const char *path)
{
    if (path == NULL || strlen(path) <= 1) {
        return -1;
    }
    char realPath[500];
    strcpy(realPath, path);
    if (path[0] != '/' && path[0] != '.') {
        sprintf(realPath, "/home/huwei/ftp/%s", path);
        printf("realPath = %s\n", realPath);
    }
    struct stat st;
    if (stat(realPath, &st) == -1) {
        return -2;
    }
    else {
        //大于1G
        if (st.st_size > 1024 * 1024 * 1024) {
            return -3;
        }
    }
    return 0;
}

// int main() {
//     char str[] = "a,b,c,d";
//     char *token;
//     char *saveptr;

//     // 第一次调用 strtok_r，传入完整字符串
//     token = strtok_r(str, ",", &saveptr);
//     while (token != NULL) {
//         printf("%s\n", token); // 输出分割的子串
//         // 接下来调用 strtok_r，传入 NULL
//         token = strtok_r(NULL, ",", &saveptr);
//     }

//     return 0;
// }