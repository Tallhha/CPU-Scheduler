#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string>
#include<pthread.h>
#include "proc.h"
#include<queue>
using namespace std;


int main(int argc, char *argv[]){

	int p1_r = stoi(argv[1]);
	int proc_count = stoi(argv[2]);
	queue<Proc> Q;

	Proc P[proc_count];
	read(p1_r,P,sizeof(P));
	
	for(int k = 0; k < proc_count; k++){	
		Q.push(P[k]);
	}
	
	//Creating Pipes
	int pipe2[2];	//Ready to Running
	pipe(pipe2);

	int pipe2_copyw = dup(pipe2[0]);
	Proc send;

	int signal[2];	//To send signal to ready for next Proc after prev one is over
	pipe(signal);

	int signal_copy = dup(signal[1]);
	char* check = new char;


	int b_pipe[2];	//Blocked to Ready
	pipe(b_pipe);
	
	int ret = 0;
        fcntl(b_pipe[0], F_SETFL, O_NONBLOCK);	//Non Blocking 
	int bpipe_copy = dup(b_pipe[1]);
	Proc blockp;


	int qt_pipe[2];	//Running to Ready if Quantum switch
	pipe(qt_pipe);
	fcntl(qt_pipe[0], F_SETFL, O_NONBLOCK);	//Non Blocking
	int qtpipe_copy = dup(qt_pipe[1]);
	int ret1 = 0;
	Proc QT;

	int ex_pipe[2];	//To send Signal
	pipe(ex_pipe);
	fcntl(ex_pipe[0], F_SETFL, O_NONBLOCK);	//Non Blocking
	int exit_copy = dup(ex_pipe[1]);
	int ret2 = 0;
	char* end = new char;

	pid_t pid = fork();
	if(pid > 0){
		bool blocktest = true;
		if(P[0].getQuantum() != -1){
			cout<<endl<<"Quantum Time: "<<P[0].getQuantum()<<endl;
		}

		while(!Q.empty()){
			cout<<"---------------------------------------------------------------------------"<<endl;	
			cout<<"Ready."<<endl;
			blocktest = true;
			send = Q.front();
			Q.pop();

			write(pipe2[1],&send,sizeof(send));	//Send Proc to Running
			sleep(0);
		
			read(signal[0],check,sizeof(check));
	
			ret1 = read(qt_pipe[0],&QT,sizeof(QT));	
			//Recieve Proc from Running
			if(ret1 > 0){	

				Q.push(QT);
				cout<<"---------------------------------------------------------------------------"<<endl;
				cout<<"Quantum Ended. Recieved in Ready"<<endl;

				Proc test1 = Q.back();
				test1.Display();
		
			}
Test:			//Recieve Proc from Block
			ret = read(b_pipe[0],&blockp,sizeof(blockp));
			if(ret > 0){		

				Q.push(blockp);
				cout<<"---------------------------------------------------------------------------"<<endl;
				cout<<"New from block in Ready: \n";

				Proc test = Q.back();
				test.Display();

				//--------If SJF, Checking Burst of Proc from block and storing acc to that
				if(blockp.getAlgo() == 4){			
					int size = Q.size();
					if(size > 1){
						Proc SJF[size];
						int k = 0;
						while(!Q.empty()){
							SJF[k] = Q.front();
							Q.pop();
							k++;
						}
						for(int j = size - 1; j >= 0; j--){
							if(SJF[j+1].getBurst() < SJF[j].getBurst()){
								Proc swap = SJF[j+1];
								SJF[j+1] = SJF[j];
								SJF[j] = swap;
							}
						}
						for(int i = 0; i < size; i++){
							Q.push(SJF[i]);
						}
					}
				}
			}		
		}

		ret2 = read(ex_pipe[0],&end,sizeof(end));
		if(ret2 <= 0){
			if( blocktest == true){
				blocktest = false;
				cout<<"---------------------------------------------------------------------------"<<endl;
				cout<<"Waiting for Blocked Procs"<<endl;		
				sleep(15);
				goto Test;
			}
		}

		cout<<"---------------------------------------------------------------------------"<<endl;
		cout<<"All Proccesses Done."<<endl;	

	}

	else if(pid == 0){

		string s = to_string(pipe2_copyw);
		char *p2_r = new char[s.size() + 1];
		s.copy(p2_r, s.size() + 1);
		p2_r[s.size()] = '\0';
		
		s = to_string(signal_copy);
		char *sig_w = new char[s.size() + 1];
		s.copy(sig_w, s.size() + 1);
		sig_w[s.size()] = '\0';
		
		s = to_string(bpipe_copy);
		char *b_w = new char[s.size() + 1];
		s.copy(b_w, s.size() + 1);
		b_w[s.size()] = '\0';
		
		s = to_string(qtpipe_copy);
		char *qt_w = new char[s.size() + 1];
		s.copy(qt_w, s.size() + 1);
		qt_w[s.size()] = '\0';
		
		s = to_string(exit_copy);
		char *ex_w = new char[s.size() + 1];
		s.copy(ex_w, s.size() + 1);
		ex_w[s.size()] = '\0';
		

		execlp("./run", "run", p2_r, sig_w, b_w, qt_w,argv[2],ex_w, NULL);
		cout<<"Exec Failed."<<endl;
	}

	exit(0);
}
