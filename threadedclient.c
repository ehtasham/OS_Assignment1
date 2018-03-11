#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>

#define BUF_SIZE 10000
#define NUM_THREADS 5

void *threadFunc(char *pArg) { /* thread main */
  char *filename = ((char*)pArg);
  int socket_fd,n;
    char buf[BUF_SIZE];
    struct sockaddr_in servaddr;
 
    socket_fd=socket(AF_INET,SOCK_STREAM,0);

 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(8080);
 
    inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));
 
    connect(socket_fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 
    char req[1000] = {0};
    sprintf(req, "GET %s HTTP/1.0\r\n\r\n", filename);
    send(socket_fd, req, strlen(req), 0);

     while (recv(socket_fd, buf, BUF_SIZE, 0) > 0) {
        fputs(buf, stdout);
        memset(buf, 0, BUF_SIZE);
    }
     close(socket_fd);
  return 0;
}

int main(int argc,char **argv)
{
    
  int i;
  int tNum[NUM_THREADS];
  pthread_t tid[NUM_THREADS];
  
  for(i = 0; i < NUM_THREADS; i++) { /* create/fork threads */
    tNum[i] = i;
    pthread_create(&tid[i], NULL, threadFunc, argv[1]);
    sleep(1);
  }
  
  for(i = 0; i < NUM_THREADS; i++) { /* wait/join threads */
    pthread_join(tid[i], NULL);
  }

  return 0;
 
}