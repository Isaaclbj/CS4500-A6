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

// struct dir_con_help
// {
//     int port;
//     int server;
// };


static const int SERVER_PORT = 8080;

// void*
// dir_conn(void *input)
// {
//     dir_con_help *helper = (dir_con_help*) input;
//     int port = helper->port;
//     int server = helper->server;
//     delete helper;

//     printf("you may stop trying to connect with another client by entering END_OF_FILE\n");
//     while(stdin)
//     {
//         char buffer[128];
//         int len = recv(server, buffer, 128, 0);
//         buffer[len] = 0;
//         if(buffer[0] = '-')
//         {
//             return 0;
//         }
//         if(buffer[0] = '+')
//         {
//             int new_port = atoi(buffer+1);
//             struct sockaddr_in dir;
//             dir.sin_family = AF_INET;
//             dir.sin_port = htons(new_port);
//             dir.sin_addr.s_addr = htonl(INADDR_ANY);
//             if(connect(port, (struct sockaddr *) & dir, sizeof(dir)) == -1)
//             {
//                 return 0;
//             }
//             while(1)
//             {

//             }
//         }

//     }
//     return 0;
// }

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

        printf("%s", buffer);
        
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
    serv.sin_port = htons(SERVER_PORT);
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

        // if(!strncmp(in_msg, "-conn ", 6))
        // {
        //     int to_con = atoi(in_msg + 6);
        //     printf("You are trying to connect with #%d server: %d ...\n", to_con, sock);
        //     send(sock, in_msg, 128, 0);
        //     pthread_t dir_conn_thread = thread;
        //     dir_con_help *pass = new dir_con_help;
        //     pass->port = to_con;
        //     pass->server = sock;
        //     pthread_create(&dir_conn_thread, 0, dir_conn, pass);
        //     pthread_join(dir_conn_thread, 0);
        // }
        // else
        // {
            send(sock, in_msg, 128, 0);
        // }
        
    }
    exit(0);
}
