/*
* 파일 내용 추가 프로그램
* 파일 이름: fappend.c
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
int fd; /* file descriptor */

char *buf = "Nice to meet you.\n";

ssize_t cnt; /* write count */

int flags = O_WRONLY | O_CREAT | O_APPEND;

mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP |S_IROTH; /* == 0644 */

if (argc < 2) {
   fprintf(stderr, "Usage: file_appendfilename\n");
   exit(1);
}

if ( (fd = open(argv[1], flags, mode)) == -1 ){
   perror("open"); /* errno에 대응하는 메시지 출력됨 */
   exit(1);
}

cnt = write(fd, buf, strlen(buf));

printf("write count = %ld\n", cnt);

close(fd);

}
