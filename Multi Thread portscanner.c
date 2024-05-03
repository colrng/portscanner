#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_TARGETS 10
#define MAX_PORTS 100

void *scan(void *arg);

void scan_port(const char *ipaddress, int port);

struct ScanParams {
    char ipaddress[16];
    int ports;
};

int main() {
    char targets[256];
    int ports;

    printf("[*] 대상 ip를 입력하세요 (여러 개인 경우 쉼표로 구분): ");
    fgets(targets, sizeof(targets), stdin);
    targets[strcspn(targets, "\n")] = 0; 

    printf("[*] 스캔할 포트 수를 입력하세요: ");
    scanf("%d", &ports);
    getchar(); // 버퍼 지우기

    pthread_t threads[MAX_TARGETS];
    struct ScanParams params[MAX_TARGETS];

    char *token = strtok(targets, ",");
    int num_targets = 0;
    while (token != NULL && num_targets < MAX_TARGETS) {
        strcpy(params[num_targets].ipaddress, token);
        params[num_targets].ports = ports;

        if (pthread_create(&threads[num_targets], NULL, scan, &params[num_targets]) != 0) {
            fprintf(stderr, "스레드를 만드는 것을 실패했습니다.\n");
            return 1;
        }

        num_targets++;
        token = strtok(NULL, ",");
    }

    // 모든 스레드가 완료될 때까지 기다립니다.
    for (int i = 0; i < num_targets; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void *scan(void *arg) {
    struct ScanParams *params = (struct ScanParams *)arg;
    char *ipaddress = params->ipaddress;
    int ports = params->ports;

    printf("\n스캔을 시작합니다. %s\n", ipaddress);
    for (int port = 1; port <= ports; port++) {
        scan_port(ipaddress, port);
    }

    pthread_exit(NULL);
}

void scan_port(const char *ipaddress, int port) {
    struct sockaddr_in server;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        fprintf(stderr, "소캣 생성에 실패하였습니다.\n");
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ipaddress, &server.sin_addr);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0) {
        printf("[+] 포트가 열렸습니다 %d\n", port);
    }

    close(sock);
}
