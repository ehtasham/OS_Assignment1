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

#define BUF_SIZE 100
int main(int argc, char *arg[]){
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len=sizeof(client_addr);
	int server_fd,client_fd;
	int on=1;
	int size;
  	char buf[BUF_SIZE];
  	char *method;
  	char *filename;
  	int status;
  	int fdimg;

	server_fd=socket(AF_INET,SOCK_STREAM,0);

	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080);

	if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		close(server_fd);
		exit(1);
	}

	if (listen(server_fd, 10) == -1)
	{
		perror("listen");
		close(server_fd);
		exit(1);
	}

	while(1){
		client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &sin_len);
		if (client_fd < 0)
		{
			perror("Connection failed..\n");
			continue;
		}
		printf("connection establised.\n");
		recv(client_fd, buf, BUF_SIZE, 0);
		method = strtok(buf, " ");
		if (strcmp(method, "GET") != 0) 
			return;
		filename = strtok(NULL, " ");
		
		if (filename[0] == '/') filename++;
		
		if (access(filename, F_OK) != 0)
			status=1;
		else
			status=0;
		if (status == 0) {
    		printf("HTTP/1.0 200 OK\r\n\r\n");
    		fdimg = open(filename, O_RDONLY);
				sendfile(client_fd, fdimg, NULL, 6000);
				close(fdimg);
  		} else {
    		printf("HTTP/1.0 404 Not Found\r\n\r\n");
  			}
  			FILE *file = fopen(filename, "r");
  while(fgets(buf, BUF_SIZE, file)) {
    send(client_fd, buf, strlen(buf), 0);
    memset(buf, 0, BUF_SIZE);

	}
	close(client_fd);
}

	return 0;
}