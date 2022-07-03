//
// Created by Siew on 10/19/2019.
//
#include "MainHeader.h"

int baseWorkload(MPI_Comm master_comm, int total_num_event, double total_time_encrypt);
/**
 * base station will receive all messages from nodes in cartesian communicator, decrypt the message and
 * finally write message to file.
 *
 * @param master_comm, mpi communicator
 * @param total_num_event, total numnber of event
 * @param total_time_encrypt, total time of encryption.
 * @return 0, exit
 */
int baseWorkload(MPI_Comm master_comm, int total_num_event, double total_time_encrypt){
    int size;
    MPI_Comm_size( master_comm, &size );
    //log file for all nodes activation time line
    FILE *logfile= fopen("log.txt","w+");
    //log file for summary of all nodes activation information in log
    FILE *summaryLogFile = fopen("logSummary.txt","w+");
    time_t rawtime;
    struct tm * timeinfo;
    double total_decrypt_time = 0, total_comm_time = 0;
    int arr_pointer, total_event = total_num_event;
    //collecting frequency random number appear in each nodes.
    int* num_event_nodes = (int *) calloc((UPPER_BOUND -LOWER_BOUND +1)*(size-1) , sizeof(int));
    /**
     * loops though all event, and receives the all messages from nodes in cartesian communicator,
     * decrypt the message and finally write message to file
     */
    while(total_num_event > 0)
    {
        //message is a buffer that store messages from other nodes.
        int* message = (int *) malloc(NUM_OF_BASE_MESSAGE * sizeof(int));
        //get time taken of communication with other node
        double start_time = MPI_Wtime(), time_taken;
        MPI_Recv(message,NUM_OF_BASE_MESSAGE,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,master_comm,MPI_STATUS_IGNORE);
        time_taken = MPI_Wtime() - start_time;
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        total_comm_time += time_taken;
        //get time taken to decrypt all message in buffer.
        double decrypt_start = MPI_Wtime();
        for (arr_pointer = 0; arr_pointer < NUM_OF_BASE_MESSAGE; arr_pointer++) {
            message[arr_pointer] = decrypt(message[arr_pointer]);
        }
        double decrypt_time = MPI_Wtime() - decrypt_start;
        /**
         * write output to log which consist of:
         * Current local time and date
         * Communication time between node and base-station
         * Reference Node
         * Random Number that trigger event
         * list of Adjacent Nodes
         */
        fprintf(logfile,"==========================================================\n");
        fprintf(logfile,"Current local time and date: %s \n",asctime(timeinfo));
        fprintf(logfile,"Communication time between node and base-station: %lf \n",time_taken);
        fprintf(logfile,"Reference Node: %d  \n", message[0]);
        fprintf(logfile,"Random Number: %d \n",message[1]);
        num_event_nodes[message[0]*(UPPER_BOUND - LOWER_BOUND + 1)+ (message[1]-LOWER_BOUND)]++;
        for (arr_pointer = ADJACENT_NODE_POS; arr_pointer < NUM_OF_BASE_MESSAGE; arr_pointer++) {
            if(message[arr_pointer] >= 0){
                fprintf(logfile,"Adjacent Nodes: %d \n",message[arr_pointer]);
            }
        }
        total_decrypt_time += decrypt_time;
        total_num_event--;
    }
    /**
     * write output to summary log which consist of:
     * Total number of event occured
     * Total Time of communication
     * Average Time of communication
     * Total Time to encrypt message
     * Total Time to decrypt message
     * Average Time to encrypt message
     * Average Time to decrypt message
     *
     * These information can be used to analysed in report for behavior of event detection
     */
    fprintf(summaryLogFile,"==========================================================\n");
    fprintf(summaryLogFile,"Total number of event occured: %d \n",total_event);
    fprintf(summaryLogFile,"Total Time of communication: %lf \n",total_comm_time);
    fprintf(summaryLogFile,"Average Time of communication: %lf \n",total_comm_time/total_event);
    fprintf(summaryLogFile,"Total Time to encrypt message: %lf \n",total_time_encrypt);
    fprintf(summaryLogFile,"Total Time to decrypt message: %lf \n",total_decrypt_time);
    fprintf(summaryLogFile,"Average Time to encrypt message: %10lf \n",total_time_encrypt/total_event);
    fprintf(summaryLogFile,"Average Time to decrypt message: %10lf \n",total_decrypt_time/total_event);
    fprintf(summaryLogFile,"==========================================================\n");
    for (arr_pointer = 0; arr_pointer < (size-1)*(UPPER_BOUND - LOWER_BOUND + 1); arr_pointer++) {
        if(num_event_nodes[arr_pointer] > 0){
            fprintf(summaryLogFile,"Rank %d has Random Number: %d occured %d \n", arr_pointer/(UPPER_BOUND-LOWER_BOUND+1), arr_pointer%(UPPER_BOUND-LOWER_BOUND+1) + LOWER_BOUND, num_event_nodes[arr_pointer]);
        }
    }
    fclose(summaryLogFile);
    fclose(logfile);
    printf("WSN has succesfully completed job.\n");
    printf("Total number of event occured: %d \n",total_event);
    printf("Total Time of communication: %lf \n",total_comm_time);
    printf("Average Time of communication: %lf \n",total_comm_time/total_event);
    printf("Total Time to encrypt message: %10lf \n",total_time_encrypt);
    printf("Total Time to decrypt message: %10lf \n",total_decrypt_time);
    printf("Average Time to encrypt message: %10lf \n",total_time_encrypt/total_event);
    printf("Average Time to decrypt message: %10lf \n",total_decrypt_time/total_event);
    return 0;
}
