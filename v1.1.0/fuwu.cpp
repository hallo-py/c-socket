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
	
	cout<<"���ڽ�������"<<endl;
	
	c=accept(serversocket, (SOCKADDR*)&addr, &len);
	has_treading=false;
	
	if (INVALID_SOCKET == c){
		cout<<"error!"<<endl;
		runt[i]=false;
	} else {
		cout << "�������ӳɹ�" << endl;
		size++;
		while (true){
			char recvBuff[255];
			int iRet = recv(c, recvBuff, 255, 0);
			if (iRet <= 0)
			{
				cout << "�ͻ��˹ر�����" << endl;
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
	
	//�����׽��ֿ�
	WSADATA wsadata;
	int iRet;
	iRet=WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (iRet != 0)
	{
		cout << "�׽��ֿ����ʧ��" << endl;
		return -1;
	}
	//�����׽���
	serversocket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (serversocket == INVALID_SOCKET)
	{
		cout << "�׽��ִ���ʧ��" << endl;
		return -1;
	}
	else
	{
		cout << "�׽��ִ����ɹ�" << endl;
	}
	//��ʼ����������ַ�����
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_port = htons(8000);
 
	//��
	iRet = bind(serversocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (iRet == SOCKET_ERROR)
	{
		cout << "��ʧ��" << endl;
		return -1;
	}
	else
	{
		cout << "�󶨳ɹ�" << endl;
	}
	//����
	iRet = listen(serversocket, 10);
	if (iRet == SOCKET_ERROR)
	{
		cout << "��������ʧ��" << endl;
		return -1;
	}
	else
	{
		cout << "���������ɹ�" << endl;
	}
	
	
	//��������(init)
//	SOCKADDR_IN addrClient;
//	int len = sizeof(SOCKADDR);
//	SOCKET connect;
//	
//	connect=accept(serversocket, (SOCKADDR*)&addrClient, &len);
//	if (INVALID_SOCKET == connectSocket[size-1])
//	{
//		cout << "��������ʧ��" << endl;
//		return -1;
//	}
//	else
//	{
//		cout << "�������ӳɹ�" << endl;
//		size++;
//	}
	//��������
	while (true){
//		cout<<"\t!has:"<<!has_treading<<" size:"<<size<<" pd:"<<((!has_treading)&&(size<10))<<endl;//text
		if ((!has_treading)&&(size<10)){     		// ���ж���û���ڽ�������Ľ���
//		    cout<<"\t has:"<<has_treading<<" !has:"<<!has_treading<<" size:"<<size<<" pd:"<<((!has_treading)&&(size<10));//text
//   			    <<"coming"<<endl;				// text
			for (int i=0;i<10;i++){                 // �ҳ�runt�е�һ��Ϊfalse(���е��߳�) 
				if (!runt[i]){
//					cout<<" coming";
				    has_treading=true;              // ���߳��ڽ�������
					runt[i]=true;                   // �̲߳�����
					try{							// �����߳� 
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
