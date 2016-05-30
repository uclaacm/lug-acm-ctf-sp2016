#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

void error(const char* s) {
    fprintf(stderr, "ERROR: %s\n", s);
    exit(1);
}

void sigchld_handler(int s) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void reap_children(void) {
    // set a SIGCHLD handler to reap zombified child processes
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        error("sigaction");
}

void swap(char buf[16], int idx1, int idx2) {
    char tmp = buf[idx1];
    buf[idx1] = buf[idx2];
    buf[idx2] = tmp;
}

void serve_request(int sock) {
    const size_t MAXREQ = 16;
    char ibuf[MAXREQ+10];
    memset(ibuf, 0, MAXREQ+10);
    size_t ibufpos = 0;
    ssize_t osent = 0;

    char* welcome = "Welcome to my fabulous Quote of the Day dispenser!\n"
                    "To receive my wisdom, please enter the password.\n"
                    "Password: ";
    osent = send(sock, welcome, strlen(welcome), 0);
    if (osent < 0)
        error("welcome");

    while (1) {
        // read data from connection
        size_t n = read(sock, ibuf+ibufpos, MAXREQ-ibufpos);

        // Socket issue
        if (n < 0)
            error("reading from socket");

        // Client prematurely broke TCP connection
        if (n == 0)
            return;

        // Advance state
        ibufpos += n;
        if (ibufpos == MAXREQ) {
            break;
        }
    }

    // Read whole password, can apply check now
    // Dammit, here's a v2 of the check!

    char key[16];
    key[0] = 0x74;
    key[1] = 0x8C;
    key[2] = 0x77;
    key[3] = 0xC9;
    key[4] = 0x75;
    key[5] = 0xDC;
    key[6] = 0x6D;
    key[7] = 0xFF;
    key[8] = 0x7B;
    key[9] = 0xC6;
    key[10] = 0x58;
    key[11] = 0x8E;
    key[12] = 0x7A;
    key[13] = 0x8F;
    key[14] = 0x6A;
    key[15] = 0xCC;
    swap(ibuf, 5, 9);
    swap(ibuf, 1, 13);
    swap(ibuf, 4, 11);
    swap(ibuf, 10, 6);
    char xors[2] = {0x18, 0xbf};
    int i;
    for (i = 0; i < 16; i++) {
        ibuf[i] ^= xors[i % 2];
    }
    for (i = 0; i < 16; i++) {
        if (key[i] != ibuf[i]) {
            char* msg = "WRONG PASSWORD! GO AWAY!\n";
            send(sock, msg, strlen(msg), 0);
            return;
        }
    }

    // Give awesome quote
    char* quote = "Here's the quote of the day:\n"
                  "The Best Offense, is a Really Good Offense\n";
    send(sock, quote, strlen(quote), 0);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // If any child process terminates and becomes a zombie, reap it
    reap_children();

    // Constructing the socket
    if (argc < 2)
        error("Usage: qotd <port>");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Binding the socket to the port and begin listening
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("binding");
    listen(sockfd, 5);

    // Listen loop
    clilen = sizeof(cli_addr);
    while (1) {

        // New connection
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("accept");

        // Fork a process to handle the connection
        pid = fork();
        if (pid < 0)
            error("fork");

        // The child closes the irrelevant fds and handles the request
        if (pid == 0)  {
            close(sockfd);
            serve_request(newsockfd);
            close(newsockfd);
            exit(0);
        }
        // The parent closes its copy of the connection and moves on
        else
            close(newsockfd);
    }

    // should never reach this point
    error("listen loop terminated");
    return 1;
}
