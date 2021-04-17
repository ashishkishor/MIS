#include <stdlib.h>
#include<string.h>
#include <stdio.h>
#include <mpi.h>
#include<iostream>
#include <ctime> 
#include<bits/stdc++.h>
using namespace std;

int generateRandomNumber(int rank, int size) {
	int s;
	int *rNums;
	rNums = new int[size];

	for (int i = 0; i < size; i++)
	{
		rNums[i] = rand(); // aspires to be random number
	}

	s = rNums[rank];
	return s;
}
int vote()
{
	int sample = rand() % 100;
	cout<<sample<<endl;
	if (sample <= 33)
		return 0;//vote to left
	else if (sample <= 66)
		return 1;//vote to himself
	else
		return 2;//vote to right
}
class Process {
public:
	int rank; // MPI generated
	int identifier; // assigned by this code
	bool status; // active or passive
	char *state;  //X0,X1,1 or 0
	//int left;
	//int right;
	int count; //for counting votes
	Process(int r, int nprocs) {
		rank = r;
		identifier = generateRandomNumber(rank, nprocs);
		count = 0;
		state = "?";
		status=true;
	}
	//vector<string>r1,r2;

};


int main(int argc, char** argv)
{
        vector<int>config;
        config.push_back(2);
        config.push_back(1);
        config.push_back(2);
        config.push_back(1);
        config.push_back(0);
        config.push_back(1);
        config.push_back(0);
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int nprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	// Get the rank of the process
	int myrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// Get the name of the processor
	//char processor_name[MPI_MAX_PROCESSOR_NAME];
	Process p(myrank, nprocs);
	myrank=p.rank;
	srand(myrank);
	//int name_len;
	//MPI_Get_processor_name(processor_name, &name_len);

	// Print off a hello world message
	printf("I am process %d of %d. Unique ID = %d\n", myrank, nprocs, p.identifier);
	MPI_Barrier(MPI_COMM_WORLD);
		int v=vote();
		printf("%d vote value is =%d\n",myrank,v);
		
	MPI_Barrier(MPI_COMM_WORLD);
	//now check upto 6 distance
	int buffer=v;
	//sprintf(buffer,p.state);
	int msg;
	vector<int>v1,v2;
	v1.push_back(v);
	
	
	for(int i=1;i<7;i++)
	{
	
	    MPI_Send(&buffer,1,MPI_INT,(myrank+i)%nprocs,0,MPI_COMM_WORLD);
	    if((myrank-i)>=0)
	    {MPI_Recv(&msg,1, MPI_INT,(myrank-i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("Node %d receives from node %d = %d\n",myrank,myrank-i,msg);
	    v1.push_back(msg);
	    }
	    else
	    {MPI_Recv(&msg,1, MPI_INT,(nprocs+myrank-i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("Node %d receives from node %d =%d\n",myrank,nprocs+myrank-i,msg);
	    v1.push_back(msg);
	    }
	    MPI_Barrier(MPI_COMM_WORLD);
	}
        MPI_Barrier(MPI_COMM_WORLD);
	reverse(v1.begin(),v1.end());
	
	printf("size of vote from left to %d node = %d\n",myrank,v1.size());
	
	for(int i=1;i<7;i++)
	{
	  if((myrank-i)>=0)
	  MPI_Send(&buffer,1,MPI_INT,(myrank-i),0,MPI_COMM_WORLD);
	  else
	  MPI_Send(&buffer,1,MPI_INT,(nprocs+myrank-i),0,MPI_COMM_WORLD);
	   
	   MPI_Recv(&msg,1, MPI_INT,(myrank+i)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("Node %d receives from node %d =%d\n",myrank,(myrank+i)%nprocs,msg);
	    v2.push_back(msg);
	    MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	printf("size of vote from right to %d node = %d\n",myrank,v2.size());
	int flag=0;
	if(v==0)
	{
	    if(v1==config)
	    {
	    flag=1;
	    char msg[10],buffer[10];
	    p.state="X0";
	    sprintf(buffer,"X1");
	    if(myrank-1>=0)
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(nprocs+myrank-1),0,MPI_COMM_WORLD);
	    sprintf(buffer,"0");
	    if(myrank-2>=0)
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank-2),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(nprocs+myrank-2),0,MPI_COMM_WORLD);
	    sprintf(buffer,"1");
	    if(myrank-3>=0)
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank-3),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(nprocs+myrank-3),0,MPI_COMM_WORLD);
	    sprintf(buffer,"0");
	    if(myrank-4>=0)
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank-4),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(nprocs+myrank-4),0,MPI_COMM_WORLD);
	    sprintf(buffer,"X1");
	    if(myrank-5>=0)
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank-5),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(nprocs+myrank-5),0,MPI_COMM_WORLD);
	    sprintf(buffer,"X0");
	    if(myrank-6>=0)
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank-6),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(nprocs+myrank-6),0,MPI_COMM_WORLD);
	    
	    }
	    
	}
	    {
	    char msg[10];
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+1)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+2)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+3)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+4)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+5)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+6)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	    }
	MPI_Barrier(MPI_COMM_WORLD);
	if(v==2)
	{
	    if(v2==config)
	    {
	    flag=1;
	    char buffer[10];
	    p.state="X0";
	    sprintf(buffer,"X1");
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	    
	    sprintf(buffer,"0");
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank+2)%nprocs,0,MPI_COMM_WORLD);
	    sprintf(buffer,"1");
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank+3)%nprocs,0,MPI_COMM_WORLD);
	    sprintf(buffer,"0");
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank+4)%nprocs,0,MPI_COMM_WORLD);
	    sprintf(buffer,"X1");
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank+5)%nprocs,0,MPI_COMM_WORLD);
	    sprintf(buffer,"X0");
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank+6)%nprocs,0,MPI_COMM_WORLD);
	    
	    }
	    
	}
	    {
	    char msg[10];
	    if(myrank-1>=0)
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    else
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(nprocs+myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	     if(myrank-2>=0)
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    else
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(nprocs+myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	     if(myrank-3>=0)
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    else
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(nprocs+myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	     if(myrank-4>=0)
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    else
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(nprocs+myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	     if(myrank-5>=0)
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    else
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(nprocs+myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	     if(myrank-6>=0)
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    else
	    MPI_Recv(&msg,10+nprocs, MPI_CHAR,(nprocs+myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    p.state=msg;
	    }
	MPI_Barrier(MPI_COMM_WORLD);
	printf("node %d has state %s\n",myrank,p.state); 

	// Finalize the MPI environment.
	MPI_Finalize();
}