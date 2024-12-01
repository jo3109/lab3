/*
* 파일, 디렉토리 나열 프로그램
* 파일 이름 : list_f_dir.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

void list_directory(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    printf("\n%s:\n", path);  // 현재 디렉토리 이름 출력

    while ((entry = readdir(dp)) != NULL) {
        // '.'와 '..'를 제외한 파일만 처리
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        printf("%s\n", entry->d_name);

        // 디렉토리일 경우 재귀적으로 방문
        if (entry->d_type == DT_DIR) {
            char new_path[1024];
            snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
            list_directory(new_path);
        }
    }

    closedir(dp);
}

int main(int argc, char *argv[]) {
    const char *start_path = ".";  // 실행된 경로를 기본으로 설정

    list_directory(start_path);

    return 0;
}
