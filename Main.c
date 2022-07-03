#include<mpi.h>
#include<stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MainHeader.h"
#include "EncryptingAlgorithm.c"
#include "DecryptingAlgorithm.c"
#include "BaseStation.c"
#include "WorkerStation.c"

/**
 * act as main driver class that will be used to initialise all nodes and
 * assign each node with thier respective work. It will have two seperate communicator but
 * cartesian communicator will work as inter communicator and MPI communicator is a master communicator
 * that holds all communicator and base station.
 *
 * @param argc, Pointer to the number of arguments
 * @param argv, Pointer to the argument vector
 * @return 0, exit
 */
int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Comm cart_comm;
    MPI_Status status;
    int dim[2] ={4,5}, period[2] ={0,0},num_of_event=0,total_num_event=0,neighbour_node[4];
    double total_time_encrypt,encrypt_start,encrypt_time=0;

    //Initialize the MPI execution environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //will only run if number of processor is 21
    if (size != 21)
    {
        printf("Please run with 21 processes.\n");fflush(stdout);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    //creates cartesian communicator for 4 x 5 grid. base station is not included
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, 1, &cart_comm);
    /**
    * for all nodes  that are in cartesian communicator
    * node 0...19 will work here.
    */
    if(cart_comm != MPI_COMM_NULL){
        double * workerResult = workerWorkload(cart_comm,MPI_COMM_WORLD);
        num_of_event = workerResult[0];
        encrypt_time += workerResult[1];
    }
    //add up all number of event of all processor and store into one variables
    MPI_Reduce(&num_of_event, &total_num_event, 1, MPI_INT, MPI_SUM, 20, MPI_COMM_WORLD);
    //add up all encryption time of all nodes.
    MPI_Reduce(&encrypt_time, &total_time_encrypt, 1, MPI_DOUBLE, MPI_SUM,20,MPI_COMM_WORLD);
    /**
     * base station will takes mpi communicator , total number of event and total encrypt time
     * to start working on wrting result to file.
     */
    if(cart_comm == MPI_COMM_NULL){
        baseWorkload(MPI_COMM_WORLD, total_num_event, total_time_encrypt);
    }
    MPI_Finalize();
    return 0;
}