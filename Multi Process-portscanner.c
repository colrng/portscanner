#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define MAX_TARGETS 10
#define MAX_PORTS 100

void scan_port(const char *ipaddress, int port);

int main() {
    char targets[256];
    int ports;

    printf("[*] 대상 ip를 입력하세요 (여러 개인 경우 쉼표로 구분): ");
    fgets(targets, sizeof(targets), stdin);
    targets[strcspn(targets, "\n")] = 0; 

    printf("[*] 스캔할 포트 수를 입력하세요: ");
    scanf("%d", &ports);
    getchar(); // 버퍼 지우기

    char *token = strtok(targets, ",");
    while (token != NULL) {
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork를 실패 하였습니다.\n");
            return 1;
        } else if (pid == 0) { // 자식 프로세스
            printf("\n%s에 대한 스캔을 시작합니다\n", token);
            for (int port = 1; port <= ports; port++) {
                scan_port(token, port);
            }
            return 0;
        }
        token = strtok(NULL, ",");
    }

    // 모든 자식 프로세스의 완료를 기다림
    int status;
    while (wait(&status) > 0);

    return 0;
}

void scan_port(const char *ipaddress, int port) {
    struct sockaddr_in server;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        fprintf(stderr, "소켓 생성 실패\n");
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ipaddress, &server.sin_addr);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0) {
        printf("[+] %d 포트가 열렸습니다\n", port);
    }

    close(sock);
}
