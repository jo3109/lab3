#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_KEY 1234  // 공유 메모리 키
#define SEM_KEY 5678  // 세마포어 키
#define MAX_BUF_SIZE 1024
#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"

// 세마포어 연산을 위한 구조체
struct sembuf sem_op;

// 세마포어 초기화 함수
void init_semaphore(int semid) {
    semctl(semid, 0, SETVAL, 1);  // 세마포어를 1로 설정하여 부모 프로세스가 먼저 실행
}

// 세마포어 P 연산(자식이 공유 메모리를 읽기 전에 대기)
void sem_wait(int semid) {
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;  // P 연산
    sem_op.sem_flg = 0;
    semop(semid, &sem_op, 1);
}

// 세마포어 V 연산(부모가 데이터를 쓴 후 자식에게 알려줌)
void sem_signal(int semid) {
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;  // V 연산
    sem_op.sem_flg = 0;
    semop(semid, &sem_op, 1);
}

int main() {
    // 공유 메모리와 세마포어 초기화
    int shmid = shmget(SHM_KEY, MAX_BUF_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char *shm_ptr = (char*) shmat(shmid, NULL, 0);
    if (shm_ptr == (char*) -1) {
        perror("shmat failed");
        exit(1);
    }

    int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget failed");
        exit(1);
    }

    init_semaphore(semid);

    pid_t pid = fork();  // 자식 프로세스 생성

    if (pid == 0) {
        // 자식 프로세스: 파일을 읽어서 공유 메모리로 복사
        FILE *input_file = fopen(INPUT_FILE, "r");
        if (input_file == NULL) {
            perror("Failed to open input file");
            exit(1);
        }

        sem_wait(semid);  // 부모가 쓰는 동안 대기

        size_t len = fread(shm_ptr, 1, MAX_BUF_SIZE, input_file);
        shm_ptr[len] = '\0';  // null terminator 추가

        fclose(input_file);

        printf("Child: Data copied to shared memory\n");

        sem_signal(semid);  // 부모에게 데이터 복사 완료 신호

        exit(0);  // 자식 프로세스 종료
    }

    else if (pid > 0) {
        // 부모 프로세스: 공유 메모리에서 데이터를 읽어서 출력 파일로 복사
        wait(NULL);  // 자식이 끝날 때까지 대기

        sem_wait(semid);  // 자식이 공유 메모리 쓰기를 완료할 때까지 대기

        FILE *output_file = fopen(OUTPUT_FILE, "w");
        if (output_file == NULL) {
            perror("Failed to open output file");
            exit(1);
        }

        fprintf(output_file, "%s", shm_ptr);  // 공유 메모리에서 읽어서 파일에 작성
        fclose(output_file);

        printf("Parent: Data copied to output file\n");

        sem_signal(semid);  // 자식에게 신호

        // 공유 메모리 분리
        shmdt(shm_ptr);
    }

    return 0;
}
