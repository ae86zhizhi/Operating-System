#include <stdio.h>
#include <string.h>
#include <windows.h>
#define MAX 10

int Num_resource,Num_processes;//��Դ����������

int available[MAX];//������Դ��
int state[MAX];//����״̬
int state2[MAX];
int requests[MAX];//��ǰ������Դ��
int max[MAX][MAX];//����������
int need[MAX][MAX];//��ǰ�������
int allocated[MAX][MAX];//��ǰ�������




int bavailable[MAX];//������Դ��
int bneed[MAX][MAX];//��ǰ�������
int ballocated[MAX][MAX];//��ǰ�������


void backup() {
	for (int i = 1; i <= Num_resource; i++) {//��Դ��������,�ܾ�
		bavailable[i] = available[i];
	}
	for (int i = 1; i <= Num_resource; i++) {//���¼���need����
		for (int j = 1; j <= Num_processes; j++) {
			bneed[i][j] = need[i][j];
			ballocated[i][j] = allocated[i][j];
		}
	}
}
void restore() {
	for (int i = 1; i <= Num_resource; i++) {//��Դ��������,�ܾ�
		available[i] = bavailable[i];
	}
	for (int i = 1; i <= Num_resource; i++) {//���¼���need����
		for (int j = 1; j <= Num_processes; j++) {
			need[i][j] = bneed[i][j];
			allocated[i][j] = ballocated[i][j];
		}
	}
}

int bank_algorithm(int id)//���м��㷨 ��Ҫ����Լ���д 
{
	for (int i = 1; i <= Num_resource; i++) {//��Դ��������,�ܾ�
		if (available[i] < requests[i])
			return 0;
	}
	backup();
	for (int i = 1; i <= Num_processes; i++) {//�������̵����״̬
		state2[i] = state[i];
	}
	for (int i = 1; i <= Num_resource; i++) {//���Է�����Դ
		need[id][i] -= requests[i];
		allocated[id][i] += requests[i];
		available[i] -= requests[i];
	}


	while (true) {
		int flag = 0;
		for (int i = 1; i <= Num_resource; i++) {//���¼���need����
			for (int j = 1; j <= Num_processes; j++) {
				need[i][j] = max[i][j] - allocated[i][j];
			}
		}
		for (int i = 1; i <= Num_processes; i++) {//���i�Ž���
			if (state2[i] == 0) {//���δ���
				int j;
				for (j = 1; j <= Num_resource; j++) {//����ܷ����
					if (available[j] < need[i][j])
						break;
				}
				if (j == Num_resource + 1) {//�ܷ���
					flag = 1;//���ַ�����
					state2[i] = 1;//���
					for (int k = 1; k <= Num_resource; k++) {//������Դ
						available[k] += allocated[i][k];
					}
				}
			}
		}
		int ans = 1;
		for (int i = 1; i <= Num_processes; i++) {//���̵����״̬
			ans = ans && state2[i];
		}
		if (ans == 1)//ȫ�����������ȫ
		{
			restore();//�ָ���ʼ״̬
			for (int i = 1; i <= Num_resource; i++) {//������Դ
				need[id][i] -= requests[i];
				allocated[id][i] += requests[i];
				available[i] -= requests[i];
			}
			for (int i = 1; i <= Num_resource; i++) {//���¼���need����
				for (int j = 1; j <= Num_processes; j++) {
					need[i][j] = max[i][j] - allocated[i][j];
				}
			}
			return 1;
		}
		if (flag == 0)break;//һ��һ����û�����ǿ϶�����ȫ
	}
	//����ȫ
	restore();//�ָ���ʼ״̬
	return -1;
}

int main()
{
    int i,j,p_num,ans ;
    FILE *fp;
    
    printf("��ȷ�������ļ�input.txt���ڣ���������output.txt\n");
   	if ((fp = fopen("Input.txt","r")) == NULL)
		printf("Read File Error\n");
    
    fscanf(fp,"%d",&Num_resource);                  //������Դ������ 
    for (i=1; i<=Num_resource; i++)
        fscanf(fp,"%d",&available[i]);              //�����ʼ��Դ�� 

    fscanf(fp,"%d",&Num_processes);                 //���������
    
    for (i=1; i<=Num_processes; i++)
    {
        for (j=1; j<=Num_resource; j++)
        {
            fscanf(fp,"%d",&max[i][j]);             //����MAX����
            need[i][j] = max[i][j];
            allocated[i][j] = 0;
        }
    }
    //�����ʼ��� 
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
        if(ans==1)                                               //�������
        {
            printf(" �C granted\n");
        }
        else if(ans==0)                                          //��������� ����������
        {
            printf("�C refused (extend the amount of available)\n");
        }
        else
        {
            printf(" �C refused (Deadlock is possible)\n");
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
