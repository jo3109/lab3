/*
* 레코드 잠금 프로그램
* 파일 이름 : lock2.c
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define THIS_PROCESS 2
#define THAT_PROCESS 1

int main() {
    int fd;
    struct flock testlock;
    int len;
    char buf[20];

    /* 읽기 잠금의 인자 지정 */
    testlock.l_type = F_RDLCK;
    testlock.l_whence = SEEK_SET;
    testlock.l_start = 0;
    testlock.l_len = 0;

    /* 파일 열기 */
    fd = open("testlock", O_RDWR);
    if (fd == -1) {
        perror("파일 열기 실패");
        exit(1);
    }

    /* 잠금 설정 */
    if (fcntl(fd, F_SETLKW, &testlock) == -1) {
        fprintf(stderr, "프로세스 %d: 잠금 실패\n", THIS_PROCESS);
        exit(1);
    }

    printf("프로세스 %d: 잠금 성공\n", THIS_PROCESS);

    /* 파일 읽기 */
    len = read(fd, buf, sizeof(buf) - 1);
    if (len == -1) {
        perror("파일 읽기 실패");
        close(fd);
        exit(1);
    }

    buf[len] = '\0';  // 널 종단 추가
    printf("프로세스 %d: 읽은 내용 \"%s\" \n", THIS_PROCESS, buf);

    printf("프로세스 %d: 잠금 해제\n", THIS_PROCESS);

    return 0;
}
