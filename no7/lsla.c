/*
* ls -la 연습 프로그램
* 프로그램 이름 : lsla.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

// 사용자 정의 system 함수
int system(const char *command) {
    if (command == NULL) {
        return -1; // 잘못된 입력
    }

    pid_t pid = fork(); // 자식 프로세스 생성
    if (pid == 0) {
        // 자식 프로세스에서 명령 실행
        char *argv[50];
        char *cmd_copy = strdup(command); // 문자열 수정 방지
        int narg = 0;

        // 명령어를 공백 기준으로 분리
        char *token = strtok(cmd_copy, " ");
        while (token != NULL && narg < 50) {
            argv[narg++] = token;
            token = strtok(NULL, " ");
        }
        argv[narg] = NULL;

        execvp(argv[0], argv); // 명령 실행
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // 부모 프로세스에서 자식 프로세스가 종료될 때까지 대기
        int status;
        waitpid(pid, &status, 0);
        return status;
    } else {
        perror("fork failed");
        return -1;
    }
}

int main() {
    printf("현재 디렉토리의 파일을 나열합니다:\n");
    system("ls -la");

    return 0;
}
