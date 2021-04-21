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
string convertToString(char* a, int size)
{
    string s = a;
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
        vector<string>config1,config2;
        config1.push_back("X1");
        config1.push_back("X0");
        config1.push_back("?");
        config1.push_back("?");
        config1.push_back("?");
        config1.push_back("?");
        config2.push_back("?");
        config2.push_back("?");
        config2.push_back("?");
        config2.push_back("?");
        config2.push_back("X0");
        config2.push_back("X1");
  vector<string>f0,f1,f2,f3,f4,f5,f6;
  f0={"X0","X0"};
  f1={"X0","?","X0"};
  f2={"X0","?","?","X0"};
  f3={"X0","?","?","?","X0"};
  f4={"X0","?","?","?","?","X0"};
  f5={"X0","?","?","?","?","?","X0"};
  f6={"X0","?","?","?","?","?","?","X0"};
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
        
  	// Print off a hello world message
	printf("I am process %d of %d. Unique ID = %d\n", myrank, nprocs, p.identifier);
	MPI_Barrier(MPI_COMM_WORLD);
	int v;
	if(p.state=="?")
	v=vote();
	else
	v=3;
	 //means they already achieved their final state
	if(j==1)
	{
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
	}
	j++;
      //int v=vote();
		
      printf("%d vote value is =%d\n",myrank,v);
  int val1=v;
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
  /*
  printf("size of vote from left to %d node = %d\n",myrank,v1.size());
  for(int l=0;l<v1.size();l++)
	{
	printf("%d ",v1[l]);
	if(l==v1.size()-1)
	printf("for node %d",myrank);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	*/
  vector<int>v2;
  val=val1; 
  i=1;
  do {
     
    MPI_Issend(&val,1,MPI_INT,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&tmp,1,MPI_INT,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    v2.push_back(tmp);
    val = tmp;
    i++;
  } while (i != 7);
  /*
  printf("size of vote from right to %d node = %d\n",myrank,v2.size());
  MPI_Barrier(MPI_COMM_WORLD);
  for(int l=0;l<v2.size();l++)
	{
	printf("%d ",v2[l]);
	if(l==v2.size()-1)
	printf("for node %d",myrank);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	*/
  v1.insert(v1.end(),v2.begin(),v2.end());
  /*
  MPI_Barrier(MPI_COMM_WORLD);
  for(int l=0;l<v1.size();l++)
	{
	printf("%d ",v1[l]);
	if(l==v1.size()-1)
	printf("for node %d",myrank);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
*/	
  int pos=-1;
  for(int l=0;l<v1.size();l++)
  {
  if(v1[l]==config[0])
  {
     int k=1;
     while(k!=config.size()&&(l+k)!=v1.size()&&config[k]==v1[l+k])
     k++;
     if(k==config.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if(pos-6==0)
  p.state="X0";
  if(pos-6==-1)
  p.state="X1";
  if(pos-6==-2)
  p.state="0";
  if(pos-6==-3)
  p.state="1";
  if(pos-6==-4)
  p.state="0";
  if(pos-6==-5)
  p.state="X1";
  if(pos-6==-6)
  p.state="X0";
  }
  
 MPI_Barrier(MPI_COMM_WORLD);
 printf("node %d has state %s\n",myrank,p.state);   
 MPI_Barrier(MPI_COMM_WORLD);
 
//Phase 2 :Expansion process
	if(myrank==0)
	printf("Phase 2\n");

    i=0;
    char vec1[5][10],vec2[5][10];
    //to check in right for expansion
    char buf[10],msg[10];
    sprintf(buf,p.state);
  do {
     
    MPI_Issend(&buf,10,MPI_CHAR,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&msg,10,MPI_CHAR,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    sprintf(vec2[i],msg);
    sprintf(buf,msg);
    i++;
  } while (i != 5);
  MPI_Barrier(MPI_COMM_WORLD);

  i=0;
  sprintf(buf,p.state);
  do {
     
    MPI_Issend(&buf,10,MPI_CHAR,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&msg,10,MPI_CHAR,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    sprintf(vec1[i],msg);
    sprintf(buf,msg);
    i++;
  } while (i != 5);
 /* 
  for(int l=4;l>=0;l--)
	{
	printf("%s ",vec1[l]);
	if(l==0)
	printf("for node %d",myrank);
	}
	printf("\n");
MPI_Barrier(MPI_COMM_WORLD);
*/

  vector<string>v_left;
  for(int l=4;l>=0;l--)
  {
  int a_size = sizeof(vec1[l]) / sizeof(char);
   string s_a = convertToString(vec1[l], a_size);
   v_left.insert( v_left.end(),s_a );
  //v_left.push_back(s_a);
  }
  v_left.insert( v_left.end(),p.state );
  for(int l=0;l<5;l++)
  {
  int a_size = sizeof(vec2[l]) / sizeof(char);
   string s_a = convertToString(vec2[l], a_size);
   v_left.insert( v_left.end(),s_a );
   //cout<<v_left[v_left.size()-1]<<endl;
  //v_left.push_back(s_a);
  }
  /*
 MPI_Barrier(MPI_COMM_WORLD);
  for(int l=0;l<v_left.size();l++)
	{
	for(int n=0;n<v_left[l].size();n++)
	printf("%c",v_left[l][n]);
	}
	printf("\n");
 MPI_Barrier(MPI_COMM_WORLD);
 */
  pos=-1;
   for(int l=0;l<11;l++)
  {
  if(v_left[l]==config1[0])
  {
     int k=1;
     while(k<config1.size()&&(l+k)<11&&config1[k]==v_left[l+k])
     k++;
     if(k==config1.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if(pos-5==0)
  p.state="1";
  if(pos-5==-1)
  p.state="0";
  if(pos-5==-2)
  p.state="X1";
  if(pos-5==-3)
  p.state="X0";
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  pos=-1;
   for(int l=0;l<11;l++)
  {
  if(v_left[l]==config2[0])
  {
     int k=1;
     while(k<config2.size()&&(l+k)<11&&config2[k]==v_left[l+k])
     k++;
     if(k==config2.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if(5-pos==5)
  p.state="1";
  if(5-pos==4)
  p.state="0";
  if(5-pos==3)
  p.state="X1";
  if(5-pos==2)
  p.state="X0";
  }
  
  
  
    
    MPI_Barrier(MPI_COMM_WORLD);
	 if(myrank==0)
	 printf("After expansion process\n");
	
	 printf("node %d has state %s\n",myrank,p.state);
	 MPI_Barrier(MPI_COMM_WORLD);

    if(myrank==0)
    printf("filling phase starts\n");
    
    i=0;
    char vec3[8][10],vec4[8][10];
    sprintf(buf,p.state);
  do {
     
    MPI_Issend(&buf,10,MPI_CHAR,leftid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&msg,10,MPI_CHAR,rightid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    sprintf(vec3[i],msg);
    sprintf(buf,msg);
    i++;
  } while (i != 8);
  MPI_Barrier(MPI_COMM_WORLD);

  i=0;
  sprintf(buf,p.state);
  do {
     
    MPI_Issend(&buf,10,MPI_CHAR,rightid,99,MPI_COMM_WORLD,&send_request);
    MPI_Recv(&msg,10,MPI_CHAR,leftid,99,MPI_COMM_WORLD,&recv_status);
    MPI_Wait(&send_request,&send_status);
    sprintf(vec4[i],msg);
    sprintf(buf,msg);
    i++;
  } while (i != 8);
  
 
MPI_Barrier(MPI_COMM_WORLD);
  v_left.clear();
  for(int l=7;l>=0;l--)
  {
  int a_size = sizeof(vec4[l]) / sizeof(char);
   string s_a = convertToString(vec4[l], a_size);
   v_left.insert( v_left.end(),s_a );
  //v_left.push_back(s_a);
  }
  v_left.insert( v_left.end(),p.state );
  for(int l=0;l<8;l++)
  {
  int a_size = sizeof(vec3[l]) / sizeof(char);
   string s_a = convertToString(vec3[l], a_size);
   v_left.insert( v_left.end(),s_a );
   //cout<<v_left[v_left.size()-1]<<endl;
  //v_left.push_back(s_a);
  }
  /*
 MPI_Barrier(MPI_COMM_WORLD);
  for(int l=0;l<v_left.size();l++)
	{
	for(int n=0;n<v_left[l].size();n++)
	printf("%c",v_left[l][n]);
	printf(" ");
	if(l==v_left.size()-1)
	printf("for node %d ",myrank);
	}
	printf("\n");
 MPI_Barrier(MPI_COMM_WORLD);
 */
 vector<string>f0_prepare;
 for(int i=6;i<=10;i++)
 f0_prepare.push_back(v_left[i]);
 vector<string>f1_prepare;
 for(int i=5;i<=11;i++)
 f1_prepare.push_back(v_left[i]);
  vector<string>f2_prepare;
  for(int i=4;i<=12;i++)
 f2_prepare.push_back(v_left[i]);
 vector<string>f3_prepare;
  for(int i=3;i<=13;i++)
 f3_prepare.push_back(v_left[i]);
 vector<string>f4_prepare;
  for(int i=2;i<=14;i++)
 f4_prepare.push_back(v_left[i]);
 vector<string>f5_prepare;
  for(int i=1;i<=15;i++)
 f5_prepare.push_back(v_left[i]);
  pos=-1;
  
   for(int l=0;l<f0_prepare.size();l++)
  {
  if(f0_prepare[l]==f0[0])
  {
     int k=1;
     while(k<f0.size()&&(l+k)<f0_prepare.size()&&f0[k]==f0_prepare[l+k])
     k++;
     if(k==f0.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if((pos-2)==-1)
  p.state="0";
  if((pos-2)==0)
  p.state="0";
  if((pos-2)==1)
  p.state="1";
  if((pos-2)==-2)
  p.state="1";
  }
  
  pos=-1;
    for(int l=0;l<7;l++)
  {
  if(f1_prepare[l]==f1[0])
  {
     int k=1;
     while(k<f1.size()&&(l+k)<7&&f1[k]==f1_prepare[l+k])
     k++;
     if(k==f1.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if(pos-3==1||pos-3==-1||pos-3==-3)
  p.state="1";
  if(pos-3==0||pos-3==-2)
  p.state="0";
  }
  pos=-1;
      for(int l=0;l<9;l++)
  {
  if(f2_prepare[l]==f2[0])
  {
     int k=1;
     while(k<f2.size()&&(l+k)<9&&f2[k]==f2_prepare[l+k])
     k++;
     if(k==f2.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if(pos-4==0||pos-4==-3)
  p.state="1";
  if((pos-4)==1||pos-4==-1||pos-4==-2||pos-4==-4)
  p.state="0";
  }
  pos=-1;
        for(int l=0;l<11;l++)
  {
  if(f3_prepare[l]==f3[0])
  {
     int k=1;
     while(k<f3.size()&&(l+k)<11&&f3[k]==f3_prepare[l+k])
     k++;
     if(k==f3.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if(pos-5==0||pos-5==-2||pos-5==-4)
  p.state="1";
  if(pos-5==1||pos-5==-1||pos-5==-3||pos-5==-5)
  p.state="0";
  }
  pos=-1;
    for(int l=0;l<13;l++)
  {
  if(f4_prepare[l]==f4[0])
  {
     int k=1;
     while(k<f4.size()&&(l+k)<13&&f4[k]==f4_prepare[l+k])
     k++;
     if(k==f4.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if(pos-6==1||pos-6==-1||pos-6==-4||pos-6==-6)
  p.state="1";
  if(pos-6==0||pos-6==-2||pos-6==-3||pos-6==-5)
  p.state="0";
  }
  pos=-1;
   for(int l=0;l<15;l++)
  {
  if(f5_prepare[l]==f5[0])
  {
     int k=1;
     while(k<f5.size()&&(l+k)<15&&f5[k]==f5_prepare[l+k])
     k++;
     if(k==f5.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if(pos-7==1||pos-7==-1||pos-7==-3||pos-7==-5||pos-7==-7)
  p.state="1";
  if(pos-7==0||pos-7==-2||pos-7==-4||pos-7==-6)
  p.state="0";
  }
  pos=-1;
      for(int l=0;l<17;l++)
  {
  if(v_left[l]==f6[0])
  {
     int k=1;
     while(k<f6.size()&&(l+k)<17&&f6[k]==v_left[l+k])
     k++;
     if(k==f6.size())
     {
     pos=l;
     break;
     }
  }
  }
  if(pos!=-1)
  {
  if(pos-8==1||pos-8==-2||pos-8==-3||pos-8==-5||pos-8==-8)
  p.state="1";
  if(pos-8==0||pos-8==-1||pos-8==-3||pos-8==-4||pos-8==-6||pos-8==-7)
  p.state="0";
  }
  
  
    MPI_Barrier(MPI_COMM_WORLD);
    if(myrank==0)
	 printf("After Filling process\n");
	
	 printf("node %d has state %s\n",myrank,p.state);
     MPI_Barrier(MPI_COMM_WORLD);
    if(p.state=="0"||p.state=="1")
    {
    break;
    }
    } 
    
  MPI_Finalize();
		
}
