#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>

#define MSG_KEY 1234  // 메시지 큐의 고유 키
#define MAX_MSG_SIZE 100
#define USER1_FILE "user1_chat_history.txt" // 사용자1 채팅 내역을 저장할 파일
#define USER2_FILE "user2_chat_history.txt" // 사용자2 채팅 내역을 저장할 파일

// 메시지 구조체
struct msg_buffer {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

// 사용자1의 채팅 내역을 파일에 저장하는 함수
void write_message_to_file(const char *msg) {
    FILE *file = fopen(USER1_FILE, "a");
    if (file == NULL) {
        perror("파일 열기 실패");
        return;
    }
    fprintf(file, "%s\n", msg);
    fclose(file);
}

// 사용자1의 채팅 내역을 파일에서 삭제하는 함수
void clear_chat_history() {
    FILE *file = fopen(USER1_FILE, "w"); // 'w' 모드는 파일을 열 때 기존 내용을 지움
    if (file == NULL) {
        perror("파일 열기 실패");
        return;
    }
    fclose(file);  // 파일을 열고 아무것도 쓰지 않으면 파일 내용이 지워짐
}

int main() {
    int msgid;
    struct msg_buffer message;
    char input[MAX_MSG_SIZE];
    
    // 메시지 큐 생성
    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    while (1) {
        printf("사용자1> ");
        fgets(input, MAX_MSG_SIZE, stdin);
        
        // 종료 명령어 처리
        if (strncmp(input, "exit", 4) == 0) {
            clear_chat_history();  // 채팅 내역 초기화
            printf("채팅을 종료합니다.\n");
            break;
        }

        // 메시지를 메시지 큐로 전송
        message.msg_type = 1;  // 메시지 타입
        strncpy(message.msg_text, input, MAX_MSG_SIZE);

        if (msgsnd(msgid, &message, sizeof(message), 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }

        // 사용자1의 채팅 내역 파일에 저장
        write_message_to_file(input);
    }

    // 메시지 큐 삭제
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }

    return 0;
}
