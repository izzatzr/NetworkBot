#include <string.h>

extern int newsockfd;

void help()
{
	int i=0;
	char *myhelp[] = {
				"\n",
				"botlist				| display current bot",
				"udpflood <ip> <port> <times> <data> 	| flooder (UDP)",
				"tcpflood <ip> <port> <times> <data> 	| flooder (TCP)",
				"timeout <time>				| set timeout for checking bot",
				"quit 					| quit from server",
				"help					| print this help",
				"shutdown				| shutdown the server",
				"checkbot				| check for incoming bot",
				NULL
				};
	while (myhelp[i] != NULL)
		{
			send(newsockfd,myhelp[i],strlen(myhelp[i]),0);
			write(newsockfd,"\n",strlen("\n"));
			i++;
		}
	i=0;
}
