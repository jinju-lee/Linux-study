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
	int serv_sock;
	int clnt_sock;
	char message[BUFSIZE];
	int str_len;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	
	//소켓 생성
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);   
	if (serv_sock == -1)
		error_handling("socket() error");
	//0으로 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	//IPv4 인터넷 프로토콜
	serv_addr.sin_family = AF_INET;
	//32bit IPTV4 주소
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//사용할 포트 번호
	serv_addr.sin_port = htons(9190);
	
	//주소 할당
	//소켓번호와 소켓주소를 묶어 놓는 함수
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	//연결 대기
	// 5 = 동시접속 허용 클라이언트 수
	if (listen(serv_sock, 5) == -1)  
		error_handling("listen() error");
	clnt_addr_size = sizeof(clnt_addr);
	
	//요청 수락
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*) &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");
	
	// 데이터 송수신
	while ((str_len = read(clnt_sock, message, BUFSIZE)) != 0) {
		write(clnt_sock, message, str_len);
		write(1, message, str_len);
	}
	
	//통신 끊기
	close(clnt_sock);       
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
