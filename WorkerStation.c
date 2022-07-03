//
// Created by Siew on 10/19/2019.
//
#include "MainHeader.h"

double * workerWorkload(MPI_Comm cart_comm, MPI_Comm master_comm);
/**
 * worker node in cartesian communicator will loop to 10000 times, generating a random number, encrypt the number
 * then, will simultaneously does blocking send the encrypted number to all its adjacent node and receives an encrypted number
 * from its adjacent node. Then the worker node will store all that value from adjacent node to a window which have
 * size of 12: window size, 3 x Maximum number of adjacent node, 4. The worker node will then find all activation value
 * in window buffer and then sends all random number to base station when event triggers.
 *
 * @param cart_comm, cartesian communicator
 * @param master_comm, mpi communicator
 * @return 0, exit
 */
double * workerWorkload(MPI_Comm cart_comm, MPI_Comm master_comm){
    int neighbour_node[4],num_of_event=0;
    double encrypt_start = 0, encrypt_time = 0;
    MPI_Status status;
    //store adjacent node which located north and south in a buffer
    MPI_Cart_shift(cart_comm, 0, 1, &neighbour_node[UP], &neighbour_node[DOWN]);
    //store adjacent node which located east and west in a buffer
    MPI_Cart_shift(cart_comm, 1, 1, &neighbour_node[LEFT], &neighbour_node[RIGHT]);

    int i,k,windowsize = MAX_NEIGHBOUR_NODE * NUM_WINDOW,rank, arr_pointer;
    //window store all value of past 3 iteration to find more activation values.
    int* window = (int *) calloc(windowsize , sizeof(int));
    MPI_Comm_rank( master_comm, &rank);
    //seed the random number
    srand(time(0) + rank);
    /**
     * Loop though 10000 times, geenerating a random number, encrypt the number
     * then, will simultaneously does blocking send the encrypted number to all its adjacent node and receives an encrypted number
     * from its adjacent node. Then the worker node will store all that value from adjacent node to a window which have
     * size of 12: window size, 3 x Maximum number of adjacent node, 4. The worker node will then find all activation value
     * in window buffer and then sends all random number to base station when event triggers.
     */
    for (k = 0; k < MAXIMUM_ITERATION ; k++) {
        int* message = (int *) calloc(NUM_OF_BASE_MESSAGE, sizeof(int));
        int* random_lookup = (int *) calloc(UPPER_BOUND + 1, sizeof(int));
        int random_number = (rand() % (UPPER_BOUND - LOWER_BOUND + 1)) + LOWER_BOUND;
        int encryptedMessage = encrypt(random_number);
        //simultaneously does blocking send the encrypted number to all its adjacent node and receives an encrypted number from neighbour
        for (i = 0; i < 4; i++) {
            int recvalue;
            if(neighbour_node[i] >= 0){
                MPI_Sendrecv(&encryptedMessage, 1, MPI_INT, neighbour_node[i], 0, &recvalue, 1, MPI_INT, neighbour_node[i], 0, cart_comm, &status);
                //store all that value from adjacent node to a window
                window[MAX_NEIGHBOUR_NODE*(k % NUM_WINDOW) + i ] =  decrypt(recvalue);
            }
        }
        //find all activation value in window buffer and then sends all random number to base station when event triggers.
        for (i = 0; i < windowsize; i++) {
            if(window[i] > 0 ){
                //find event
                random_lookup[window[i]]++;
                if(random_lookup[window[i]] == 3){
                    /**
                     * sends all information of trigger event to base station consist of:
                     * rank
                     * random number
                     * list of adjacent nodes
                     */
                    encrypt_start = MPI_Wtime();
                    message[0] = encrypt(rank);
                    message[1] = encrypt(window[i]);
                    for (arr_pointer = ADJACENT_NODE_POS; arr_pointer < NUM_OF_BASE_MESSAGE; arr_pointer++) {
                        message[arr_pointer] = encrypt(neighbour_node[arr_pointer - ADJACENT_NODE_POS]);
                    }
                    encrypt_time += MPI_Wtime() - encrypt_start;
                    MPI_Send(message,NUM_OF_BASE_MESSAGE,MPI_INT,20,status.MPI_SOURCE,master_comm);
                    num_of_event++;
                }
            }
        }
        free(message);
        free(random_lookup);
    }
    double * retval = malloc(2 * sizeof(double));
    retval[0] = num_of_event;
    retval[1] = encrypt_time;
    return retval;
}
