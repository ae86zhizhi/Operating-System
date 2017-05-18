#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 25

int main(VOID)
{
 HANDLE ReadHandle, WriteHandle;
 STARTUPINFO si;//指定所创建进程及其主线程的初始化信息，如窗口位置、窗口大小、标题栏、窗口样式等
 PROCESS_INFORMATION pi;//所创建进程及其主线程的信息 
 CHAR buffer[BUFFER_SIZE];
 DWORD read;//实际读取的字节数 	
 
 //如果这个为空管道就不能被继承了，所以这里创建安全变量 
 SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL,TRUE};

 //分配内存 
 ZeroMemory(&pi, sizeof(pi));
 
//创建无名管道 ,获得读写句柄 
 if ( !CreatePipe(&ReadHandle, &WriteHandle, &sa, 0)) {
    fprintf(stderr,"Create Pipe Failed\n");
    return 1;
 }
  
 
 
 GetStartupInfo(&si);
 si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
 si.hStdOutput = WriteHandle;//这里比较关键，子进程的标准输出被重定位到了管道的写句柄 
 si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
 si.dwFlags = STARTF_USESTDHANDLES;//表明将在STARTUPINFO结构的hStdInput, hStdOutput和 hStdError成员设置标准输入句柄、标准输出句柄和标准错误句柄 
 SetHandleInformation( ReadHandle, HANDLE_FLAG_INHERIT, 0); 
 
 //创建子进程 
 if (!CreateProcess(NULL, 
                     ".\\win32-pipe-child.exe",
                     NULL,
                     NULL,
                     TRUE, //继承父进程的句柄 
                     0,
                     NULL,
                     NULL,
                     &si,//指定所创建进程及其主线程的初始化信息，如窗口位置、窗口大小、标题栏、窗口样式等
                     &pi))//所创建进程及其主线程的信息
  {
     fprintf(stderr, "Process Creation Failed\n");
     return -1;
  }
               

//关闭管道的写句柄 
CloseHandle(WriteHandle);       
//从管道中读出数据 
if (ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL))     
    printf("parent: >%s<",buffer);
else
    fprintf(stderr, "parent: Error reading from pipe\n");
//关闭管道的读句柄 
 CloseHandle(ReadHandle);


//等待子进程结束 
 WaitForSingleObject(pi.hProcess, INFINITE);  

 system("pause"); 
 
//释放句柄 
 CloseHandle(pi.hProcess);
 CloseHandle(pi.hThread);                 
}
