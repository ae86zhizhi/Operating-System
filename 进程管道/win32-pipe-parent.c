#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 25

int main(VOID)
{
 HANDLE ReadHandle, WriteHandle;
 STARTUPINFO si;//ָ�����������̼������̵߳ĳ�ʼ����Ϣ���細��λ�á����ڴ�С����������������ʽ��
 PROCESS_INFORMATION pi;//���������̼������̵߳���Ϣ 
 CHAR buffer[BUFFER_SIZE];
 DWORD read;//ʵ�ʶ�ȡ���ֽ��� 	
 
 //������Ϊ�չܵ��Ͳ��ܱ��̳��ˣ��������ﴴ����ȫ���� 
 SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL,TRUE};

 //�����ڴ� 
 ZeroMemory(&pi, sizeof(pi));
 
//���������ܵ� ,��ö�д��� 
 if ( !CreatePipe(&ReadHandle, &WriteHandle, &sa, 0)) {
    fprintf(stderr,"Create Pipe Failed\n");
    return 1;
 }
  
 
 
 GetStartupInfo(&si);
 si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
 si.hStdOutput = WriteHandle;//����ȽϹؼ����ӽ��̵ı�׼������ض�λ���˹ܵ���д��� 
 si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
 si.dwFlags = STARTF_USESTDHANDLES;//��������STARTUPINFO�ṹ��hStdInput, hStdOutput�� hStdError��Ա���ñ�׼����������׼�������ͱ�׼������ 
 SetHandleInformation( ReadHandle, HANDLE_FLAG_INHERIT, 0); 
 
 //�����ӽ��� 
 if (!CreateProcess(NULL, 
                     ".\\win32-pipe-child.exe",
                     NULL,
                     NULL,
                     TRUE, //�̳и����̵ľ�� 
                     0,
                     NULL,
                     NULL,
                     &si,//ָ�����������̼������̵߳ĳ�ʼ����Ϣ���細��λ�á����ڴ�С����������������ʽ��
                     &pi))//���������̼������̵߳���Ϣ
  {
     fprintf(stderr, "Process Creation Failed\n");
     return -1;
  }
               

//�رչܵ���д��� 
CloseHandle(WriteHandle);       
//�ӹܵ��ж������� 
if (ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL))     
    printf("parent: >%s<",buffer);
else
    fprintf(stderr, "parent: Error reading from pipe\n");
//�رչܵ��Ķ���� 
 CloseHandle(ReadHandle);


//�ȴ��ӽ��̽��� 
 WaitForSingleObject(pi.hProcess, INFINITE);  

 system("pause"); 
 
//�ͷž�� 
 CloseHandle(pi.hProcess);
 CloseHandle(pi.hThread);                 
}
