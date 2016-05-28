//TCP Echo Server
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>

#define	SERV_TCP_PORT	9877
#define	MAXLINE	512

char	*pname;
char op_line[MAXLINE];
main(argc, argv)
int	argc;
char	*argv[];
{
	int	sockfd, newsockfd, clilen, childpid;
	struct sockaddr_in	cli_addr, serv_addr;
	pname = argv[0];
	
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("server: can't open stream socket");
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr 	= htonl(INADDR_ANY);
	serv_addr.sin_port		= htons(SERV_TCP_PORT);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		perror("server: can't bind local address");
	listen(sockfd, 5);
	for ( ; ; ) 
	{	
		printf("\nTCP Server Running\n");		
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&clilen);
		if (newsockfd < 0)
			perror("server: accept error");
		if ( (childpid = fork()) < 0)
			perror("server: fork error");
		else if (childpid == 0) 
		{	
			close(sockfd);
			str_echo(newsockfd);
			exit(0);
		}
		close(newsockfd);
	}
}

str_echo(sockfd)
int sockfd;
{
	int i,n,k;
	int a=0,e=0,i1=0,o=0,u=0;	
	char line[MAXLINE];
	char rev_line[MAXLINE];
	int vow[5];
	char b[5][MAXLINE];
	char vowel[5][MAXLINE];
	for ( ; ; ) 
	{
		n = recv(sockfd, line, MAXLINE, 0);
		if (n == 0)
			return;
		else if (n < 0)
			perror("str_echo: readline error");	
		k=strlen(line);
		for(i=0;i<k;i++)
		rev_line[i]=line[k-1-i];
		rev_line[i]='\0';
		if (send(sockfd, rev_line, n,0) != n)
		perror("str_echo: writen error");

		for(i=0;rev_line[i]!='\0';++i) 
		{ 
			if(rev_line[i]=='a' || rev_line[i]=='A')
			a++;
			if(rev_line[i]=='e' || rev_line[i]=='E')
			e++;
			if(rev_line[i]=='i' || rev_line[i]=='I')
			i1++;
			if(rev_line[i]=='o' || rev_line[i]=='O')
			o++;
			if(rev_line[i]=='u' || rev_line[i]=='U')
			u++;
		}

		vow[0]=a;
		vow[1]=e;
		vow[2]=i1;
		vow[3]=o;
		vow[4]=u;

		strcpy(b[0],"Number of times a repeats : ");
		strcpy(b[1],"Number of times e repeats : ");
		strcpy(b[2],"Number of times i repeats : ");
		strcpy(b[3],"Number of times o repeats : ");
		strcpy(b[4],"Number of times u repeats : ");

		for (i = 0 ; i < 5 ; ++i)
		{
		   sprintf(vowel[i], "%d", vow[i]);
		   strcat(b[i],vowel[i]);
		   b[i][strlen(b[i])] = '\n';
		}
		b[4][strlen(b[4])] = '\0';

		for (i = 0 ; i < 5 ; ++i)
			{
			if (send(sockfd, b[i],strlen(b[i]),0) != strlen(b[i]))
			perror("str_echo: writen error");
			}
		}
}

