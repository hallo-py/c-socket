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

// global 
bool has_treading=false;   // accept-ing
int size=0;				   // listen size
SOCKET serversocket;       // server socket

thread t[10];   		   // thread
bool runt[10];             // is run thread

char sendText[255];		   // send text
bool threadSend[10];	   // can send to cilent(in the thred) 

int uploadSize=0;		// upload size
bool canUpload(){
	for (int i=0;i<uploadSize;i++){
		if (!threadSend[i]){   // if can send(==false[is wating]) ---> break
            return false;
		}
	}
	return true;
}

bool uploadText(string text){
	if (!canUpload()){
		return false;
	}
//	cout<<"comming uploadText"<<endl;
	strcpy(sendText,text.data()); 					// reset "sendText"
//	cout<<"\ttext:"<<sendText<<endl;
	memset(threadSend,false,sizeof(threadSend));	// reset "threadSend"
	return true;
}

void sendTread(SOCKET user,int i){
	while(true){
		if (!threadSend[i]){			// if true ---> send
//		    cout<<"send!"<<endl;
			send(user, sendText, 255, 0);
			threadSend[i]=true;    // don't send again
		}
	}
}

thread upload[10];
bool isupload[10];

void uploadTread(string text,int j){
	uploadSize++;
	if (uploadSize<10){
//		cout<<"\tcoming upload thread"<<endl;
		if (canUpload()){
			uploadText(text);
		} else {
			while (!canUpload()){
				if (canUpload()){
					uploadText(text);
					break;
				}
			}
		}
	}
//	cout<<"\texit upload thread"<<endl;
	isupload[j]=false;
	uploadSize--;
}

void self_accept(int i){
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
		thread send_thread(sendTread,c,i);     // send thread
//		cout<<"send_thread creat!"<<endl;
		while (true){
			char recvBuff[255];
//			cout<<"recv!"<<endl;
			int iRet = recv(c, recvBuff, 255, 0);
			if (iRet <= 0)
			{
				cout << "�ͻ��˹ر�����" << endl;
				runt[i]=false;
				size--;
				send_thread.detach();
				break;
			}
			// �յ��ͻ�����Ϣ 
			cout <<recvBuff<< endl;
			if (uploadSize>=10){		// ���ж���Ϣ�Ƿ���� 
				char tips[]="[����̫����,��������]";
				send(c,tips,strlen(tips),0);
			} else {   		// ���ж���û���ڽ�������Ľ���
				for (int j=0;j<10;j++){                 // �ҳ�isupload�е�һ��Ϊfalse(���е��߳�) 
					if (!isupload[j]){
					    isupload[j]=true;              // ����Ϣ�ڷ��� 
						try{						   // �����߳� 
							upload[j].detach();
						} catch (system_error err){}
						upload[j]=thread(uploadTread,recvBuff,j);    
						break;
					}
	//				cout<<endl;
				}
			}
		}
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
	 
	// init set
	memset(runt,false,sizeof(runt)); 					// init "is run thread"
	memset(threadSend,true,sizeof(threadSend));		// init "threadSend"
	memset(isupload,false,sizeof(isupload));		    // inti "isupload"
	
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
	addrSrv.sin_addr.S_un.S_addr = inet_addr(addr.data());
	addrSrv.sin_port = htons(port);
 
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
