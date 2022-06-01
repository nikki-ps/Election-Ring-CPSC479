# cpsc479-project-1
Leaders election in a ring

GROUP MEMBERS:

Zach Serna serna648@csu.fullerton.edu
Nicole Serna NicoleSerna@csu.fullerton.edu



LANGUAGE USED: C++

HOW TO COMPILE:

1. add the file to your directory

2. compile the .cpp file using mpic++ (filename) -o ./(desiredexecname) 

3. run the exe with mpirun -n (desired number of processors) ./(desiredexecname)


DESCRIPTION:

Our implementation of the Electing Two Leaders in a Ring Topology assignment. The program will generate N identical copies
of nodes with a 5 digit ID attached to these nodes. 

The nodes' random ID's are generated as follows: 

The first two digits NN is a randomly generated number by the process; the process generates a random number in the interval 10..99 and:
If the value is negative, then take the absolute value of it
If the value is less than 10, then add 10 to it
If the value is greater than 100, then take modulo 100
OR find a formula to make it exactly in the range 10..99
The next two digits RR represent the rank. Since the number of processes in the ring must be greater than 5, if the rank is one digit (e.g. rank 5) then left-pad it with one 0’s (.e.g 05).
The last digit D represents the mod 2 of the random value generated earlier NN.


After being generated, the nodes will then determine the PRESIDENT and the VICE PRESIDENT. The president is the smallest even
value, while the vice president is the largest odd value, by default, the values for president and vice president are generated
to 99998 and 1 respectively. The president and vice presidents will then be determined by travelling across the ring, ending at
Rank 0 to determine their final rank. The newly elected president and vice president as well as their respective rank is then
printed to the screen.

We chose to implement the Concurrent two leader election algorithm in our project.
