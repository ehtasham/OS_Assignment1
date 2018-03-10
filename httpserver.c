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
	int server,client;
	int on=1;
	int size;
  	char buf[BUF_SIZE];
  	char *method;
  	char *filename;
  	int status;
  	int resource;

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
		exit(1);
	}

	while(1){
		client = accept(server, (struct sockaddr *) &client_addr, &sin_len);
		if (client < 0)
		{
			printf("Not Connected\n");
			continue;
		}
		printf("Connected.\n");
		recv(client, buf, BUF_SIZE, 0);
		method = strtok(buf, " ");
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

  			
	close(client);
}

	return 0;
}