#include <stdio.h>
#include <string.h>
#include <windows.h>
#define MAX 10

int Num_resource,Num_processes;//资源数，进程数

int available[MAX];//可用资源数
int state[MAX];//进程状态
int state2[MAX];
int requests[MAX];//当前请求资源数
int max[MAX][MAX];//最大需求矩阵
int need[MAX][MAX];//当前需求矩阵
int allocated[MAX][MAX];//当前分配矩阵




int bavailable[MAX];//可用资源数
int bneed[MAX][MAX];//当前需求矩阵
int ballocated[MAX][MAX];//当前分配矩阵


void backup() {
	for (int i = 1; i <= Num_resource; i++) {//资源不够分配,拒绝
		bavailable[i] = available[i];
	}
	for (int i = 1; i <= Num_resource; i++) {//重新计算need矩阵
		for (int j = 1; j <= Num_processes; j++) {
			bneed[i][j] = need[i][j];
			ballocated[i][j] = allocated[i][j];
		}
	}
}
void restore() {
	for (int i = 1; i <= Num_resource; i++) {//资源不够分配,拒绝
		available[i] = bavailable[i];
	}
	for (int i = 1; i <= Num_resource; i++) {//重新计算need矩阵
		for (int j = 1; j <= Num_processes; j++) {
			need[i][j] = bneed[i][j];
			allocated[i][j] = ballocated[i][j];
		}
	}
}

int bank_algorithm(int id)//银行家算法 需要大家自己填写 
{
	for (int i = 1; i <= Num_resource; i++) {//资源不够分配,拒绝
		if (available[i] < requests[i])
			return 0;
	}
	backup();
	for (int i = 1; i <= Num_processes; i++) {//拷贝进程的完成状态
		state2[i] = state[i];
	}
	for (int i = 1; i <= Num_resource; i++) {//尝试分配资源
		need[id][i] -= requests[i];
		allocated[id][i] += requests[i];
		available[i] -= requests[i];
	}


	while (true) {
		int flag = 0;
		for (int i = 1; i <= Num_resource; i++) {//重新计算need矩阵
			for (int j = 1; j <= Num_processes; j++) {
				need[i][j] = max[i][j] - allocated[i][j];
			}
		}
		for (int i = 1; i <= Num_processes; i++) {//检查i号进程
			if (state2[i] == 0) {//如果未完成
				int j;
				for (j = 1; j <= Num_resource; j++) {//检查能否分配
					if (available[j] < need[i][j])
						break;
				}
				if (j == Num_resource + 1) {//能分配
					flag = 1;//此轮分配了
					state2[i] = 1;//完成
					for (int k = 1; k <= Num_resource; k++) {//回收资源
						available[k] += allocated[i][k];
					}
				}
			}
		}
		int ans = 1;
		for (int i = 1; i <= Num_processes; i++) {//进程的完成状态
			ans = ans && state2[i];
		}
		if (ans == 1)//全都完成啦，安全
		{
			restore();//恢复初始状态
			for (int i = 1; i <= Num_resource; i++) {//分配资源
				need[id][i] -= requests[i];
				allocated[id][i] += requests[i];
				available[i] -= requests[i];
			}
			for (int i = 1; i <= Num_resource; i++) {//重新计算need矩阵
				for (int j = 1; j <= Num_processes; j++) {
					need[i][j] = max[i][j] - allocated[i][j];
				}
			}
			return 1;
		}
		if (flag == 0)break;//一轮一个都没分配那肯定不安全
	}
	//不安全
	restore();//恢复初始状态
	return -1;
}

int main()
{
    int i,j,p_num,ans ;
    FILE *fp;
    
    printf("请确保输入文件input.txt存在，结果输出到output.txt\n");
   	if ((fp = fopen("Input.txt","r")) == NULL)
		printf("Read File Error\n");
    
    fscanf(fp,"%d",&Num_resource);                  //读入资源种类数 
    for (i=1; i<=Num_resource; i++)
        fscanf(fp,"%d",&available[i]);              //读入初始资源数 

    fscanf(fp,"%d",&Num_processes);                 //读入进程数
    
    for (i=1; i<=Num_processes; i++)
    {
        for (j=1; j<=Num_resource; j++)
        {
            fscanf(fp,"%d",&max[i][j]);             //读入MAX矩阵
            need[i][j] = max[i][j];
            allocated[i][j] = 0;
        }
    }
    //输出初始情况 
    printf("Initial: \n");
    for (i=1;i<=Num_resource;i++)
        printf("%c ",'A'+i-1);
    printf("\n");
    for (i=1;i<=Num_resource;i++)
        printf("%d ",available[i]);
    printf("\n");
    
    while(fscanf(fp,"%d",&p_num)!=EOF)
    {
        for (i=1; i<=Num_resource; i++)
            fscanf(fp, "%d",&requests[i]);
        printf("Process %d requests (",p_num);
        for (i=1; i<=Num_resource; i++)
        {
            printf("%d",requests[i]);
            if(i!=Num_resource) printf(",");
        }
        printf(")");
        
        ans = bank_algorithm(p_num);
        if(ans==1)                                               //允许分配
        {
            printf(" – granted\n");
        }
        else if(ans==0)                                          //不允许分配 超过最大可用
        {
            printf("– refused (extend the amount of available)\n");
        }
        else
        {
            printf(" – refused (Deadlock is possible)\n");
        }
        
        for (i=1; i<=Num_resource; i++)
        {
            if(need[p_num][i]!=0)
                break;
        }
        if(i==Num_resource+1)
        {
			state[p_num] = 1;
			for (int i = 1; i <= Num_resource; i++) {
				available[i] += allocated[p_num][i];
				allocated[p_num][i] = 0;

			}
            printf("Process %d finishes\n",p_num);
        }

    }
    
    system("pause"); 
}
