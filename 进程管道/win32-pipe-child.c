#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 25                          //����ܵ�����Ĵ�С 

int main(VOID)
{
    HANDLE ReadHandle, WriteHandle; 
    char message[BUFFER_SIZE] = "������";
 	DWORD written;
    
    
    ReadHandle = GetStdHandle(STD_INPUT_HANDLE);   //�õ�������                  
    WriteHandle= GetStdHandle(STD_OUTPUT_HANDLE);  //�õ������� 
   	
	if (!WriteFile (WriteHandle, message, BUFFER_SIZE, &written, NULL))
    fprintf(stderr, "child: Error writing to pipe\n");
    return 0;       
}
