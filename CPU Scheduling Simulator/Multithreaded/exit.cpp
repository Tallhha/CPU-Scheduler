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

	int p3_r = stoi(argv[1]);
	int total = stoi(argv[2]);
	int ex_w = stoi(argv[3]);

	Proc end;
	Proc store[total];
	bool exx = true;
	int turn = 0, wt = 0, burst = 0, i = 0;
	float avgturn = 0.0, avgwait = 0.0, throughput = 0.0;

End:
	read(p3_r, &end, sizeof(end));	
	
	cout<<"---------------------------------------------------------------------------"<<endl;
	cout<<"Exit"<<endl;
	end.Display();

	burst = end.getBurst();
	wt = end.getWaiting();

	turn = burst + wt;		//Calculating Turn Around Time
	end.setTurnAround(turn);

	end.TW();
	store[i] = end;
	i++;

	if(i == total){
		cout<<"---------------------------------------------------------------------------"<<endl;
		cout<<"\t\t\t Gantts Chart: "<<endl<<endl;
		for(int j = 0; j < total; j++){
			store[j].Display();
			store[j].TW();
			cout<<endl;
			avgwait += store[j].getWaiting();
			avgturn += store[j].getTurnAround();
			throughput += store[j].getBurst1();
		}
			//^ Calculating Avg Wait, Avg TurnAround, and Through Put v
		avgwait /= total;
		avgturn /= total;	
		throughput /= total;
			
			//Writing them in File.
		cout<<"Inserting Avg, Turn Around and Through Put in file."<<endl;
		int file = open("processes.txt", O_WRONLY | O_APPEND);
		dup2(file,1);

		cout<<"Average Waiting Time: "<<avgwait<<endl;
		cout<<"Average Turn Around Time: "<<avgturn<<endl;
		cout<<"Through Put: "<<throughput<<endl<<endl;

		write(ex_w,"1",1);
		exit(0);
	}

	goto End;

}
