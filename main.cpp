#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <iostream>

#define BACKLOG 10   // how many pending connections queue will hold

class Server
{
private:
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    //struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
	char port[100] = "3484";

public:
	int InitializeServer(char *arg[])
	{
		memset(&hints, 0, sizeof hints);
	    hints.ai_family = AF_UNSPEC;
	    hints.ai_socktype = SOCK_STREAM;
	    hints.ai_flags = AI_PASSIVE; // use my IP

	    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
	        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	        return 1;
	    }

	    // loop through all the results and bind to the first we can
	    for(p = servinfo; p != NULL; p = p->ai_next) {
	        if ((sockfd = socket(p->ai_family, p->ai_socktype,
	                p->ai_protocol)) == -1) {
	            perror("server: socket");
	            continue;
	        }

	        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
	                sizeof(int)) == -1) {
	            perror("setsockopt");
	            exit(1);
	        }

	        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	            close(sockfd);
	            perror("server: bind");
	            continue;
	        }

	        break;
	    }

	    freeaddrinfo(servinfo); // all done with this structure

	    if (p == NULL)  {
	        fprintf(stderr, "server: failed to bind\n");
	        exit(1);
	    }

	    if (listen(sockfd, BACKLOG) == -1) {
	        perror("listen");
	        exit(1);
	    }
	}

	Server(char *argv[])
	{
		int test;
		if ((test = InitializeServer(argv)) != 0)
		{
			printf("Failed to Initialize Server\n");
		}
	}


	// get sockaddr, IPv4 or IPv6:
	void *get_in_addr(struct sockaddr *sa)
	{
	    if (sa->sa_family == AF_INET) {
	        return &(((struct sockaddr_in*)sa)->sin_addr);
	    }

	    return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}

	int mainLoop()
	{
		printf("server: waiting for connections...\n");
		char buf[256];    // Buffer for client data

	    while(1) {  // main accept() loop
	        sin_size = sizeof their_addr;
	        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	        if (new_fd == -1) {
	            perror("accept");
	            continue;
	        }

	        inet_ntop(their_addr.ss_family,
	            get_in_addr((struct sockaddr *)&their_addr),
	            s, sizeof s);
	        printf("server: got connection from %s\n", s);

	        if (!fork()) { // this is the child process

				close(sockfd); // child doesn't need the listener
	            if (send(new_fd, "Hello, world!", 13, 0) == -1)
	                perror("send");

	            while(1)
	            {
	            	int nbytes = recv(new_fd, buf, sizeof buf, 0);
	            	if(nbytes == 0)
	            	{
	            		break;
	            	}
	            	if (send(new_fd, buf, nbytes, 0) == -1)
	                	perror("send");
	            }

	            close(new_fd);
	            exit(0);
	        }
	        close(new_fd);  // parent doesn't need this
	    }
	}

};


int main(int argc, char *argv[])
{
	std::cout << "Starting program.\n";
	Server *server = new Server(argv);
	server->mainLoop();
}