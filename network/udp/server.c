#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFSIZE 81

int main() {
  int socketMain;
  int length;
  int messageLength;
  const int bufSize = 81;
  char buf[BUFFSIZE];
  struct sockaddr_in serverAddr, clientAddr;

  if ((socketMain = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("Сервер не может открыть сокет для UDP\n");
    exit(1);
  }
  bzero((char *)&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  inet_aton("127.0.0.1", &serverAddr.sin_addr);
  serverAddr.sin_port = 0;

  if (bind(socketMain, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) {
    printf("Неудачное связывание\n");
    exit(1);
  }

  length = sizeof(serverAddr);
  if (getsockname(socketMain, (struct sockaddr *)&serverAddr, &length)) {
    printf("Ошибка при вызове getsockname\n");
    exit(1);
  }

  printf("Сервер: Номер порта - %d\n", ntohs(serverAddr.sin_port));
  printf("SERVER: IP %s\n", inet_ntoa(serverAddr.sin_addr));

  while (1) {
    length = sizeof(clientAddr);
    bzero(buf, sizeof(BUFFSIZE));
    messageLength = recvfrom(socketMain, buf, BUFFSIZE, 0,
                             (struct sockaddr *)&clientAddr, &length);

    if (messageLength < 0) {
      perror("Bad socket");
      exit(1);
    }

    printf("SERVER: IP адрес клиента: %s\n", inet_ntoa(clientAddr.sin_addr));
    printf("SERVER: PORT клиента: %d\n", ntohs(clientAddr.sin_port));
    printf("SERVER: Длина сообщения - %d\n", messageLength);
    printf("SERVER: Сообщение: %s\n\n", buf);
  }

  return 0;
}
