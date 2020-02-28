#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <arpa/inet.h>
#include <poll.h>
#include <iostream>

void*
get_msg(void *ss)
{
    int client = *(int*) ss;
    delete (int*)ss;
    while(1)
    {
        char buffer[128];
        int len = recv(client, buffer, 128, 0);
        buffer[len] = 0;
        printf("%s\n", buffer);
    }
    return 0;
}

int
main(int argc, char**argv)
{
    int sock = 0;
    struct sockaddr_in serv;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8080);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connect(sock, (struct sockaddr *) &serv, sizeof(serv)) == -1)
    {
        perror("failed to connect");
        exit(-1);
    }
    printf("connected to %d\n", sock);
    pthread_t thread;
    int *pass = new int;
    *pass = sock;
    pthread_create(&thread, 0, get_msg, (void*)pass);

    while(1)
    {
        char in_msg[128];
        int ii = 0;
        while(ii < 127)
        {
            scanf("%c", &in_msg[ii]);
            if(in_msg[ii] == '\n' || in_msg[ii] == '\0' || in_msg[ii] == '\r')
            {
                break;
            }
            ii++;
        }
        in_msg[ii + 1] = 0;
	
        send(sock, in_msg, 128, 0);
    }
    exit(0);
}
