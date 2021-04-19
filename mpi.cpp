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
	char *state;  //X0,X1,1 or 0
	//int left;
	//int right;
	int count; //for counting votes
	Process(int r, int nprocs) {
		rank = r;
		identifier = generateRandomNumber(rank, nprocs);
		count = 0;
		state = "?";
	
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
  int myrank, nprocs, leftid, rightid;
  int val, sum, tmp;
  MPI_Status recv_status, send_status;
  MPI_Request send_request;
  
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);
	
	// Get the number of processes
	
	
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	// Get the rank of the process
	
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// Get the name of the processor
	//char processor_name[MPI_MAX_PROCESSOR_NAME];
	Process p(myrank, nprocs);
	p.state="?";
	myrank=p.rank;
	srand(myrank);
	//int name_len;
	//MPI_Get_processor_name(processor_name, &name_len);




     
  /*--------------------*/
  /* Find out neighbors */
  /*--------------------*/

  if ((leftid=(myrank-1)) < 0) leftid = nprocs-1;
  if ((rightid=(myrank+1)) == nprocs) rightid = 0;
  
  /*---------------------------------------------------------------------*/
  /* Send the process rank stored as val to the process on my right and  */
  /* receive a process rank from the process on my left and store as tmp */
  /*---------------------------------------------------------------------*/
  int flag=0,j=1;
  while(j!=2)
  {
        if(myrank==0)
        {
        printf("Round %d \n\n\n\n",j);
        }
        j++;
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
  val = v;
  vector<int>v1;
  v1.push_back(v);
  int i=1;
  do {
     
    MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    v1.push_back(tmp);
    val = tmp;
    //sum += val;
    i++;
  } while (i != 7);
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
	vector<int>v2;

  i=1;
  do {
     
    MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    v2.push_back(tmp);
    val = tmp;
    //sum += val;
    i++;
  } while (i != 7);
  v2.push_back(v);
  
  printf("size of vote from right to %d node = %d\n",myrank,v2.size());
  for(int i=0;i<v2.size();i++)
	{
	printf("%d ",v2[i]);
	if(i==v2.size()-1)
	printf("for node %d",myrank);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
   if(config==v1||v2==config)
   p.state="X0";
   if(config==v1)
   {
      val=1;
   }
   else
   val=0;
   
     MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp==1)
    {
    p.state="X1";
    val=1;
    }
    else
    val=0;
    
     MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp==1)
    {
    p.state="0";
    val=1;
    }
   else
   val=0;
   
     MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp==1)
    {
    p.state="1";
    val=1;
    }
   else
   val=0;
   
     MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp==1)
    {
    p.state="0";
    val=1;
    }
   else
   val=0;
   
     MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp==1)
    {
    p.state="X1";
    val=1;
    }
   else
   val=0;
   
     MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp==1)
    {
    p.state="X0";
    val=1;
    }
    else
    val=0;
 MPI_Barrier(MPI_COMM_WORLD);
 printf("node %d has state %s\n",myrank,p.state);   
 MPI_Barrier(MPI_COMM_WORLD);
//Phase 2 :Expansion process
	if(myrank==0)
	printf("Phase 2\n");
    i=0;
    char buf[10],msg[10];
    sprintf(buf,p.state);
  do {
     
    MPI_Issend(&buf,10,MPI_CHAR,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&msg,10,MPI_CHAR,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    sprintf(buf,msg);
    //sum += val;
    i++;
  } while (i != 4);
  printf("node %d has buffer =%s\n",myrank,buf);
  MPI_Barrier(MPI_COMM_WORLD);
  string str=buf;
  if(p.state=="X0"&&str=="?")
  {
  p.state="0";
  val=1;
  }
  else
  val=0;
  printf("node %d has state (%s,%d)\n",myrank,p.state,val);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
   if(tmp==1)
   p.state="1";
   
  MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp==1)
    {
    p.state="X1";
    val=1;
    }
    else
    val=0;
    MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp==1)
    p.state="X0";
    
    MPI_Barrier(MPI_COMM_WORLD);
	 if(myrank==0)
	 printf("After expansion process\n");
	
	 printf("node %d has state %s\n",myrank,p.state);
	 MPI_Barrier(MPI_COMM_WORLD);
    if(myrank==0)
    printf("filling phase starts\n");
    char v3[6][10];
    i=1;
    
    sprintf(buf,p.state);
  do {
    MPI_Issend(&buf,10,MPI_CHAR,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&msg,10,MPI_CHAR,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    
    string str=msg;
    
    sprintf(v3[i-1],msg);
    sprintf(buf,msg);
    //sum += val;
    i++;
  } while (i != 7);
  int count=0;
  
   for(int i=0;i<6;i++)
	{
	printf("%s ",v3[i]);
	if(i==5)
	printf("for node %d",myrank);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);

  if(p.state=="X0")
  {
  
   for(int i=0;i<6;i++)
    {
    string str=v3[i];
     if(str=="?")
     count++;
     else if(str=="X0")
     {
     val=1;
     break;
     }
     else
     break;
    }
  }
  else
      val=0;
  printf("node %d has val,count of = (%d,%d)\n",myrank,val,count);
  MPI_Barrier(MPI_COMM_WORLD);
  if(val==1)
  {
    val=count+1;
    if(val==1||val==2||val==5||val==6||val==7)
    p.state="0";
    if(val==3||val==4)
    p.state="1";
  }
  else
  val=0;
    MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
  if(tmp!=0)
  {
  if(tmp==1||tmp==2||tmp==5||tmp==6||tmp==7)
  p.state="1";
  if(tmp==3||tmp==4)
  p.state="0";
  }
  
  MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp!=0)
    {
    if(tmp==1||tmp==3||tmp==4||tmp==7)
    p.state="0";
    if(tmp==2||tmp==5||tmp==6)
    p.state="1";
    val=tmp;
    }
    else
    val=0;
     MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp!=0)
    {
    if(tmp==2||tmp==3||tmp==5||tmp==6)
    p.state="0";
    if(tmp==1||tmp==4||tmp==7)
    p.state="1";
    val=tmp;
    }
    else
    val=0;
    MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp!=0)
    {
    if(tmp==4||tmp==5||tmp==7)
    p.state="0";
    if(tmp==2||tmp==3||tmp==6)
    p.state="1";
    val=tmp;
    }
    else
    val=0;
    MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp!=0)
    {
    if(tmp==3||tmp==6||tmp==7)
    p.state="0";
    if(tmp==4||tmp==5)
    p.state="1";
    val=tmp;
    }
    else
    val=0;
    MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp!=0)
    {
    if(tmp==4||tmp==5)
    p.state="0";
    if(tmp==6||tmp==7)
    p.state="1";
    val=tmp;
    }
    else
    val=0;
    MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp!=0)
    {
    if(tmp==6||tmp==7)
    p.state="0";
    if(tmp==5)
    p.state="1";
    val=tmp;
    }
    else
    val=0;
    MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp!=0)
    {
    if(tmp==7)
    p.state="0";
    if(tmp==6)
    p.state="1";
    val=tmp;
    }
    else
    val=0;
       MPI_Issend(&val,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    if(tmp!=0)
    {
    if(tmp==7)
    p.state="1";
    }
    else
    val=0;
    MPI_Barrier(MPI_COMM_WORLD);
    if(myrank==0)
	 printf("After Filling process\n");
	
	 printf("node %d has state %s\n",myrank,p.state);
    if(p.state=="?")
    {
    flag=1;
    }
    } 
    
  MPI_Finalize();
		
}
