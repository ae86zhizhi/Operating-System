#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 25                          //定义管道缓冲的大小 

int main(VOID)
{
    HANDLE ReadHandle, WriteHandle; 
    char message[BUFFER_SIZE] = "喵喵！";
 	DWORD written;
    
    
    ReadHandle = GetStdHandle(STD_INPUT_HANDLE);   //得到输入句柄                  
    WriteHandle= GetStdHandle(STD_OUTPUT_HANDLE);  //得到输出句柄 
   	
	if (!WriteFile (WriteHandle, message, BUFFER_SIZE, &written, NULL))
    fprintf(stderr, "child: Error writing to pipe\n");
    return 0;       
}
