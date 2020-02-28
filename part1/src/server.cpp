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

/*
    some pieces of code are based on the example code,
    provided on piazza.
    https://d1b10bmlvqabco.cloudfront.net/attach/k51bluky59n2jr/goynt4j0qaR/k6k8p5i5qxqa/L10.key.pdf
*/

static const int BUFFER_SIZE = 128;
static const int PORT = 8080;
static const int MAX_CHILDREN = 16;


// class llist
// {
//     public:
//         int data;
//         llist *next;
    
//     llist(int dd)
//     {
//         data = dd;
//         next = 0;
//     }
    
//     llist* delink(int dd)
//     {
//         if(dd == this->data)
//         {
//             llist *ret = this->next;
//             delete this;
//             return ret;
//         }
//         else
//         {
//             llist *ret = this;
//             if(next)
//             {
//                 this->next = delink(dd);
//             }
//             return this;
//         }
//     }
// };

// llist *list_of_fds;

typedef struct Client_info
{
    
        int index;
        int sd;
        struct sockaddr_in c_addr;
        socklen_t addr_sz;
} Client_info;


Client_info clients[MAX_CHILDREN];
// number of clients
int c_num = 0;

void*
conn_client(void *sd_ptr);

//checks rv
void 
check_rv(int rv, int err)
{
    if(rv < 0)
    {
        puts("something went wrong!");
        printf("error code: %d\n", err);
        abort();
    }
}


int
main(const int argc, const char **argv)
{
    if(argc != 3)
    {
        perror("usage: ./server FLAG ARG");
        abort();
    }

    if(!strcmp(argv[2], "-ip"))
    {
        printf("You should be using: ./server -ip <IP_ADDR>\n");
    }

    int server_fd, sock, clnt_sock, rv, children = 0;
    struct sockaddr_in adr;
    int addr_len = sizeof(adr);

    rv = (server_fd = socket(AF_INET, SOCK_STREAM, 0));
    check_rv(rv, 1001);

    

    // set attr
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = INADDR_ANY;
    adr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[2], &adr.sin_addr);
    rv = bind(server_fd, (struct sockaddr *) &adr, 
        sizeof(adr));
    check_rv(rv, 1003);

    printf("We are on %s::%d\n", argv[2], PORT);
    puts("start listening");
    
    assert(listen(server_fd, MAX_CHILDREN) >= 0);

    pthread_t thread[MAX_CHILDREN];

    while(1)
    {
        clients[c_num].sd = accept(server_fd, (struct sockaddr*)&clients[c_num], &clients[c_num].addr_sz);
        printf("Client id %d has joined\n", clients[c_num].sd);
        for(int ii = 0; ii < c_num; ii++)
        {
            char greetings[100];
            int n = sprintf(greetings, "%d has joined\n", clients[c_num].sd);
            greetings[n] = 0;
            send(clients[ii].sd, greetings, n + 1, 0);
        }
        clients[c_num].index = c_num;
        pthread_create(&thread[c_num], 0, conn_client, (void*)&clients[c_num]);
        c_num++;
    }
    
    for(int ii = 0; ii < MAX_CHILDREN; ii++)
    {
        pthread_join(thread[ii], 0);
    }

}

void*
conn_client(void* sd_ptr)
{
    // socket client info
    Client_info sci = *(Client_info*)sd_ptr;
    const char *man = "You may use -pm <USER_ID> <MSG> to send private messages.\nYou may use -list to find out who's on the server\0";
    send(sci.sd, man, 109, 0);

    // read from client
    while(1)
    {
        char buffer[BUFFER_SIZE] = {0};
        int len = read(sci.sd, buffer, BUFFER_SIZE);
        buffer[len] = 0;
        printf("user %d entered %s", sci.sd, buffer);

        // chekc if it's command
        if(buffer[0] == '-')
        {
            puts("handling - ");
            // list
            if(!strncmp(buffer, "-list", 5))
            {
                char out[BUFFER_SIZE];
                int nn = sprintf(out, "Current users:\n");
                for(int ii = 0; ii < c_num; ii++)
                {
                    nn += sprintf(out + nn, "%d\n", clients[ii].sd);
                }
                out[nn] = 0;
                send(sci.sd, out, nn + 1, 0);
            }

            // pm
            if(!strncmp(buffer, "-pm ", 4))
            {
                char target[36];
                memset(target, 0, 36);
                int pos = 4;
                while(1)
                {
                    if(buffer[pos] && buffer[pos] != ' ')
                    {
                        printf("%c", buffer[pos]);
                        target[pos - 4] = buffer[pos];
                        pos++;
                        continue;
                    }
                    break;
                }
                printf("looking for %s", target);

                for(int ii = 0; ii < c_num; ii++)
                {
                    char *end;
                    int target_id = strtol(target, &end, 10);
                    if(clients[ii].sd == target_id)
                    {
                        char message[BUFFER_SIZE + 26];
                        int nn = sprintf(message, "%d (private message)> %s", sci.sd, buffer + pos);
                        message[nn] = 0;
                        send(clients[ii].sd, message, nn + 1, 0);
                        send(sci.sd, "message delivered\n", 20, 0);
                        break;
                    }
                }
            }
        }

        // broadcast
        else
        {
            char message[BUFFER_SIZE + 22];
            int nn = sprintf(message, "user id: %d says> %s", sci.sd, buffer);
            message[nn] = 0;

            for(int ii = 0; ii < c_num; ii++)
            {
                send(clients[ii].sd, message, nn + 1, 0);
            }
        }
    }
}
//     check_rv(bytes_read, 2001);
//     buffer[msg_sz - 1] = 0;
// }
