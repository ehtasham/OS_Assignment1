#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define BUF_SIZE 10000      /* Size of shared buffer */

int buffer[BUF_SIZE];   /* shared buffer */
int add = 0;            /* place to add next element */
int rem = 0;            /* place to remove next element */
int num = 0;  

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;      /* mutex lock for buffer */
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

struct sockaddr_in server_addr, client_addr;
socklen_t sin_len=sizeof(client_addr);
int server,client;
int on=1;
int size;
char buf[BUF_SIZE];
char *method;
char *filename;
int status;
int resource;


void *consumer (void *param);

int main(int argc, char *argv[]) {

    server=socket(AF_INET,SOCK_STREAM,0);
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        printf("bind");
        close(server);
        exit(1);
    }

    if (listen(server, 10) == -1)
    {
        printf("listen");
        close(server);
    }

    pthread_t tid2; 
    pthread_create(&tid2, NULL, consumer, NULL);

    while(1){
        client = accept(server, (struct sockaddr *) &client_addr, &sin_len);
        if (client < 0)
        {
            printf("Not Connected\n");
            continue;
        }
        printf("Connected.\n");
        pthread_mutex_lock (&m);    

        while (num == BUF_SIZE) {  
                pthread_cond_wait (&c_prod, &m);
            }
        buffer[add] = client;
    
        add = (add+1) % BUF_SIZE;
        num++;
        pthread_mutex_unlock (&m);
        pthread_cond_signal (&c_cons);

    }   
    return 0;
}

void *consumer(void *param) {
    int i;
    while(1){

            pthread_mutex_lock (&m);
            if (num < 0) {
                exit(1);
            }
            while (num == 0) {  /* block if buffer empty */
                pthread_cond_wait (&c_cons, &m);
            }

            recv(client, buf, BUF_SIZE, 0);
            method = strtok(buf, " ");
            printf("method %s\n", method );
            filename = strtok(NULL, " ");

            if (filename[0] == '/') filename++;
        
        if (access(filename, F_OK) != 0)
            status=1;
        else
            status=0;

        if (status == 0) {
            printf("HTTP/1.1 200 OK\r\n\r\n");
            resource = open(filename, O_RDONLY);
            sendfile(client, resource, NULL, 6000);
            close(resource);

            FILE *file = fopen(filename, "r");
            while(fgets(buf, BUF_SIZE, file)) {
            send(client, buf, strlen(buf), 0);
            memset(buf, 0, BUF_SIZE);
        }
        } else {
            printf("HTTP/1.1 404 Not Found\r\n\r\n");
            }
            
            i = buffer[rem];
            rem = (rem+1) % BUF_SIZE;
            num--;    
            close(client);

            pthread_mutex_unlock (&m);
            pthread_cond_signal (&c_prod);
        
}

            
     

    
    return 0;
}