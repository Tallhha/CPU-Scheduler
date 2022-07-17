#include<iostream>
using namespace std;

struct Proc {

int arrival,burst,quantum,turnaround,waiting,burst1,Pcount,algo;

Proc():arrival(0),burst(0),quantum(-1),turnaround(0),waiting(0),burst1(0),Pcount(0),algo(0){}

Proc(int arr, int b, int q, int t, int w, int b1, int pc, int al):arrival(arr), burst(b), quantum(q), turnaround(t), waiting(w),burst1(b1),Pcount(pc), algo(al){}

Proc& operator=(const Proc &right){
	
	this->arrival = right.arrival;
	this->burst = right.burst;
	this->quantum = right.quantum;
	this->turnaround = right.turnaround;
	this->waiting = right.waiting;
	this->burst1 = right.burst1;
	this->Pcount = right.Pcount;
	this->algo = right.algo;
	return *this;

}

void setProc(int arr, int b){

	this->arrival = arr;
	this->burst = b;
}

void setArrival(int arr){
	this->arrival = arr;
}

void setBurst(int b){
	this->burst = b;
}

void setBurst1(int b1){
	this->burst1 = b1;
}

void setQuantum(int q){
	this->quantum = q;
}


void setTurnAround(int t){
	this->turnaround = t;
}

void setWaiting(int w){
	this->waiting = w;
}

void setPcount(int pc){
	this->Pcount = pc;
}

void setAlgo(int al){
	this->algo = al;
}


int getAlgo(){
	return algo;
}

int getPcount(){
	return Pcount;
}

int getTurnAround(){
	return turnaround;
}

int getWaiting(){
	return waiting;
}


int getQuantum(){
	return quantum;
}

int getArrival(){
	return arrival;
}

int getBurst(){
	return burst;
}

int getBurst1(){
	return burst1;
}

void Display(){

	cout<<"Proc Number: "<<Pcount<<endl;
	cout<<"Arrival: "<<arrival<<"\t\t"<<"Burst: "<<burst<<endl;

}
	
void TW(){
	cout<<"Turn Around Time: "<<turnaround<<endl<<"Waiting Time: "<<waiting<<endl;
}

};

