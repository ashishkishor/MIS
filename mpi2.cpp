#include <stdlib.h>
#include<string.h>
#include <stdio.h>
#include <mpi.h>
#include<iostream>
#include <ctime> 
#include<bits/stdc++.h>
#include <unistd.h>
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
	MPI_Request request;
	MPI_Status status;
	// Get the number of processes
	int nprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	// Get the rank of the process
	int myrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// Get the name of the processor
	//char processor_name[MPI_MAX_PROCESSOR_NAME];
	Process p(myrank, nprocs);
	p.state="?";
	myrank=p.rank;
	srand(myrank);
	//int name_len;
	//MPI_Get_processor_name(processor_name, &name_len);

	// Print off a hello world message
	printf("I am process %d of %d. Unique ID = %d\n", myrank, nprocs, p.identifier);
	MPI_Barrier(MPI_COMM_WORLD);
	int v;
	v=vote();
	if(myrank==0)
	v=2;
	if(myrank==1)
	v=1;
	if(myrank==2)
	v=2;
	if(myrank==3)
	v=1;
	if(myrank==4)
	v=0;
	if(myrank==5)
	v=1;
	if(myrank==6)
	v=0;
	
		//int v=vote();
		
		printf("%d vote value is =%d\n",myrank,v);
		
	MPI_Barrier(MPI_COMM_WORLD);
	//now check upto 6 distance
	int buffer=v;
	//sprintf(buffer,p.state);
	int msg;
	vector<int>v1,v2;
	v1.push_back(v);
	
	//Phase Drawing
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
	for(int i=0;i<v1.size();i++)
	{
	printf("%d ",v1[i]);
	if(i==v1.size()-1)
	printf("for node %d",myrank);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	
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
	v2.push_back(v);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("size of vote from right to %d node = %d\n",myrank,v2.size());
	for(int i=0;i<v2.size();i++)
	{
	printf("%d ",v2[i]);
	if(i==v2.size()-1)
	printf("for node %d",myrank);
	}
	printf("\n");
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
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(nprocs+myrank-1),0,MPI_COMM_WORLD);
	    memset(buffer,0,sizeof(buffer));
	    sprintf(buffer,"0");
	    if(myrank-2>=0)
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(myrank-2),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(nprocs+myrank-2),0,MPI_COMM_WORLD);
	    memset(buffer,0,sizeof(buffer));
	    sprintf(buffer,"1");
	    if(myrank-3>=0)
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(myrank-3),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(nprocs+myrank-3),0,MPI_COMM_WORLD);
	    memset(buffer,0,sizeof(buffer));
	    sprintf(buffer,"0");
	    if(myrank-4>=0)
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(myrank-4),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(nprocs+myrank-4),0,MPI_COMM_WORLD);
	    memset(buffer,0,sizeof(buffer));
	    sprintf(buffer,"X1");
	    if(myrank-5>=0)
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(myrank-5),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(nprocs+myrank-5),0,MPI_COMM_WORLD);
	    memset(buffer,0,sizeof(buffer));
	    sprintf(buffer,"X0");
	    if(myrank-6>=0)
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(myrank-6),0,MPI_COMM_WORLD);
	    else
	    MPI_Send(&buffer,strlen(buffer)+1,MPI_CHAR,(nprocs+myrank-6),0,MPI_COMM_WORLD);
	    
	    }
	    
	}
	if(flag==0)
	    {
	    char msg[10];
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank+1)%nprocs, 0, MPI_COMM_WORLD,&request);
	    printf("node %d received from node %d ,the state =%s\n",myrank,myrank+1,msg);
	    if(strlen(msg)>0)
	    p.state=msg;
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank+2)%nprocs, 0, MPI_COMM_WORLD,&request);
	    printf("node %d received from node %d ,the state =%s\n",myrank,myrank+2,msg);
	    if(strlen(msg)>0)
	    p.state=msg;
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank+3)%nprocs, 0, MPI_COMM_WORLD, &request);
	    printf("node %d received from node %d ,the state =%s\n",myrank,myrank+3,msg);
	    if(strlen(msg)>0)
	    p.state=msg;
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank+4)%nprocs, 0, MPI_COMM_WORLD, &request);
	    printf("node %d received from node %d ,the state =%s\n",myrank,myrank+4,msg);
	    if(strlen(msg)>0)
	    p.state=msg;
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank+5)%nprocs, 0, MPI_COMM_WORLD, &request);
	    printf("node %d received from node %d ,the state =%s\n",myrank,myrank+5,msg);
	    if(strlen(msg)>0)
	    p.state=msg;
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank+6)%nprocs, 0, MPI_COMM_WORLD, &request);
	    printf("node %d received from node %d ,the state =%s\n",myrank,myrank+6,msg);
	    if(strlen(msg)>0)
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
	    MPI_Send(&buffer,10,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	    
	    sprintf(buffer,"0");
	    MPI_Send(&buffer,10,MPI_CHAR,(myrank+2)%nprocs,0,MPI_COMM_WORLD);
	    sprintf(buffer,"1");
	    MPI_Send(&buffer,10,MPI_CHAR,(myrank+3)%nprocs,0,MPI_COMM_WORLD);
	    sprintf(buffer,"0");
	    MPI_Send(&buffer,10,MPI_CHAR,(myrank+4)%nprocs,0,MPI_COMM_WORLD);
	    sprintf(buffer,"X1");
	    MPI_Send(&buffer,10,MPI_CHAR,(myrank+5)%nprocs,0,MPI_COMM_WORLD);
	    sprintf(buffer,"X0");
	    MPI_Send(&buffer,10,MPI_CHAR,(myrank+6)%nprocs,0,MPI_COMM_WORLD);
	    
	    }
	    
	}
	if(flag==0)
	    {
	    char msg[10];
	    if(myrank-1>=0)
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank-1), 0, MPI_COMM_WORLD,&request);
	    else
	    MPI_Irecv(&msg,10, MPI_CHAR,(nprocs+myrank-1), 0, MPI_COMM_WORLD, &request);
	    if(strlen(msg)>0)
	    p.state=msg;
	     if(myrank-2>=0)
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank-2), 0, MPI_COMM_WORLD, &request);
	    else
	    MPI_Irecv(&msg,10, MPI_CHAR,(nprocs+myrank-2), 0, MPI_COMM_WORLD, &request);
	    if(strlen(msg)>0)
	    p.state=msg;
	     if(myrank-3>=0)
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank-3), 0, MPI_COMM_WORLD, &request);
	    else
	    MPI_Irecv(&msg,10, MPI_CHAR,(nprocs+myrank-3), 0, MPI_COMM_WORLD, &request);
	    if(strlen(msg)>0)
	    p.state=msg;
	     if(myrank-4>=0)
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank-4), 0, MPI_COMM_WORLD, &request);
	    else
	    MPI_Irecv(&msg,10, MPI_CHAR,(nprocs+myrank-4), 0, MPI_COMM_WORLD,&request);
	    if(strlen(msg)>0)
	    p.state=msg;
	     if(myrank-5>=0)
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank-5), 0, MPI_COMM_WORLD, &request);
	    else
	    MPI_Irecv(&msg,10, MPI_CHAR,(nprocs+myrank-5), 0, MPI_COMM_WORLD, &request);
	    if(strlen(msg)>0)
	    p.state=msg;
	     if(myrank-6>=0)
	    MPI_Irecv(&msg,10, MPI_CHAR,(myrank-6), 0, MPI_COMM_WORLD, &request);
	    else
	    MPI_Irecv(&msg,10, MPI_CHAR,(nprocs+myrank-6), 0, MPI_COMM_WORLD, &request);
	    if(strlen(msg)>0)
	    p.state=msg;
	    }

	printf("node %d has state %s\n",myrank,p.state); 
	MPI_Barrier(MPI_COMM_WORLD);

	//Phase 2 :Expansion process
	if(myrank==0)
	printf("Phase 2\n");
	
	
	char exp[10],msg1[10];
	sprintf(exp,p.state);
	MPI_Send(&exp,10,MPI_CHAR,(myrank+4)%nprocs,0,MPI_COMM_WORLD);
	printf("node %d has sent message to %d = %s\n",myrank,(myrank+4)%nprocs,exp);
	MPI_Barrier(MPI_COMM_WORLD);
	if(myrank-4>=0)
	{
	MPI_Irecv(&msg1,10, MPI_CHAR,(myrank-4), 0, MPI_COMM_WORLD,&request);
	printf("node %d has received message from %d = %s\n",myrank,myrank-4,&request);
	}
	
	else
	{MPI_Irecv(&msg1,10, MPI_CHAR,(nprocs+myrank-4), 0, MPI_COMM_WORLD,&request);
	
	printf("node %d has received message from %d = %s\n",myrank,nprocs+myrank-4,msg1);
	}
	
        /*
	if(myrank-4>=0)
	MPI_Send(&exp,10,MPI_CHAR,(myrank-4),0,MPI_COMM_WORLD);
	else
	MPI_Send(&exp,10,MPI_CHAR,(nprocs+myrank-4),0,MPI_COMM_WORLD);
	*/
       /*
	 char rbuf[10];
	 printf("efew %s\n",rbuf);
	 if(myrank-4>=0)
	 {
	 MPI_Irecv(&rbuf,10,MPI_CHAR,(myrank-4),0, MPI_COMM_WORLD,&request);
	 if(strlen(rbuf)>0)
	 {
	 printf("node %d has received message from %d = %s\n",myrank,myrank-4,rbuf);
	 }
	 }
	 else
	 {
	 MPI_Irecv(&rbuf,10, MPI_CHAR,(nprocs+myrank-4), 0, MPI_COMM_WORLD,&request);
	 if(strlen(rbuf)>0)
	 printf("node %d has received message from %d=  %s\n",myrank,nprocs+myrank-4,rbuf);
	 }
	 MPI_Barrier(MPI_COMM_WORLD);
	 if(myrank==0)
	printf("ashish\n");
	
	 
	 if(p.state=="X0"&&msg1=="?")
	{
	 	char m[10];
	 	sprintf(m,p.state);
	 	//printf("node %d has sent message to node %d\n",myrank,myrank-4);
	 	if(myrank-4>=0)
	 MPI_Send(&m,10,MPI_CHAR,(myrank-4),0,MPI_COMM_WORLD);
	 	else
	 MPI_Send(&m,10,MPI_CHAR,(nprocs+myrank-4),0,MPI_COMM_WORLD);
	}
	 MPI_Barrier(MPI_COMM_WORLD);
	 if(p.state=="X0")
	 {
	 char buff[10];
	 MPI_Irecv(&buff,10, MPI_CHAR,(myrank+4)%nprocs, 0, MPI_COMM_WORLD,&request);
	 */
	// printf("node %d has receieved message from node %d\n",myrank,(myrank+4)%nprocs);
	 
	 
	 if(p.state=="X0"&&msg1=="X1")
	 {
	 p.state="0";
	 char b[10];
	 sprintf(b,"X1");
	 MPI_Send(&b,10,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	 sprintf(b,"X0");
	 MPI_Send(&b,10,MPI_CHAR,(myrank+2)%nprocs,0,MPI_COMM_WORLD);
	 sprintf(b,"1");
	 if(myrank-1>=0)
	 MPI_Send(&b,10,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	 else
	 MPI_Send(&b,10,MPI_CHAR,(nprocs+myrank-1),0,MPI_COMM_WORLD);
	 
	 }
	
	 if(p.state=="?")
	 {
	 char str[10];
	 if(myrank-1>=0)
	 MPI_Irecv(&str,10, MPI_CHAR,(myrank-1), 0, MPI_COMM_WORLD, &request);
	 else
	 MPI_Irecv(&str,10, MPI_CHAR,(nprocs+myrank-1), 0, MPI_COMM_WORLD, &request);
	 if(str=="X1")
	 p.state=str;
	 if(myrank-2>=0)
	 MPI_Irecv(&str,10, MPI_CHAR,(myrank-2), 0, MPI_COMM_WORLD, &request);
	 else
	 MPI_Irecv(&str,10, MPI_CHAR,(nprocs+myrank-2), 0, MPI_COMM_WORLD, &request);
	 if(str=="X0")
	 p.state=str;
	 
	 }
	 if(p.state=="X1")
	 {
	 char str[10];
	 MPI_Irecv(&str,10, MPI_CHAR,(myrank+1)%nprocs, 0, MPI_COMM_WORLD, &request);
	 if(str=="1")
	 p.state=str;
	 }
	 
	 MPI_Barrier(MPI_COMM_WORLD);
	 if(myrank==0)
	 printf("After expansion process\n");
	
	 printf("node %d has state %s\n",myrank,p.state); 
	 
	MPI_Barrier(MPI_COMM_WORLD);
	if(myrank==0)
	printf("Filling starts\n");
	//Filling process
	vector<string>v3;
	char buff[10];
	sprintf(buff,p.state);
	char message[10];
	for(int i=1;i<=7;i++)
	{
	
	    MPI_Send(&buff,10,MPI_CHAR,(myrank+i)%nprocs,0,MPI_COMM_WORLD);
	    if((myrank-i)>=0)
	    {MPI_Irecv(&message,10, MPI_CHAR,(myrank-i), 0, MPI_COMM_WORLD,&request);
	    printf("Node %d receives from node %d = %d\n",myrank,myrank-i,message);
	    v3.push_back(message);
	    }
	    else
	    {MPI_Irecv(&message,10, MPI_CHAR,(nprocs+myrank-i), 0, MPI_COMM_WORLD,&request);
	    printf("Node %d receives from node %d =%d\n",myrank,nprocs+myrank-i,message);
	    v3.push_back(message);
	    }
	    MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(p.state=="X0")
	{
	int count=0;
	for(int i=0;i<v3.size();i++)
	{
	if(v3[i]=="?")
	count++;
	else
	break;
	}
	if(count==0)   ///F0 configuration
	{
	p.state=0;
	char tt[10];
	sprintf(tt,"1");
	MPI_Send(&tt,10,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	sprintf(tt,"0");
	if(myrank-1>=0)
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1+nprocs),0,MPI_COMM_WORLD);
        sprintf(tt,"1");
        if(myrank-2>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-2),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-2+nprocs),0,MPI_COMM_WORLD);
	}
	if(count==1)
	{
	p.state="0";
	char tt[10];
	sprintf(tt,"1");
	MPI_Send(&tt,10,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	sprintf(tt,"1");
	if(myrank-1>=0)
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1+nprocs),0,MPI_COMM_WORLD);
        sprintf(tt,"0");
        if(myrank-2>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-2),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-2+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-3>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-3),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-3+nprocs),0,MPI_COMM_WORLD);
	}
	if(count==2)
	{
	p.state="1";
	char tt[10];
	sprintf(tt,"0");
	MPI_Send(&tt,10,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	sprintf(tt,"0");
	if(myrank-1>=0)
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1+nprocs),0,MPI_COMM_WORLD);
        sprintf(tt,"0");
        if(myrank-2>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-2),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-2+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-3>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-3),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-3+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-4>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-4),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-4+nprocs),0,MPI_COMM_WORLD);
	}
	if(count==3)
	{
	p.state="1";
	char tt[10];
	sprintf(tt,"0");
	MPI_Send(&tt,10,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	sprintf(tt,"0");
	if(myrank-1>=0)
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1+nprocs),0,MPI_COMM_WORLD);
        sprintf(tt,"1");
        if(myrank-2>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-2),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-2+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-3>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-3),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-3+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-4>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-4),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-4+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-5>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-5),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-5+nprocs),0,MPI_COMM_WORLD);
	}
	if(count==4)
	{
	p.state="0";
	char tt[10];
	sprintf(tt,"1");
	MPI_Send(&tt,10,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	sprintf(tt,"1");
	if(myrank-1>=0)
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1+nprocs),0,MPI_COMM_WORLD);
        sprintf(tt,"0");
        if(myrank-2>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-2),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-2+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-3>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-3),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-3+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-4>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-4),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-4+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-5>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-5),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-5+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-6>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-6),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-6+nprocs),0,MPI_COMM_WORLD);
	}
	if(count==5)
	{
	p.state="0";
	char tt[10];
	sprintf(tt,"1");
	MPI_Send(&tt,10,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	sprintf(tt,"1");
	if(myrank-1>=0)
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1+nprocs),0,MPI_COMM_WORLD);
        sprintf(tt,"0");
        if(myrank-2>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-2),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-2+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-3>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-3),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-3+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-4>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-4),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-4+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-5>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-5),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-5+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-6>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-6),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-6+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-7>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-7),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-7+nprocs),0,MPI_COMM_WORLD);
	}
	if(count==6)
	{
	p.state="0";
	char tt[10];
	sprintf(tt,"1");
	MPI_Send(&tt,10,MPI_CHAR,(myrank+1)%nprocs,0,MPI_COMM_WORLD);
	sprintf(tt,"0");
	if(myrank-1>=0)
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-1+nprocs),0,MPI_COMM_WORLD);
        sprintf(tt,"1");
        if(myrank-2>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-2),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-2+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-3>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-3),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-3+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-4>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-4),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-4+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-5>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-5),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-5+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-6>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-6),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-6+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"0");
        if(myrank-7>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-7),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-7+nprocs),0,MPI_COMM_WORLD);
	sprintf(tt,"1");
        if(myrank-8>=0)
        MPI_Send(&tt,10,MPI_CHAR,(myrank-8),0,MPI_COMM_WORLD);
	else
	MPI_Send(&tt,10,MPI_CHAR,(myrank-8+nprocs),0,MPI_COMM_WORLD);
	}
	}
	char sy[10];
	MPI_Irecv(&sy,10, MPI_CHAR,(myrank+1)%nprocs, 0, MPI_COMM_WORLD, &request);
	if(strlen(sy)>0)
	{
	p.state=sy;
	memset(sy,NULL,sizeof(sy));
	}
	MPI_Irecv(&sy,10, MPI_CHAR,(myrank+2)%nprocs, 0, MPI_COMM_WORLD, &request);
	if(strlen(sy)>0)
	{
	p.state=sy;
	memset(sy,NULL,sizeof(sy));
	}
	MPI_Irecv(&sy,10, MPI_CHAR,(myrank+3)%nprocs, 0, MPI_COMM_WORLD, &request);
	if(strlen(sy)>0)
	{
	p.state=sy;
	memset(sy,NULL,sizeof(sy));
	}
	MPI_Irecv(&sy,10, MPI_CHAR,(myrank+4)%nprocs, 0, MPI_COMM_WORLD, &request);
	if(strlen(sy)>0)
	{
	p.state=sy;
	memset(sy,NULL,sizeof(sy));
	}
	MPI_Irecv(&sy,10, MPI_CHAR,(myrank+5)%nprocs, 0, MPI_COMM_WORLD, &request);
	if(strlen(sy)>0)
	{
	p.state=sy;
	memset(sy,NULL,sizeof(sy));
	}
	MPI_Irecv(&sy,10, MPI_CHAR,(myrank+6)%nprocs, 0, MPI_COMM_WORLD, &request);
	if(strlen(sy)>0)
	{
	p.state=sy;
	memset(sy,NULL,sizeof(sy));
	}
	MPI_Irecv(&sy,10, MPI_CHAR,(myrank+7)%nprocs, 0, MPI_COMM_WORLD, &request);
	if(strlen(sy)>0)
	{
	p.state=sy;
	memset(sy,NULL,sizeof(sy));
	}
	MPI_Irecv(&sy,10, MPI_CHAR,(myrank+8)%nprocs, 0, MPI_COMM_WORLD, &request);
	if(strlen(sy)>0)
	{
	p.state=sy;
	memset(sy,NULL,sizeof(sy));
	}
        MPI_Barrier(MPI_COMM_WORLD);
        if(myrank==0)
	 printf("After Filling process\n");
	
	 printf("node %d has state %s\n",myrank,p.state); 

	// Finalize the MPI environment.
	MPI_Finalize();
	
}
