#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    struct iovec iov[2];
    char *messages[2] = {"Hello ", "from UDP server!\n"};
    struct msghdr msg;
    socklen_t len;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    len = sizeof(cliaddr);
    recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
    buffer[1023] = '\0';
    printf("Client : %s\n", buffer);

    // Prepare the iovec structures
    iov[0].iov_base = messages[0];
    iov[0].iov_len = strlen(messages[0]);
    iov[1].iov_base = messages[1];
    iov[1].iov_len = strlen(messages[1]);

    // Prepare msghdr for sending
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = &cliaddr;
    msg.msg_namelen = len;
    msg.msg_iov = iov;
    msg.msg_iovlen = 2;

    // Send message
    sendmsg(sockfd, &msg, 0);

    return 0;
}

