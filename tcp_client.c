//TCP Echo Server
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>

#define	SERV_TCP_PORT	9877
#define SERV_HOST_ADDR "127.0.0.1" 
#define	MAXLINE	512

char *pname;

main(argc, argv)
int argc;
char *argv[];
{
	int	sockfd;
	struct sockaddr_in	serv_addr;
	pname = argv[0];
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr 	= inet_addr(SERV_HOST_ADDR);
	serv_addr.sin_port		= htons(SERV_TCP_PORT);
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("client: can't open stream socket");
	if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		perror("client: can't connect to server");
	str_cli(stdin, sockfd);
	close(sockfd);
	exit(0);
}
str_cli(fp, sockfd)
register FILE	*fp;
register int sockfd;
{
	int n;
	char sendline[MAXLINE],recvline[MAXLINE + 1];
	printf("Enter the string to be sent to the server\n");

	while (fgets(sendline, MAXLINE, fp) != NULL) 
	{
		n = strlen(sendline);
		char vowel[4];
		char new_vowel[5][MAXLINE];	
		sendline[n-1]='\0';
		printf("Sending %d bytes to server at %s\n",n,SERV_HOST_ADDR);
		if (send(sockfd, sendline, n,0) != n)
			perror("str_cli: writen error on socket");
		n = recv(sockfd, recvline, MAXLINE,0);
		if (n < 0)
			perror("str_cli: readline error");
		recvline[n] = 0;
		printf("Received From Server: ");
		fputs(recvline, stdout);

		recv(sockfd, vowel, MAXLINE,0);
		printf("\n\nVowels:\n");
		puts(vowel);
		exit(0);
		
	}
	if (ferror(fp))
		perror("str_cli: error reading file");
}
		
