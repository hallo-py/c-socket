#include <iostream>
#include <string.h>
#include <winsock2.h>
#pragma comment (lib,"WS2_32.lib")
 
using namespace std;
int main()
{
	//�����׽��ֿ�
	WSADATA wsadata;
	int iRet=WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (iRet != 0)
	{
		cout << "�׽��ֿ����ʧ��" << endl;
		return -1;
	}
	else
	{
		cout << "�׽��ֿ���سɹ�" << endl;
	}
	//�����׽���
	SOCKET clientsocket;
	clientsocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientsocket == INVALID_SOCKET)
	{
		cout << "�׽��ִ���ʧ��" << endl;
		return -1;
	}
	else
	{
		cout << "�׽��ִ����ɹ�" << endl;
	}
 
	//��ʼ���ͻ��˵�ַ�����
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_port = htons(8000);
 
	//����
	iRet=connect(clientsocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (iRet == SOCKET_ERROR)
	{
		cout << "����ʧ��" << endl;
		return -1;
	}
	else
	{
		cout << "���ӳɹ�" << endl;
	}
    
    char name[6];
    cout<<"name:";
    cin>>name;
    
	char buff_front[7];   // ���͵�ǰ�����ַ�
	strcpy(buff_front,name);
	strcat(buff_front,":"); 
	//��������
	while (true)
	{
		char s[255-7];
		cin.getline(s,255-7);
		
		char Buff[255];
		strcpy(Buff,buff_front);
		strcat(Buff,s);
		
		if (strcmp(Buff,buff_front)==0){
			strcpy(Buff,"");						// clear
			strcat(Buff,"Welcome user ");
            strcat(Buff,name);
			send(clientsocket, Buff, strlen("Welcome user ")+6, 0);
			continue;
		}
		
		send(clientsocket, Buff, 255, 0);	
	}
 
}
