#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    struct iovec iov[2];
    char *messages[2] = {"Hello ", "from UDP client!\n"};
    struct msghdr msg = {0};
    char buffer[1024];

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    // Define message to be sent
    iov[0].iov_base = messages[0];
    iov[0].iov_len = strlen(messages[0]);
    iov[1].iov_base = messages[1];
    iov[1].iov_len = strlen(messages[1]);

    // Prepare the msghdr structure
    msg.msg_name = &servaddr;
    msg.msg_namelen = sizeof(servaddr);
    msg.msg_iov = iov;
    msg.msg_iovlen = 2;

    // Send the message
    if (sendmsg(sockfd, &msg, 0) == -1) {
        perror("sendmsg failed");
        exit(EXIT_FAILURE);
    }
    printf("Message sent.\n");

    // Receive server's response
    socklen_t len = sizeof(servaddr);
    ssize_t n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    } else {
        perror("recvfrom failed");
    }

    close(sockfd);
    return 0;
}

