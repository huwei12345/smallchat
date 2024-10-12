#include "../FileTools.h"

int main() {
    if (FileTools::createDirectory("./a/b/c") != 0) {
        printf("error create\n");
    }
}