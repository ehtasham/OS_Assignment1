#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>


#define BUF_SIZE 100000
ssize_t bytes_read;
ssize_t total_bytes;
// double time_elapsed_in_second;

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
  char req[10000] = {0};
  
  sprintf(req, "GET %s HTTP/1.0\r\n\r\n", filename);
    
  send(socket_fd, req, strlen(req), 0);
  bytes_read=recv(socket_fd, buf, BUF_SIZE, 0);
  total_bytes = total_bytes + bytes_read;
   
  fputs(buf, stdout);
      

  printf("read bytes %d \n",bytes_read);
  
    
  close(socket_fd);

     
printf("total bytes %d \n",total_bytes);
  return 0;
}

int main(int argc,char **argv)
{
    
  int i;
  int NUM_THREADS=atoi(argv[2]);
  int tNum[NUM_THREADS];
  pthread_t tid[NUM_THREADS];

  double throughput;

struct timeval start, end;
  
  for(i = 0; i < NUM_THREADS; i++) { /* create/fork threads */
    tNum[i] = i;
    

      gettimeofday(&start, NULL);
    pthread_create(&tid[i], NULL, threadFunc, argv[1]);
    ;

    
    sleep(1);
  }
  
  for(i = 0; i < NUM_THREADS; i++) { /* wait/join threads */
    pthread_join(tid[i], NULL);
    
  }
    gettimeofday(&end, NULL);
      long seconds = (end.tv_sec - start.tv_sec);
  long micros = ((seconds * 1000) + end.tv_usec) - (start.tv_usec);

  printf("Time elpased is %d seconds and %d micros\n", seconds, micros);
    throughput=(total_bytes)/micros;
    printf("throughput is %f\n",throughput);
  return 0;
 
}