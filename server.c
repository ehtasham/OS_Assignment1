/*Required Headers*/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>
 
int main()
{
 
    char str[100];
    int server_fd, comm_fd;
 
    struct sockaddr_in servaddr;
 
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(22000);
 
    bind(server_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    listen(server_fd, 200);
 
    comm_fd = accept(server_fd, (struct sockaddr*) NULL, NULL);
 
    while(1)
    {
 
        read(comm_fd,str,100);
 
        printf("Echoing back - %s",str);
 
    }
}