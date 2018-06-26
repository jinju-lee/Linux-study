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
	
	//���� ����
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);   
	if (serv_sock == -1)
		error_handling("socket() error");
	//0���� �ʱ�ȭ
	memset(&serv_addr, 0, sizeof(serv_addr));
	//IPv4 ���ͳ� ��������
	serv_addr.sin_family = AF_INET;
	//32bit IPTV4 �ּ�
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//����� ��Ʈ ��ȣ
	serv_addr.sin_port = htons(9190);
	
	//�ּ� �Ҵ�
	//���Ϲ�ȣ�� �����ּҸ� ���� ���� �Լ�
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	//���� ���
	// 5 = �������� ��� Ŭ���̾�Ʈ ��
	if (listen(serv_sock, 5) == -1)  
		error_handling("listen() error");
	clnt_addr_size = sizeof(clnt_addr);
	
	//��û ����
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, (socklen_t*) &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");
	
	// ������ �ۼ���
	while ((str_len = read(clnt_sock, message, BUFSIZE)) != 0) {
		write(clnt_sock, message, str_len);
		write(1, message, str_len);
	}
	
	//��� ����
	close(clnt_sock);       
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
