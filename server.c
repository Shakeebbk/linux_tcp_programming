#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) {
	char buffer[255];
	int socketfd, portno, newsocketfd, n, cli_len;
	struct sockaddr_in server_addr, cli_addr;
	
	if(argc < 2) {
		error("Incorrect arguments\n");
	}
	
	portno = atoi(argv[1]);
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(socketfd < 0) {
		error("Error creating the socket\n");
	}
	
	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portno);
	
	if(bind(socketfd, (struct sockaddr_in *)&server_addr, sizeof(server_addr)) < 0) {
		error("Error binding the socket\n");
	}
	
	listen(socketfd, 5);
	
	cli_len = sizeof(cli_addr);
	newsocketfd = accept(socketfd, (struct sockaddr_in*)&cli_addr, &cli_len);
	
	if(newsocketfd < 0) {
		error("Error in accepting the connection\n");
	}
	
	bzero(buffer, sizeof(buffer));
	
	n = read(newsocketfd, buffer, sizeof(buffer));
	
	if(n < 0) {
		error("Error reading the socket\n");
	}
	printf("Here is the message %s\n", buffer);
	
	n =  write(newsocketfd, "I got your message", 18);
	
	if(n < 0) {
		error("Error writing the message\n");
	}
	exit(0);
}
