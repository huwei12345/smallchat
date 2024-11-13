#include "FileTools.h"
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
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

void FileTools::deleteDirectory(const char *dirPath) {
    DIR *dir = opendir(dirPath);
    struct dirent *entry;
    
    if (dir == NULL) {
        perror("无法打开目录");
        return;
    }

    // 遍历目录中的每个文件和子目录
    while ((entry = readdir(dir)) != NULL) {
        // 跳过 "." 和 ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 构建完整路径
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirPath, entry->d_name);

        // 如果是目录，递归删除子目录
        if (entry->d_type == DT_DIR) {
            deleteDirectory(path);
        } else {
            // 如果是文件，删除文件
            if (remove(path) == 0) {
                printf("删除文件: %s\n", path);
            } else {
                perror("删除文件失败");
            }
        }
    }

    // 关闭目录
    closedir(dir);

    // 删除空目录
    if (rmdir(dirPath) == 0) {
        printf("删除空目录: %s\n", dirPath);
    } else {
        perror("删除目录失败");
    }
}

int FileTools::eraseFile(const char *path) {
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
        printf("has no file %s\n", path);
        return -2;
    }
    //大于1G
    if (S_ISDIR(st.st_mode)) {
        printf("删除路径是一个目录%s\n", realPath);
        deleteDirectory(realPath);
    } else if (S_ISREG(st.st_mode)) {
        printf("删除路径是一个文件%s\n", realPath);
        remove(realPath);
    } else {
        printf("删除路径既不是文件也不是目录。\n");
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