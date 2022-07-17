#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
using namespace std;

int main(){
					//CPU SCHEDULER 
	pid_t pid = fork();

	if( pid > 0 ){
		//Scheduler
		wait(NULL);
	
	}	

	else if( pid == 0 ){
		//New
		cout<<"New Proccess"<<endl;
	
		execlp("./new", "new" , "Test" , NULL);
		cout<<"Exec Failed"<<endl;

	}
	else{
		//Fork Failed
		cout<<"Fork() Failed."<<endl;
	}

	return 0;
}
