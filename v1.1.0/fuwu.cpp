// io
#include <iostream>

// sock
#include <winsock2.h>
#pragma comment (lib,"WS2_32.lib")

// tread
#include <thread>
#include <future>

// string
#include <string.h>
 
using namespace std;

bool has_treading=false;   // accept-ing
int size=0;				   // listen size
SOCKET serversocket;       // server socket

thread t[10];   		   // thread
bool runt[10];             // is run thread

void self_accept(int i){
//	cout<<"kai"<<endl;
	
    SOCKADDR_IN addr;
	int len = sizeof(SOCKADDR);
	SOCKET c;
	
	cout<<"正在接受链接"<<endl;
	
	c=accept(serversocket, (SOCKADDR*)&addr, &len);
	has_treading=false;
	
	if (INVALID_SOCKET == c){
		cout<<"error!"<<endl;
		runt[i]=false;
	} else {
		cout << "接受连接成功" << endl;
		size++;
		while (true){
			char recvBuff[255];
			int iRet = recv(c, recvBuff, 255, 0);
			if (iRet <= 0)
			{
				cout << "客户端关闭连接" << endl;
				runt[i]=false;
				size--;
				break;
			}
			cout <<recvBuff<< endl;
		}
	}
}

int main()
{
	// init set
	memset(runt,false,sizeof(runt)); // init "is run thread"
	
	//加载套接字库
	WSADATA wsadata;
	int iRet;
	iRet=WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (iRet != 0)
	{
		cout << "套接字库加载失败" << endl;
		return -1;
	}
	//创建套接字
	serversocket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (serversocket == INVALID_SOCKET)
	{
		cout << "套接字创建失败" << endl;
		return -1;
	}
	else
	{
		cout << "套接字创建成功" << endl;
	}
	//初始化服务器地址族变量
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_port = htons(8000);
 
	//绑定
	iRet = bind(serversocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (iRet == SOCKET_ERROR)
	{
		cout << "绑定失败" << endl;
		return -1;
	}
	else
	{
		cout << "绑定成功" << endl;
	}
	//监听
	iRet = listen(serversocket, 10);
	if (iRet == SOCKET_ERROR)
	{
		cout << "开启监听失败" << endl;
		return -1;
	}
	else
	{
		cout << "开启监听成功" << endl;
	}
	
	
	//接收连接(init)
//	SOCKADDR_IN addrClient;
//	int len = sizeof(SOCKADDR);
//	SOCKET connect;
//	
//	connect=accept(serversocket, (SOCKADDR*)&addrClient, &len);
//	if (INVALID_SOCKET == connectSocket[size-1])
//	{
//		cout << "接受连接失败" << endl;
//		return -1;
//	}
//	else
//	{
//		cout << "接受连接成功" << endl;
//		size++;
//	}
	//接收数据
	while (true){
//		cout<<"\t!has:"<<!has_treading<<" size:"<<size<<" pd:"<<((!has_treading)&&(size<10))<<endl;//text
		if ((!has_treading)&&(size<10)){     		// 先判断有没有在接受申请的进程
//		    cout<<"\t has:"<<has_treading<<" !has:"<<!has_treading<<" size:"<<size<<" pd:"<<((!has_treading)&&(size<10));//text
//   			    <<"coming"<<endl;				// text
			for (int i=0;i<10;i++){                 // 找出runt中第一个为false(空闲的线程) 
				if (!runt[i]){
//					cout<<" coming";
				    has_treading=true;              // 有线程在接受申请
					runt[i]=true;                   // 线程不空闲
					try{							// 开启线程 
						t[i].detach();
					} catch (system_error err){}
					t[i]=thread(self_accept,i);     
					break;
		//			t.join();
				}
//				cout<<endl;
			}
		}
	}
}
