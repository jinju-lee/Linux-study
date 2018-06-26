#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 1024

void error_handling(char *message);

int main()
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUFSIZE];
	int str_len;

	//소켓 생성
	//프로토콜 체계로 인터넷 사용, IPv4 주소 체계 - TCP사용
	sock = socket(PF_INET, SOCK_STREAM, 0);  
	if (sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	//인터넷 전용 소켓 주소 구조체
	//주소 체계
	serv_addr.sin_family = AF_INET;
	//IP 주소
	serv_addr.sin_addr.s_addr = inet_addr("192.168.0.11");
	//포트 번호
	serv_addr.sin_port = htons(9190);

	//연결 요청
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	while (1) 
	{
		//데이터 전송, 확인
		fputs("Input message : ", stdout);
		fgets(message, BUFSIZE, stdin);
		if (!strcmp(message, "q\n"))
			break;
		write(sock, message, strlen(message));
		str_len = read(sock, message, BUFSIZE - 1);
		message[str_len] = 0;
		printf("output message : %s \n", message);
	}
	
	//통신 끊기
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
