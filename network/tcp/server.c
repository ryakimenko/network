#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFSIZE 81


int main()
{
    int sockMain, sockClient, length, child;
    struct sockaddr_in servAddr;
    if ((sockMain = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Сервер не может открыть главный socket.");
        exit(1);
    }
    servAddr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &servAddr.sin_addr);
    servAddr.sin_port = 0;
    if (bind(sockMain, (struct sockaddr*)&servAddr, sizeof(servAddr))) {
        perror("Связывание сервера неудачно.");
        exit(1);
    }
    length = sizeof(servAddr);
    if (getsockname(sockMain, (struct sockaddr*)&servAddr, &length)) {
        perror("Вызов getsockname неудачен.");
        exit(1);
    }
    printf("СЕРВЕР: номер порта - % d\n", ntohs(servAddr.sin_port));
    printf("СЕРВЕР: IP адрес - %s\n\n", inet_ntoa(servAddr.sin_addr));

    listen(sockMain, 5);

    signal(SIGCHLD, SIG_IGN);
    while (1) {
        int ssock = accept(sockMain, 0, 0);
        if (ssock < 0) {
            perror("accept error\n");
            exit(-1);
        }
        char buf[BUFFSIZE];
        int msgLength;
        bzero(buf, BUFFSIZE);
        switch (fork()) {
        case (0):
            while (1) {
                if ((msgLength = recv(ssock, buf, BUFFSIZE, 0)) < 0) {
                    perror("Плохое получение дочерним процессом.");
                    exit(1);
                } else if (msgLength == 0) {
                    break;
                }
                printf("SERVER: Socket для клиента - %d\n", ssock);
                printf("SERVER: Длина сообщения - %d\n", msgLength);
                printf("SERVER: Сообщение: %s\n\n", buf);
            }

        default:
            while (waitpid(-1, NULL, WNOHANG) > 0) {
            };

            break;
        case -1:
            perror("fork error\n");
            exit(-1);
        }
    }
    shutdown(sockClient, 2);
}
