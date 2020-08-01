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

class Server
{
private:
	int sockfd, new_fd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	char port[100] = "3499";

public:
	int InitializeServer(char *arg[])
	{
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE; //using my ip


		if((rv = getaddrinfo(arg[1], port, &hints, &servinfo)) != 0)
		{
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			return 1;
		}
		
		for(p = servinfo; p != NULL; p=p->ai_next) 
		{
			sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
			if(listen < 0)
			{
				continue;
			}

			if(bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
				close(sockfd);
				continue;
			}

			break;
		}

		//check if not Bound
		if(p == NULL)
		{
			return 1;
		}
		freeaddrinfo(servinfo); //Done with this

		//now Listen
		if (listen(sockfd, 10) == -1) {
			return 1;
		}
		return 0;
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

		// Start off with room for 5 connections
    	// (We'll realloc as necessary)
    	int fd_count = 0;
    	int fd_size = 5;
    	struct pollfd *pfds = (struct pollfd *)malloc(sizeof *pfds *fd_size);

		// Add the listener to set
    	pfds[0].fd = sockfd;
    	pfds[0].events = POLLIN; // Report ready to read on incoming connection


		while(1) 
		{
	        int poll_count = poll(pfds, fd_count, -1);

	        if (poll_count == -1) {
	            perror("poll");
	            exit(1);
	        }

	        //loop through connections
	        for(int i = 0; i < fd_count; i++)
	        {
				//check listener for new connections

				//pass connection to new 



	        }
		}
	}


};


int main(int argc, char *argv[])
{
	std::cout << "Starting program.\n";
	Server *server = new Server(argv);
	server->mainLoop();
}