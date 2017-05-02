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
	GetMemSta();					//输出当前内存情况
	printf("================================================.\n");
	cout << "Allocated 128M Memory Space. " << endl;
	LPVOID  space128 = malloc(128 * 1024 * 1024);//使用内存分配来获得128MB块
	memset(space128,0, 128 * 1024 * 1024);//尝试向刚申请的内存空间内写入0
	cout << "Memory zeroed. " << endl;
	GetMemSta();					//输出当前内存情况
	free(space128); 				//释放刚刚申请的128MB块
	cout << "Freed 128M Memory Space. " << endl;
	GetMemSta();					//输出当前内存情况
	printf("================================================.\n");
	cout << "Allocated 128M Memory Space. " << endl;
	LPVOID pBlock = VirtualAlloc(NULL, 128 << 20, MEM_COMMIT, PAGE_READWRITE);//使用虚拟分配以获得物理128MB块						
	memset(pBlock, 0, 128 * 1024 * 1024);//尝试向刚申请的内存空间内写入0
	cout << "Memory zeroed. " << endl;
	GetMemSta();					//输出当前内存情况
	VirtualFree(pBlock, 0, MEM_RELEASE);//释放刚刚申请的128MB块
	cout << "Freed 128M Memory Space. " << endl;
	GetMemSta();					//输出当前内存情况
	printf("================================================.\n");
	cout << "Allocated 512M Virtual Memory Space." << endl;
	pBlock = VirtualAlloc(NULL, 512 << 20, MEM_RESERVE, PAGE_READWRITE);//使用虚拟分配以获得虚拟512MB块
	
	cout << "Could not zero memory " << endl;
	//memset(pBlock, 0, 512 * 1024 * 1024);//尝试向刚申请的内存空间内写入0
	GetMemSta();					//输出当前内存情况
	VirtualFree(pBlock, 0, MEM_RELEASE);//释放刚刚申请的虚拟512MB块
	cout << "Freed 512M Virtual Memory Space." << endl;
	GetMemSta();					//输出当前内存情况
	printf("================================================.\n");
	cout << "Allocated 512M Virtual Memory Space and 128M Physical Memory Space." << endl;
	pBlock = VirtualAlloc(NULL, 512 << 20, MEM_RESERVE, PAGE_READWRITE);//使用虚拟分配调配获得虚拟512MB块，再为其调配128MB物理存储
	pBlock = VirtualAlloc(pBlock, 128 << 20, MEM_COMMIT, PAGE_READWRITE);
	memset(pBlock, 0, 128 * 1024 * 1024);//尝试向刚申请的128MB内存空间内写入0
	cout << "Memory zeroed. " << endl;
	GetMemSta();					//输出当前内存情况
	VirtualFree(pBlock, 0, MEM_RELEASE);//释放刚刚申请的虚拟512MB块
	cout << "Freed 512M Virtual Memory Space and 128M Physical Memory Space." << endl;
	GetMemSta();					//输出当前内存情况
	return 0;
}