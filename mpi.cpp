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
		return 0;
	else if (sample <= 66)
		return 1;
	else
		return 2;
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
        vector<string>config;
        config.push_back("X0");
        config.push_back("X1");
        config.push_back("0");
        config.push_back("1");
        config.push_back("0");
        config.push_back("X1");
        config.push_back("X0");
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
		int left,right;
		if (v == 0)
		{
			left = 1;
			right = 0;
		}
		
		else if (v == 1)
		{
			left = 0;
			right = 0;
			p.count++;
		}
		else
		{
			left = 0;
			right = 1;
		}
	MPI_Barrier(MPI_COMM_WORLD);  
	MPI_Send(&right, 1, MPI_INT, (myrank + 1)%nprocs, 0, MPI_COMM_WORLD); // send my id to next process in ring 
	int h;
	if(myrank==0)
	{
	MPI_Recv(&h, 1, MPI_INT, nprocs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	else
	{
	MPI_Recv(&h, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if(myrank==0)
	printf("node %d recieved from node %d = %d vote value\n",myrank,nprocs-1,h);
	else
	printf("node %d recieved from node %d = %d vote value\n",myrank,myrank-1,h);
	if (h == 1)
	   p.count++;
	   
	MPI_Barrier(MPI_COMM_WORLD);
	
	if(myrank==0)
	MPI_Send(&left, 1, MPI_INT, nprocs - 1, 0, MPI_COMM_WORLD);
	else
	MPI_Send(&left, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD); // send to previous neighbour in ring
	int l;
	MPI_Recv(&l, 1, MPI_INT, (myrank + 1)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv from previous neighbour in ring
	printf("node %d recieved from node %d = %d vote value\n",myrank,(myrank+1)%nprocs,h);
	if (l == 1)
	   p.count++;
	
	MPI_Barrier(MPI_COMM_WORLD);
	if (p.count == 1)
		p.state = "X0";
	else if (p.count == 2)
		p.state = "X1";
	else if(p.count==3)
		p.state ="1";
	else
		p.state ="0";
	printf("node %d has state = %s\n",myrank,p.state);
	MPI_Barrier(MPI_COMM_WORLD);
	//now check upto 6 distance
	char buffer[10];
	sprintf(buffer,p.state);
	char msg[10];
	vector<string>v1,v2;
	v1.push_back(p.state);
	
	
	for(int i=1;i<7;i++)
	{
	
	    MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank+i)%nprocs,0,MPI_COMM_WORLD);
	    if((myrank-i)>=0)
	    {MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank-i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("Node %d receives from node %d = %s\n",myrank,myrank-i,msg);
	    v1.push_back(msg);
	    }
	    else
	    {MPI_Recv(&msg, 10+ nprocs, MPI_CHAR,(nprocs+myrank-i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("Node %d receives from node %d =%s\n",myrank,nprocs+myrank-i,msg);
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
	  MPI_Send(&buffer,10+nprocs,MPI_CHAR,(myrank-i),0,MPI_COMM_WORLD);
	  else
	  MPI_Send(&buffer,10+nprocs,MPI_CHAR,(nprocs+myrank-i),0,MPI_COMM_WORLD);
	   
	   MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+i)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("Node %d receives from node %d =%s\n",myrank,(myrank+i)%nprocs,msg);
	    v2.push_back(msg);
	    MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	printf("size of vote from right to %d node = %d\n",myrank,v2.size());
	v2.insert(v2.end(),v1.begin(),v1.end());
	int flag=0;
	for(int i=0;i< v2.size();i++)
	{
		if(v2[i]==config[0])
		{
	  	int j=1;
	  	while(j<config.size()&&v2[i+j]==config[j])
	  		j++;
	  	if(j==config.size())
	   	flag=1;
		}
	}
	if(flag==1)
	{
	  if(p.state=="0")
	  	p.status=false;
	  if(p.state=="1")
	  	p.status=false;
	}
	
	/*
	if(p.state==1)
	{
	vector<string>v;
	if(myrank-3<0)
	MPI_Recv(&msg,10+nprocs, MPI_CHAR,(nprocs+myrank-3), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	else
	 MPI_Recv(&msg,10+nprocs, MPI_CHAR,myrank-3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	 v.push_back(msg);
	if(myrank-2<0)
	MPI_Recv(&msg,10+nprocs, MPI_CHAR,(nprocs+myrank-2), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	else
	 MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank-2), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	 v.push_back(msg);
	if(myrank-1<0)
	MPI_Recv(&msg,10+nprocs, MPI_CHAR,(nprocs+myrank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	else
	 MPI_Recv(&msg,10+nprocs, MPI_CHAR,myrank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	 v.push_back(msg);
	 v.push_back(p.state);
	 MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+1)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	 v.push_back(msg);
	 MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+2)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	 v.push_back(msg);
	 MPI_Recv(&msg,10+nprocs, MPI_CHAR,(myrank+3)%nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	 v.push_back(msg);
	}
	*/
	//MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, prev_rank, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	// Finalize the MPI environment.
	MPI_Finalize();
}
