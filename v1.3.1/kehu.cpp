// io
#include <iostream>
#include <string.h>
using namespace std;

// socket
#include <winsock2.h>
#pragma comment (lib,"WS2_32.lib")

// tread
#include <thread>
#include <future>

// pause
#include <windows.h>


SOCKET clientsocket;

int recvClient(){
	while (true){
		int iRet;
		char recvBuff[225];
		iRet=recv(clientsocket,recvBuff,225,0);
		if (iRet <= 0){
			cout << "服务端关闭连接" << endl;
			system("pause");
			exit(0);
		}
		cout <<"\t"<<recvBuff<<endl;						// 发送过来的数据已有换行(\n) 
	}
}

int main()
{
	// inp
	string addr;
	cout<<"addr:";
	cin>>addr;
	int port;
	cout<<"port:";
	cin>>port;
	
	if (addr=="d"){
		addr="127.0.0.1";
	}
	
	//加载套接字库
	WSADATA wsadata;
	int iRet=WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (iRet != 0)
	{
		cout << "套接字库加载失败" << endl;
		return -1;
	}
	else
	{
		cout << "套接字库加载成功" << endl;
	}
	//创建套接字
	clientsocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientsocket == INVALID_SOCKET)
	{
		cout << "套接字创建失败" << endl;
		return -1;
	}
	else
	{
		cout << "套接字创建成功" << endl;
	}
 
	//初始化客户端地址族变量
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(addr.data());
	addrSrv.sin_port = htons(port);
 
	//连接
	iRet=connect(clientsocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (iRet == SOCKET_ERROR)
	{
		cout << "连接失败" << endl;
		return -1;
	}
	else
	{
		cout << "连接成功" << endl;
	}
    
    char name[6];
    cout<<"name:";
    cin>>name;
    
	char buff_front[7];   // 发送的前几个字符
	strcpy(buff_front,name);
	strcat(buff_front,":"); 
	//发送数据
	
	thread recvThread(recvClient);
	
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
