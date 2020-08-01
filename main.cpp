#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>

class Server
{
private:
	int sockfd, new_fd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	struct sigaction sa;
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
		while(1) 
		{
			  
		}
	}


};


int main(int argc, char *argv[])
{
	std::cout << "Starting program.\n";
	Server *server = new Server(argv);
	server->mainLoop();
}