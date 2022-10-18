#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int sock;

    if (argc < 4) {
        printf("Wrong input: should be 'ip address, port, message'\n");
        exit(1);
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Can't get socket\n");
        exit(1);
    }

    struct sockaddr_in addres;

    addres.sin_family = AF_INET;
    addres.sin_port = 0;
    addres.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&addres, sizeof(addres))){
        printf("Client didn't get port\n");
        exit(1);
    }

    printf("CLIENT: Port number - %d\n", ntohs(addres.sin_port));

    struct sockaddr_in servAddr;
    servAddr.sin_family=AF_INET;
    servAddr.sin_port = htons(atoi(argv[2]));

    printf("CLIENT: Ready to send\n");

    inet_pton(AF_INET, argv[1], &servAddr.sin_addr);

    char message[5];
    for (int i = 0; i < atoi(argv[3]); i++){
        if (sendto(sock, argv[3], strlen(argv[3]), 0, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
            printf("Error in sendto\n");
            exit(1);
        }
        sleep(atoi(argv[3]));
    }
    printf("CLIENT: Sending is done\n");
    close(sock);
    return 0;
}