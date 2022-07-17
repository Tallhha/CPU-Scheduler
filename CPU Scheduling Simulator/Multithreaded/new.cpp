#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string>
#include<queue>
#include "proc.h"
using namespace std;

int main(int argc, char *argv[]){
	
	int pipe1[2];			//Pipe from new to ready
	pipe(pipe1);
	
	int i = 0, j = 0, arrival = 0, burst = 0, quant = -1, Proc_Count = 0;
	char info[200];			
	string s = "";

	int file = open("Sample_1_RR.txt", O_RDONLY);
	int bytes = read(file,info,sizeof(info));		//File Reading
	
	if(info[bytes-1]!='\n'){
		info[bytes] = '\n';
		info[bytes+1] = 'E';
	}
	else{
		info[bytes] = 'E';
	}

	int temp = 0;			//Counting Total Procs
	while(info[temp]!='E'){
		if(info[temp]=='P'){
			Proc_Count++;
		}
	temp++;
	}

	Proc P[Proc_Count];
	
	s = "";
	while(info[i]!= '\n'){	
		s+=info[i]; 
		i++;			//Algorithm Name
	}
	i++;
	
	cout<<s<<endl;
	int algo = 0;
	if(s[0] == 'R'){
		algo = 1;
	}
	else if(s[0] == 'F'){
		algo = 2;
	}
	else if(s[0] == 'S' && s[1] =='R'){
		algo = 3;
	}
	else if(s[0] == 'S' && s[1] == 'J' && s[2] == 'F'){
		algo = 4;
	}


	int back = dup(1);

	int f1 = open("processes.txt", O_WRONLY | O_APPEND);
	dup2(f1,1);
	cout<<s<<endl;
	close(f1);

	dup2(back,1);
	close(back);
	
	if(s[0] == 'R'){
		s = "";
		while(info[i]!= '\n'){	
			s+=info[i]; 
			i++;
		}			//Getting Quantum if RR
		i++;
		quant = stoi(s);	
	}

	while(info[i] != 'E'){
		s = "";				//Storing Arrival and Burst Time of all procs in Procs array
		while(info[i]!= '\n'){	
			s+=info[i]; 
			i++;
		}
		i++;
		
		s = "";
		while(info[i]!= '\n'){	
			s+=info[i]; 
			i++;
		}
		i++;
		arrival = stoi(s);

		s = "";
		while(info[i]!= '\n'){	
			s+=info[i]; 
			i++;
		}
		i++;
		
		burst = stoi(s);
		
		P[j].setProc(arrival,burst);
		P[j].setBurst1(burst);
		P[j].setPcount(j+1);
		P[j].setQuantum(quant);
		P[j].setAlgo(algo);
		//P[j].Display();
		j++;

	}
	
	//Sorting According to Arrival Time
	i = 0, j = 0, temp = 0;
	Proc swap;
	for (i = 1; i < Proc_Count; i++) {
		temp = P[i].getArrival();
		swap = P[i];
		for ( j=i; j>0 && temp < P[j-1].getArrival(); j--){
			P[j] = P[j-1];
		}
		P[j] = swap;
	}


//------------------------------IF SJF, Sorting According to shortest jobs and Time
	if(P[0].getAlgo() == 4){
		for(int k = 0; k < Proc_Count; k++){
			if(P[k].getArrival() == P[k+1].getArrival()){
				if(P[k].getBurst() > P[k+1].getBurst()){
					Proc swapB = P[k];
					P[k] = P[k+1];
					P[k+1] = swapB;
				}
			}
			else{
				break;
			}
		}

		int Check_Burst = P[0].getBurst();
		i = 0; j = 0;

		for(i = 1; i < Proc_Count; i++){
			for(j = i + 1; j < Proc_Count; j++){
				if(P[j].getArrival() <= Check_Burst){
					if(P[i].getBurst() > P[j].getBurst()){
						Proc swapB = P[i];
						P[i] = P[j];
						P[j] = swapB;
					}
				}	
			}
			Check_Burst += P[i].getBurst();				
		}
	}

//------------------------------------------^^SJF Sorting End^^------------

	for(int k = 0; k < Proc_Count; k++){
		P[k].Display();		//Displaying All Procs after Sorting acc to Algo
	}

	write(pipe1[1],P,sizeof(P));	//Sending to Ready

	pid_t pid = fork();
	if(pid > 0){
		//NEW
		wait(NULL);
	}	
	else if( pid == 0 ){
		//Ready
	
		int pipe1_copyr = dup(pipe1[0]);

		string s1 = to_string(pipe1_copyr);
		char *p1_r = new char[s1.size() + 1];
		s1.copy(p1_r, s1.size() + 1);
		p1_r[s1.size()] = '\0';

		string s2 = to_string(Proc_Count);
		char *pc = new char[s2.size() + 1];
		s2.copy(pc, s2.size() + 1);
		pc[s2.size()] = '\0';

		execlp("./ready", "ready" , p1_r , pc, NULL);
		cout<<"Exec Failed"<<endl;
	
	}

	//return 0;
	exit(0);

}
