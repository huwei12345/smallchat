#include "FileTools.h"

int FileTools::createDirectory(const char *path)
{
    if (strlen(path) >= 1000) {
        return -1;
    }
    // 分割路径字符串

    char pathStr[1000];
    char pathSub[1000];
    struct stat st;
    memset(pathSub, 0, 1000);
    strcpy(pathStr, path);
    char *token = pathStr;
    char *saveptr = NULL;
    // 第一次调用 strtok_r，传入完整字符串
    token = strtok_r(token, "/", &saveptr);
    sprintf(pathSub, "%s%s/", pathSub, token);
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
                perror("mkdir failed");
                return -1; // 创建目录失败
            }
            printf("Created directory: %s\n", pathSub);
        }
    }



    // char *pchSrc = "./a/b/c";
	// char chBuffer[102] ;
	// char *pchDilem = "/";
	// char *saveptr = NULL;
	// char *token = NULL;
	// strncpy(chBuffer,pchSrc ,sizeof(chBuffer) - 1);
	// token = chBuffer;
    // token = strtok_r( token, "/", &saveptr);
	// while(NULL != ( token = strtok_r(NULL, "/", &saveptr) ))
	// {
	// 	printf("\n token[%s] saveptr[%s]\n",token,saveptr);
	// 	token = NULL;
	// }

        // 在当前路径上添加子目录
        // if (strlen(subpath) == 0) {
        //     continue; // 跳过空部分
        // } 

    //}
    
    return 0; // 成功
}

int FileTools::checkFile(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1) {
        return -1;
    }
    else {
        //大于1G
        if (st.st_size > 1024 * 1024 * 1024) {
            return -2;
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