

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <alg.h>
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

/*
 * The class representing a process in the system. Holds all necessary process info.
 */
int vote()
{
	int sample = rand() % 100;
	if (sample <= 33)
		return 0;
	else if (sample <= 0.66)
		return 1;
	else
		return 2;
}
class Process {
public:
	int rank; // MPI generated
	int identifier; // assigned by this code
	bool status; // active or passive
	string state;  //X0,X1,1 or 0
	//int left;
	//int right;
	int count; //for counting votes
	Process(int r, int nprocs) {
		rank = r;
		identifier = generateRandomNumber(rank, nprocs);
		status = true;
		count = 0;
		/*
		if (rank == 0)
		{
			left = nprocs - 1;
			right = rank + 1;
		}
		else if (rank == nprocs - 1)
		{
			right = 0;
			left = rank - 1;
		}
		else
		{
			left = rank-1;
			right = rank + 1;
		}*/
		state = "?";
	}

	int getRank() {
		return rank;
	}

	void setRank(int r) {
		rank = r;
	}

	int getIdentifier() {
		return identifier;
	}

	void setIdentifier(int id) {
		identifier = id;
	}

	bool getStatus() {
		return status;
	}

	void setStatus(bool s) {
		status = s;
	}
};

int main(int argc, char** argv) {
	int myrank, nprocs;
	int my_id, left_id, right_id,right=0,int left=0,int h=0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	Process p(myrank, nprocs);
	printf("I am process %d of %d. Unique ID = %d\n", p.getRank(), nprocs, p.getIdentifier());
	my_id = p.getIdentifier();
	myrank = p.getRank();

	while (true) // loop to continue until a leader is elected
	{
		if (p.getStatus() == true) { // If an active node			
			if (myrank == 0) // Special boundary handling for first process in ring
			{
				int v = vote();
				if (v == 0)
				{
					left = 1;
					right = 0;
				}
				else if (v == 1)
				{
					left = 0;
					right = 0;
				}
				else
				{
					left = 0;
					right = 1;
				}
					MPI_Send(&left, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD); // send my id to next process in ring
					MPI_Recv(&h, 1, MPI_INT, nprocs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv from last process in ring
					if (h == 1)
						p.count++;
					MPI_Send(&right, 1, MPI_INT, nprocs - 1, 0, MPI_COMM_WORLD); // recv from last process in ring
					MPI_Recv(&h, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv from next process in ring
					if (h == 1)
						p.count++;
				}
				
			else if (myrank == nprocs - 1) // Special boundary handling for last process in ring
			{
				int v = vote();
				if (v == 0)
				{
					left = 1;
					right = 0;
				}
				else if (v == 1)
				{
					left = 0;
					right = 0;
				}
				else
				{
					left = 0;
					right = 1;
				}
				MPI_Send(&left, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // send to next process in ring, since I am last process therefore my next is p0
				MPI_Recv(&h, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv from previous neighbour in ring
				if (h == 1)
					p.count++;
				MPI_Send(&right, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD); // send to previous neighbour in ring
				MPI_Recv(&h, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv from next process in ring, since I am last process therefore my next is p0
				if (h == 1)
					p.count++;
			}
			else // Rest of the nodes (e.g p1,p2..)
			{
				int v = vote();
				if (v == 0)
				{
					left = 1;
					right = 0;
				}
				else if (v == 1)
				{
					left = 0;
					right = 0;
				}
				else
				{
					left = 0;
					right = 1;
				}
					MPI_Send(&left, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD); // send to next process in ring
					MPI_Recv(&h, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv from previous neighbour in ring
					if (h == 1)
						p.count++;
					MPI_Send(&right, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD); // send to previous neighbour in ring
					MPI_Recv(&h, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv from previous neighbour in ring
					if (h == 1)
						p.count++;
			}

			if (p.count==0||p.count==3) 
			{
				p.setStatus(false);
				p.setStatus(false);
			}
			else if (p.count==1) 
			{
				p.state = "X0"; 
			}
			else if (p.count == 2)
			{
				p.state = "X1";
			}
			
		}

		// If I'm a passive node I will only pass msgs forward and backward in the ring. 
		
		else if (p.getStatus() == false) {
			if (p.getRank() == 0) // Special boundary handling for first process in ring
			{
				MPI_Recv(&right_id, 1, MPI_INT, nprocs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(&right_id, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD);

				MPI_Recv(&left_id, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(&left_id, 1, MPI_INT, nprocs - 1, 0, MPI_COMM_WORLD);
			}
			else if (p.getRank() == nprocs - 1) // Special boundary handling for last process in ring
			{
				MPI_Recv(&right_id, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(&right_id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

				MPI_Recv(&left_id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(&left_id, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD);
			}
			else // Rest of the nodes (e.g p1,p2..)
			{
				MPI_Recv(&right_id, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv from previous neighbour
				MPI_Send(&right_id, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD); // pass on to next neighbour

				MPI_Recv(&left_id, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv from next neighbour
				MPI_Send(&left_id, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD); // pass to previous neighbour
			}
		}
	}

	
	
	MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS); // kill all other processes.
	MPI_Finalize(); // prepare to exit
	return 0;
}
