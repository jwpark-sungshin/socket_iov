#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h> // For struct iovec
#include <arpa/inet.h>

#define PORT 8080

int main() {
    struct sockaddr_in serv_addr;
    int sock = 0;
    struct iovec iov[2];
    char *messages[2] = {"Hello ", "from the client!\n"};
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Prepare the iovec structures
    iov[0].iov_base = messages[0];
    iov[0].iov_len = strlen(messages[0]);
    iov[1].iov_base = messages[1];
    iov[1].iov_len = strlen(messages[1]);

    // Write with writev
    writev(sock, iov, 2);

    read(sock, buffer, 1024);
    printf("Server response: %s\n", buffer);
    return 0;
}

