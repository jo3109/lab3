/*
* 레코드 잠금 예제 프로그램
* 파일 이름 : lock1.c
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define THIS_PROCESS 1
#define THAT_PROCESS 2

int main() {
    int fd;
    struct flock testlock;
    char buf[20];

    /* 쓰기 잠금의 인자 지정 */
    testlock.l_type = F_WRLCK;
    testlock.l_whence = SEEK_SET;
    testlock.l_start = 0;
    testlock.l_len = 0;

    /* 파일 열기 */
    fd = open("testlock", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("파일 열기 실패");
        exit(1);
    }

    if (fcntl(fd, F_SETLKW, &testlock) == -1) {
        fprintf(stderr, "프로세스 %d: 잠금 실패\n", THIS_PROCESS);
        exit(1);
    }

    printf("프로세스 %d: 잠금 성공\n", THIS_PROCESS);

    sprintf(buf, "Hello, process %d", THAT_PROCESS);
    write(fd, buf, 17);

    printf("프로세스 %d: \"%s\" 내용을 testlock 파일에 기록\n", THIS_PROCESS, buf);

    sleep(5);

    printf("프로세스 %d: 잠금 해제\n", THIS_PROCESS);
    return 0;
}
