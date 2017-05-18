#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define N 16
int buffer[N];  /* 可以简单地假定为 int buffer[N] */

int in = 0, out = 0;
DWORD WINAPI Producer()
{
	int cnt = 20;//生产者执行次数
	while (cnt--)
	{
		/* Produce an item */
		while (((in + 1) % N) == out);
		/* do nothing -- no free buffers */
		buffer[in] = rand();
		printf("produce a new item to the %d - th slot\n", in);
		printf("The number of items in buffer is %d\n", buffer[in]);
		in = (in + 1) % N;
		Sleep(300);
	}
	return 0;
}

DWORD WINAPI Consumer()
{
	int item;
	int cnt = 20;//消费者执行次数
	while (cnt--)
	{
		while (in == out)
		//* do nothing -- no free buffers */
		// remove an item from the buffer
		item = buffer[out];
		printf("consume an item in the %d - th slot\n", out);
		printf("The number of items in buffer is %d\n", buffer[out]);
		out = (out + 1) % N;
		//return item;
		Sleep(500);
	}
	return 0;
}
int main()
{
	srand((unsigned)time(0));
	//创建生产者和消费者线程
	HANDLE hpro = CreateThread(NULL, 0, Producer, NULL, 0, NULL);
	HANDLE hcon = CreateThread(NULL, 0, Consumer, NULL, 0, NULL);
	Sleep(20000);
	system("pause");
	return 0;
}