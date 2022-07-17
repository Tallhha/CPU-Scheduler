#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string>

#include "proc.h"
using namespace std;

int main(int argc, char *argv[]){
	
	int p2_r = stoi(argv[1]);
	int sig_w = stoi(argv[2]);
	int qt_w = stoi(argv[4]);
	int totalp = stoi(argv[5]);

	Proc recieved, send;
	bool flag = true, eflag = true;

	int pipe4[2];
	pipe(pipe4);
	int pipe4_copyw = dup(pipe4[0]);

	int pipe3[2];
	pipe(pipe3);
	int pipe3_copyw = dup(pipe3[0]);

	pid_t pid;

	int arr_t = 0, burst_t = 0,q_t = -1, tick = 0, num = 0;
	int wt = 0, blockwt = 0, pcount = 0, rem = 0, rem1 = 0, exitp = 0;
	bool first = true;
Run:

	read(p2_r, &recieved, sizeof(recieved));
	recieved.Display();
	cout<<"---------------------------------------------------------------------------"<<endl;

	arr_t = recieved.getArrival();
	burst_t = recieved.getBurst();
	q_t = recieved.getQuantum();
	blockwt = recieved.getWaiting();

	pcount++;

	if(first){
		wt = arr_t;
		recieved.setWaiting(wt);
		first = false;
		rem = recieved.getQuantum();
	}				//Setting Waiting Time
	else{
		recieved.setWaiting(wt-arr_t-blockwt);
	}
	cout<<"Running."<<endl;
					//Counting Waiting Time, Exucting Proc
	while(burst_t != 0){

		cout<<"Burst Remaining: "<<burst_t - 1<<endl;
	
		sleep(1);
		burst_t--;
		tick++;
		wt++;		

		if(q_t != -1){
			q_t--;
			cout<<"Quantum Remaining: "<<q_t<<endl;
		}
				//^^ If Quantum vv			
		if(q_t == 0 && burst_t != 0){
			cout<<"Quantum Over. Sending Back.\n";
										
			if(pcount > totalp){
				pcount = 0;
				rem += rem;
				totalp -= exitp;
			}

			recieved.setWaiting(rem);
			recieved.setProc(arr_t,burst_t);
			write(qt_w,&recieved,sizeof(recieved));
			goto NextProc;
		}
				//Checking Block after 5 ticks
		if(tick == 5 && burst_t != 0){
			srand(time(NULL));
			int num = rand() % 2;
					
			if(pcount == totalp){
				num = 1;
			}
			cout<<"Num is: "<<num<<endl;					
			if(num == 1){
				continue;
			}
			else{
				//Num = 0, Enters Blocked State
				if(pcount > totalp){
					pcount = 0;
					rem1 += 5;
     					totalp -= exitp;
				}
	
				recieved.setWaiting(rem1);
				recieved.setProc(arr_t,burst_t);
						
				if(flag == true){
					flag = false;
					pid = fork();
				}

				if(pid > 0){
					write(pipe4[1],&recieved,sizeof(recieved));					
					cout<<"---------------------------------------------------------------------------"<<endl;

					cout<<"Blocked"<<endl;
					recieved.Display();

					sleep(0);
					goto NextProc;

				}
				else if(pid == 0){

					string s = to_string(pipe4_copyw);
					char *p4_r = new char[s.size() + 1];
					s.copy(p4_r, s.size() + 1);
					p4_r[s.size()] = '\0';
		
					execlp("./block", "block", p4_r, argv[3], NULL);
					cout<<"Exec Failed."<<endl;
				}
			}
		}
	}

	//Exit State	
	if(eflag == true){
		eflag = false;	
		pid = fork();
	}

	if(pid > 0){
		exitp++;
		recieved.setBurst(recieved.getBurst1());
		write(pipe3[1],&recieved,sizeof(recieved));
		sleep(1);

		goto NextProc;

	}
	else if(pid == 0){

		string s = to_string(pipe3_copyw);
		char *p3_r = new char[s.size() + 1];
		s.copy(p3_r, s.size() + 1);
		p3_r[s.size()] = '\0';
		
		execlp("./exit", "exit", p3_r, argv[5],argv[6], NULL);
		cout<<"Exec Failed."<<endl;
	}

NextProc:
	tick = 0;
	write(sig_w,"1",1);
	sleep(0);

	goto Run;

	exit(0);

}
