#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

int main(int argc, char *argv[] ) 
{
    //Variable Decleration
    //integers to hold the rank and size of the process.
    //integers to hold the ID (ID) and the randomly generated number (NN)
    //integer to hold the max value (MAX) used to generate the random number
    //int array of size 4 (PRVR) to hold  0: President Value, 1:President Rank, 2:VP Value, 3: VP Rank
    int rank, size, ID, NN; 
    int PRVR[4];
    int MAX = 89;

    //Char Arrays used to convert the integer values to chars
    //We incrementaly build the ID, so we use char arrays to concatenate them together.
    //char arrays for the completed ID (charID), NN Value (NNArray), rank value in 2 digit format (RRArray),
    //the value of the current rank(rankChar), and the result of the mod 2 operation on NN (Dvalue)
    char charID[5], NNArray[2], RRArray[2], rankChar[1], Dvalue[1];

    //initializing MPI Processess
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    //creating a seed to generate a random number. Time + rank is used to have a different seed for the different processess.
    srand (time(NULL) + rank);

    //Actions to be taken only for the processes, rank 0
    if (rank == 0)
    {
        //The random number, NN, is generated. NN range is 9 < NN < 100, so MAX is 89 + 10
        NN = (rand() % MAX + 10);
        //sprintF converts the integer values into char arrays
        //the ID is composed of 3 sections of integers NN RR D
        //these ints are converted to chars to concatenate them together with strcat
        sprintf(NNArray, "%d", NN);
        sprintf(RRArray, "0%d", rank);
        sprintf(Dvalue, "%d", (NN % 2));
        strcpy(charID, NNArray);
        strcat(charID, RRArray);
        strcat(charID, Dvalue);
        //the final result of the process is converted back into an integer, ID
        ID = atoi(charID);

        //if the ID is even, rank 0 becomes the President and the VP is assigned to 1 (smallest odd value)
        if(ID % 2 == 0) {
            PRVR[0] = ID;
            PRVR[1] = rank;
            PRVR[2] = 00001;
            PRVR[3] = rank;
        }
        //if the ID is odd, rank 0 becomes the VP and the President is assigned 99998 (highest even value)
        else {
            PRVR[0] = 99998;
            PRVR[1] = rank;
            PRVR[2] = ID;
            PRVR[3] = rank;
        }
        //the array with the election information is sent to the next process
        MPI_Send(PRVR, 4, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);

        //rank 0 then recieves the final results of the election(represented by array PRVR) from the last process
        MPI_Recv(&PRVR, 4, MPI_INT, (rank - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //the results of the election are printed
        printf("President: %d \nPresident Rank: %d \nVice President: %d \nVP Rank: %d \n",
        PRVR[0],PRVR[1],PRVR[2],PRVR[3]);
    }
    //the actions taken for every other rank
    else {
        //the current election information is obtained (array PRVR)
        MPI_Recv(&PRVR, 4, MPI_INT, (rank - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //a random number (NN) is generated
        NN = (rand() % MAX + 10);
        //a process similar to the one in rank 0 of converted the ID section integers (NN RR D) into char arrays occurs here
        //however RRArray is given special consideration
        //because the result for the rank number must always be 2 digits, ranks < 10 are given a leading 0
        sprintf(NNArray, "%d", NN);
        if (rank < 10) {
            sprintf(RRArray, "0%d", rank);
        }
        else {
            sprintf(RRArray, "%d", rank);
        }
        sprintf(Dvalue, "%d", (NN % 2));
        strcpy(charID, NNArray);
        strcat(charID, RRArray);
        strcat(charID, Dvalue);
        ID = atoi(charID);

        //the generated ID of this rank is compared to the PRVR values of the previous process, recieved by MPI_Recv

        //if the generated ID of this rank is even
        //the ID is compared to the current President, if it is smaller this rank becomes the new president
        //the PRVR array is updated to reflect this change
        if(ID % 2 == 0) {
            if ( ID < PRVR[0]) {
                PRVR[0] = ID;
                PRVR[1] = rank;
            }
        }
        //if the generated ID of this rank is odd
        //the ID is compared to the current VP, if it is larger this rank becomes the new president
        //the PRVR array is updated to reflect this change
        else {
            if ( ID > PRVR[2]) {
                PRVR[2] = ID;
                PRVR[3] = rank;
            }
        }
        //the PRVR array is sent to the next process
        MPI_Send(PRVR, 4, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    }
    //the election is complete and the MPI connection is finalized
    MPI_Finalize();
    return 0;
}