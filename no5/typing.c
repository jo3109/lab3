/*
* 타자 연습 프로그램
* 파일 이름 : typing.c
*/

#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(void)
{
    int fd;
    int nread, cnt = 0, errcnt = 0;
    char ch;
    char *text[] = {
        "How was your day?",
        "I need you to help me.",
        "I like listening to music."
    };
    int num_sentences = sizeof(text) / sizeof(text[0]);  // 문장 개수
    struct termios init_attr, new_attr;
    time_t start_time, end_time;  // 시간 측정을 위한 변수

    fd = open(ttyname(fileno(stdin)), O_RDWR);

    tcgetattr(fd, &init_attr);

    new_attr = init_attr;

    new_attr.c_lflag &= ~ICANON;

    new_attr.c_lflag &= ~ECHO; /* ~(ECHO | ECHOE | ECHOK | ECHONL); */

    new_attr.c_cc[VMIN] = 1;

    new_attr.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
        fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
    }

    // 타이핑 시작 시간을 기록
    start_time = time(NULL);

    printf("다음 문장을 받아쓰시오.");

    // 각 문장을 차례대로 입력받기
    for (int i = 0; i < num_sentences; i++) {
        printf("\n%s\n", text[i]);

        cnt = 0;  // 문장별 카운트 초기화

        while ((nread = read(fd, &ch, 1)) > 0 && ch != '\n') {
            if (ch == text[i][cnt++])
                write(fd, &ch, 1);
            else {
                write(fd, "*", 1);
                errcnt++;
            }
        }
	printf("\n");
    }

    // 타이핑 종료 시간을 기록
    end_time = time(NULL);

    // 경과 시간과 타이핑된 문자 수로 분당 타수를 계산
    double elapsed_time = difftime(end_time, start_time);  // 타이핑에 걸린 시간 (초 단위)
    double wpm = (cnt / 5.0) / (elapsed_time / 60.0);  // 분당 타수 (WPM)

    printf("\n타이핑 오류의 횟수는 %d\n", errcnt);
    printf("분당 타수: %.2f 타수\n", wpm);

    tcsetattr(fd, TCSANOW, &init_attr);

    close(fd);

    return 0;
}
