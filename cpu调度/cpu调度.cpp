#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
struct PCB {
	int pid;  			//进程标识
	float priority;  	//优先级
	int arrivetime;  	//到达时刻
	int cputime;  		//CPU Burst时间
	int finishtime;  	//完成时刻
	int waitingtime;  	//等待时间
	int runtime;		//截至目前已执行的时间
	int lefttime;		//截至目前还需要执行的时间（总时间-执行时间）
	int turnaround;  	//周转时间（完成时刻-到达时刻）
	int next;			//用来将PCB排成ReadyQueue
	int ii;
};

struct ReadyQueue {
	int currenttime;		//当前时刻
	int processescount;	//当前时刻队列中总的进程数目
	int firstprocess;	//当前时刻处于队列头的进程
	int lastprocess;		//当前时刻处于队列尾的进程
};
vector<PCB> processes;
void input() {
	int n;
	cin >> n;
	PCB t;
	while (n--) {
		cin >> t.pid >> t.arrivetime >> t.cputime >> t.priority;
		processes.push_back(t);
	}
}
void show() {
	int wait = 0, turnaround = 0;
	for (PCB &p : processes) {
		turnaround += p.turnaround;
	}
	wait = turnaround;
	for (PCB &p : processes) {
		wait -= p.cputime;
	}
	cout << "平均等待时间: " << ((double)wait) / processes.size() << endl;
	cout << "平均周转时间: " << ((double)turnaround) / processes.size() << endl;
}
void cal_turnaround() {
	for (PCB &p : processes) {
		p.turnaround = p.finishtime - p.arrivetime;
	}
}
bool camp_fcfs(PCB &a, PCB &b) {
	return a.arrivetime < b.arrivetime;
}
void fcfs() {
	sort(processes.begin(), processes.end(), camp_fcfs);
	int time = 0;
	cout << "FCFS" << endl;
	for (PCB &p : processes) {
		
		if (time < p.arrivetime)time = p.arrivetime;
		p.waitingtime = time - p.arrivetime;
		cout << time << ":"<<p.pid << endl;
		time += p.cputime;
		p.finishtime = time;
	}
	cal_turnaround();
	show();
}
bool camp_rr(PCB &a, PCB &b) {
	return a.arrivetime < b.arrivetime;
}
void RR(int timeslip) {
	auto p = processes;
	sort(processes.begin(), processes.end(), camp_rr);
	int time = 0;
	cout << "RR:"<<timeslip << endl;
	queue<PCB> q;
	for (int i = 0; i < processes.size(); i++) {
		processes[i].lefttime = processes[i].cputime;
		processes[i].runtime = 0;
		processes[i].ii = 0;
	}
	int pleft = processes.size();
	while (true) {
		if (!q.empty()) {
			if (q.front().runtime == 0)
			{
				for (int i = 0; i < processes.size(); i++) {
					if (processes[i].pid == q.front().pid)
					{
						processes[i].waitingtime = time;
					}
				}
			}
			if (q.front().lefttime <= timeslip) {
				cout << time << ":" << q.front().pid<< endl;
				time += q.front().lefttime;
				q.front().runtime += q.front().lefttime;
				q.front().lefttime = 0;
			}
			else {
				cout << time << ":" << q.front().pid << endl;
				time += timeslip;
				q.front().runtime += timeslip;
				q.front().lefttime -= timeslip;
			}

		}
		for (int i = 0; i < processes.size(); i++) {
			if (processes[i].arrivetime <= time && processes[i].ii == 0)
			{
				processes[i].ii = 1;
				q.push(processes[i]);
				pleft--;
			}
		}
		auto a = q.front();
		if (a.lefttime != 0 && !q.empty()) {
			q.pop();
			q.push(a);
		}
		else {
			q.pop();
			for (int i = 0; i < processes.size(); i++) {
				if (processes[i].pid == a.pid)
				{
					processes[i].finishtime = time;
				}
			}
		}
		if (q.empty() && pleft == 0)break;
	}
	cal_turnaround();
	show();
	processes = p;
}
bool camp_sjf(PCB &a, PCB &b) {
	return a.cputime < b.cputime;
}
void sjf() {
	sort(processes.begin(), processes.end(), camp_fcfs);
	int time = 0;
	cout << "SJF" << endl;
	int pleft = processes.size();
	for (int i = 0; i < processes.size(); i++) {
		processes[i].lefttime = processes[i].cputime;
		processes[i].runtime = 0;
		processes[i].ii = 0;
	}
	while (true) {
		if (pleft == 0)break;
		vector<PCB> t;
		for (PCB &p : processes) {
			if (p.arrivetime <= time && p.ii == 0)
			{
				t.push_back(p);
			}
		}
		sort(t.begin(), t.end(), camp_sjf);
		if (pleft == 0)break;
		if (t.size() == 0) {
			time++; continue;
		}
		t[0].waitingtime = time - t[0].arrivetime;
		cout << time << ":" << t[0].pid << endl;
		time += t[0].cputime;
		
		for (PCB &p : processes) {
			if (p.pid == t[0].pid)
			{
				p.ii = 1;
				p.finishtime = time;
			}
		}
		pleft--;
	}
	
	cal_turnaround();
	show();
}
bool camp_srtf(PCB &a, PCB &b) {
	return a.lefttime < b.lefttime;
}
void srtf() {
	for (int i = 0; i < processes.size(); i++) {
		processes[i].lefttime = processes[i].cputime;
		processes[i].runtime = 0;
		processes[i].ii = 0;
	}
	cout << "SRTF" << endl;
	vector<PCB> jq;
	int time = 0;
	int pleft = processes.size();
	int pre = -1;
	while (true) {
		
		for (int i = 0; i < processes.size(); i++) {//加入
			if (processes[i].arrivetime <= time && processes[i].ii == 0)
			{
				processes[i].ii = 1;
				jq.push_back(processes[i]);
				pleft--;
			}
		}
		sort(jq.begin(), jq.end(), camp_srtf);
		if (jq.size() > 0) {//执行
			if (jq[0].pid != pre) {
				cout << time << ":" << jq[0].pid<<endl;
				pre = jq[0].pid;
			}
			jq[0].runtime++;
			jq[0].lefttime--;
		}
		time++;
		if (jq.size() > 0 && jq[0].lefttime == 0) {
			for (int i = 0; i < processes.size(); i++) {
				if (processes[i].pid == jq[0].pid)
				{
					processes[i].finishtime = time;
				}
			}
			jq.erase(jq.begin(), jq.begin() + 1);
		}
		if (jq.size() == 0 && pleft == 0) {
			break;
		}	
	}
	cal_turnaround();
	show();
}
bool camp_pri(PCB &a, PCB &b) {
	return a.priority < b.priority;
}
void pri_np() {
	sort(processes.begin(), processes.end(), camp_fcfs);
	int time = 0;
	cout << "非抢占优先级" << endl;
	int pleft = processes.size();
	for (int i = 0; i < processes.size(); i++) {
		processes[i].lefttime = processes[i].cputime;
		processes[i].runtime = 0;
		processes[i].ii = 0;
	}
	while (true) {
		if (pleft == 0)break;
		vector<PCB> t;
		for (PCB &p : processes) {
			if (p.arrivetime <= time && p.ii == 0)
			{
				t.push_back(p);
			}
		}
		sort(t.begin(), t.end(), camp_pri);
		if (pleft == 0)break;
		if (t.size() == 0) {
			time++; continue;
		}
		t[0].waitingtime = time - t[0].arrivetime;
		cout << time << ":" << t[0].pid << endl;
		time += t[0].cputime;

		for (PCB &p : processes) {
			if (p.pid == t[0].pid)
			{
				p.ii = 1;
				p.finishtime = time;
			}
		}
		pleft--;
	}

	cal_turnaround();
	show();
}
void pri_p() {
	cout << "抢占优先级" << endl;
	for (int i = 0; i < processes.size(); i++) {
		processes[i].lefttime = processes[i].cputime;
		processes[i].runtime = 0;
		processes[i].ii = 0;
	}

	vector<PCB> jq;
	int time = 0;
	int pleft = processes.size();
	int pre = -1;
	while (true) {

		for (int i = 0; i < processes.size(); i++) {//加入
			if (processes[i].arrivetime <= time && processes[i].ii == 0)
			{
				processes[i].ii = 1;
				jq.push_back(processes[i]);
				pleft--;
			}
		}
		sort(jq.begin(), jq.end(), camp_pri);
		if (jq.size() > 0) {//执行
			if (jq[0].pid != pre) {
				cout << time << ":" << jq[0].pid << endl;
				pre = jq[0].pid;
			}
			jq[0].runtime++;
			jq[0].lefttime--;
		}
		time++;
		if (jq.size() > 0 && jq[0].lefttime == 0) {
			for (int i = 0; i < processes.size(); i++) {
				if (processes[i].pid == jq[0].pid)
				{
					processes[i].finishtime = time;
				}
			}
			jq.erase(jq.begin(), jq.begin() + 1);
		}
		if (jq.size() == 0 && pleft == 0) {
			break;
		}
	}
	cal_turnaround();
	show();
}
bool camp_hrrn(PCB &a, PCB &b) {
	return (a.waitingtime + a.cputime)/ (double)a.cputime > (b.waitingtime + b.cputime) / (double)b.cputime;
}
void hrrn() {
	sort(processes.begin(), processes.end(), camp_fcfs);
	int time = 0;
	cout << "高响应比" << endl;
	int pleft = processes.size();
	for (int i = 0; i < processes.size(); i++) {
		processes[i].lefttime = processes[i].cputime;
		processes[i].runtime = 0;
		processes[i].ii = 0;
	}
	while (true) {
		if (pleft == 0)break;
		vector<PCB> t;
		for (PCB &p : processes) {
			if (p.arrivetime <= time && p.ii == 0)
			{
				t.push_back(p);
			}
		}
		for (PCB &l : t) {
			l.waitingtime = time - l.arrivetime;
		}
		sort(t.begin(), t.end(), camp_hrrn);
		if (pleft == 0)break;
		if (t.size() == 0) {
			time++; continue;
		}
		t[0].waitingtime = time - t[0].arrivetime;
		cout << time << ":" << t[0].pid << endl;
		time += t[0].cputime;

		for (PCB &p : processes) {
			if (p.pid == t[0].pid)
			{
				p.ii = 1;
				p.finishtime = time;
			}
		}
		pleft--;
	}

	cal_turnaround();
	show();
}
int main() {
	freopen("input1.txt","r",stdin);
	input();
	fcfs();
	RR(5);
	RR(10);
	sjf();
	srtf();
	pri_np();
	pri_p();
	hrrn();
	return 0;
}