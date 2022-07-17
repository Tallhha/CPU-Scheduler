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

int main(int argc, char *argv[]){

	int p4_r = stoi(argv[1]);
	int b_w = stoi(argv[2]);
	Proc block;
	queue<Proc> Q;

Top:	

	read(p4_r, &block, sizeof(block));	

	Q.push(block);


	srand(time(NULL));
	int tick = rand() % 15;
			//Blocked for a Random Time
	sleep(tick);

	Q.pop();
	write(b_w,&block,sizeof(block));
	
	goto Top;
	
	exit(0);

}
