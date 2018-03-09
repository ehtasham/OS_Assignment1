#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>
 
int main(int argc,char **argv)
{
    int socket_fd,n;
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;
 
    socket_fd=socket(AF_INET,SOCK_STREAM,0);

 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(22000);
 
    inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));
 
    connect(socket_fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 
    while(1)
    {

        fgets(sendline,100,stdin); 
 
        write(socket_fd,sendline,strlen(sendline)+1);
    }
 
}