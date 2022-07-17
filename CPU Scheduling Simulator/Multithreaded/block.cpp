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
#include<queue>
#include "proc.h"
using namespace std;

queue<Proc> IQ,OQ,PQ;		//Input,Output,Printer Queues

void* Input(void* arg) {
	
	Proc* Q = (Proc *)arg;
		
	IQ.push(*(Q));

	pthread_exit(NULL);
}

void* Output(void* arg) {
	
	Proc* Q = (Proc *)arg;
			
	OQ.push(*(Q));

	pthread_exit(NULL);
}

void* Printer(void* arg) {
	
	Proc* Q = (Proc *)arg;
			
	PQ.push(*(Q));

	pthread_exit(NULL);
}

int main(int argc, char *argv[]){

	int p4_r = stoi(argv[1]);
	int b_w = stoi(argv[2]);
	Proc block;
	pthread_t tid1, tid2, tid3;

Top:	

	read(p4_r, &block, sizeof(block));	

	srand(time(NULL));
	int num = rand()%3 + 1;
				//Random Number to Check which Queue
	if(num == 1){
		pthread_create(&tid1,NULL,Input,(void*)&block);
		pthread_join(tid1,NULL);

	}
	else if(num == 2){
		pthread_create(&tid2,NULL,Output,(void*)&block);
		pthread_join(tid2,NULL);

	}
	else if(num == 3){
		pthread_create(&tid3,NULL,Printer,(void*)&block);
		pthread_join(tid3,NULL);

	}


	srand(time(NULL));
	int tick = rand() % 15;
			//Blocked for a Random Time
	sleep(tick);

	if(!IQ.empty()){
		block = IQ.front();
		IQ.pop();
		write(b_w,&block,sizeof(block));

	}
	if(!OQ.empty()){
		block = OQ.front();
		OQ.pop();
		write(b_w,&block,sizeof(block));

	}
	if(!PQ.empty()){
		block = PQ.front();
		PQ.pop();
		write(b_w,&block,sizeof(block));

	}

	goto Top;
	
	exit(0);

}
