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
	int portno, socketfd, n;
	struct sockaddr_in cli_addr, server_addr;
	struct hostent *server;
	char buffer[255];
	
	if(argc < 3) {
		error("Incorrect arguments\n");
	}
	
	portno = atoi(argv[2]);
	
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(socketfd < 0) {
		error("Error getting a socket descriptor\n");
	}
	
	server = gethostbyname(argv[1]);
	
	if(server == NULL) {
		error("Error getting host address\n");
	}
	
	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	
	bcopy((char*)server->h_addr, (char*)&server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons(portno);
	
	if(connect(socketfd, &server_addr, sizeof(server_addr)) < 0) {
		error("Error in connecting!\n");
	}
	
	printf("Please enter the message\n");
	bzero(buffer, sizeof(buffer));
	
	fgets(buffer, sizeof(buffer), stdin);
	
	n = write(socketfd, buffer, strlen(buffer));
	
	if(n < 0) {
		error("Error writing\n");
	}
	
	bzero(buffer, sizeof(buffer));
	n = read(socketfd, buffer, sizeof(buffer)) ;
	
	if(n < 0) {
		error("Error reading the message\n");
	}
	printf("%s\n", buffer);
	exit(0);
}