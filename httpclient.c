#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>

#define BUF_SIZE 100

void GET(int clientfd, char *path) {
  char req[1000] = {0};
  sprintf(req, "GET %s HTTP/1.0\r\n\r\n", path);
  send(clientfd, req, strlen(req), 0);
}
 
int main(int argc,char **argv)
{
    int socket_fd,n;
    char buf[BUF_SIZE];
    struct sockaddr_in servaddr;
 
    socket_fd=socket(AF_INET,SOCK_STREAM,0);

 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(8080);
 
    inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));
 
    connect(socket_fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 
  
    // GET(socket_fd, "image.html");

    char req[1000] = {0};
    sprintf(req, "GET %s HTTP/1.0\r\n\r\n", argv[1]);
    send(socket_fd, req, strlen(req), 0);


    while (recv(socket_fd, buf, BUF_SIZE, 0) > 0) {
        fputs(buf, stdout);
        memset(buf, 0, BUF_SIZE);
    }
     close(socket_fd);

  return 0;
 
}