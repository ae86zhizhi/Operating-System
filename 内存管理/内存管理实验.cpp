#include <cstdio>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <winbase.h>
using namespace std;

void GetMemSta() {
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	MEMORYSTATUSEX MemInfo;
	MemInfo.dwLength = sizeof(MemInfo);
	GlobalMemoryStatusEx(&MemInfo);
	cout << "Current Memory Status is:" << endl
		<< "Total Physical Memory is " << MemInfo.ullTotalPhys/ 1024 / 1024 << "MB" << endl
		<< "Available Physical Memory is " << MemInfo.ullAvailPhys / 1024 / 1024 << "MB" << endl
		<< "Total Virtual memory is " << MemInfo.ullTotalVirtual / 1024 / 1024 << "MB" << endl
		<< "Available Virtual memory " << MemInfo.ullAvailVirtual / 1024 / 1024 << "MB" << endl;
}
int main() {
	GetMemSta();					//�����ǰ�ڴ����
	printf("================================================.\n");
	cout << "Allocated 128M Memory Space. " << endl;
	LPVOID  space128 = malloc(128 * 1024 * 1024);//ʹ���ڴ���������128MB��
	memset(space128,0, 128 * 1024 * 1024);//�������������ڴ�ռ���д��0
	cout << "Memory zeroed. " << endl;
	GetMemSta();					//�����ǰ�ڴ����
	free(space128); 				//�ͷŸո������128MB��
	cout << "Freed 128M Memory Space. " << endl;
	GetMemSta();					//�����ǰ�ڴ����
	printf("================================================.\n");
	cout << "Allocated 128M Memory Space. " << endl;
	LPVOID pBlock = VirtualAlloc(NULL, 128 << 20, MEM_COMMIT, PAGE_READWRITE);//ʹ����������Ի������128MB��						
	memset(pBlock, 0, 128 * 1024 * 1024);//�������������ڴ�ռ���д��0
	cout << "Memory zeroed. " << endl;
	GetMemSta();					//�����ǰ�ڴ����
	VirtualFree(pBlock, 0, MEM_RELEASE);//�ͷŸո������128MB��
	cout << "Freed 128M Memory Space. " << endl;
	GetMemSta();					//�����ǰ�ڴ����
	printf("================================================.\n");
	cout << "Allocated 512M Virtual Memory Space." << endl;
	pBlock = VirtualAlloc(NULL, 512 << 20, MEM_RESERVE, PAGE_READWRITE);//ʹ����������Ի������512MB��
	
	cout << "Could not zero memory " << endl;
	//memset(pBlock, 0, 512 * 1024 * 1024);//�������������ڴ�ռ���д��0
	GetMemSta();					//�����ǰ�ڴ����
	VirtualFree(pBlock, 0, MEM_RELEASE);//�ͷŸո����������512MB��
	cout << "Freed 512M Virtual Memory Space." << endl;
	GetMemSta();					//�����ǰ�ڴ����
	printf("================================================.\n");
	cout << "Allocated 512M Virtual Memory Space and 128M Physical Memory Space." << endl;
	pBlock = VirtualAlloc(NULL, 512 << 20, MEM_RESERVE, PAGE_READWRITE);//ʹ������������������512MB�飬��Ϊ�����128MB����洢
	pBlock = VirtualAlloc(pBlock, 128 << 20, MEM_COMMIT, PAGE_READWRITE);
	memset(pBlock, 0, 128 * 1024 * 1024);//������������128MB�ڴ�ռ���д��0
	cout << "Memory zeroed. " << endl;
	GetMemSta();					//�����ǰ�ڴ����
	VirtualFree(pBlock, 0, MEM_RELEASE);//�ͷŸո����������512MB��
	cout << "Freed 512M Virtual Memory Space and 128M Physical Memory Space." << endl;
	GetMemSta();					//�����ǰ�ڴ����
	return 0;
}