/*README FILE*/

Step 1: First follow all the steps given in the original sprinkler read me file till the execution of the program movementCreator_Random_Correct_AP.py. 
----------------------------------------------------------------------------------------------------------------------------------
Step 2: 

PROGRAM NAME: creating_tick_mobility.cpp

PURPOSE: For generating the co-ordiantes of the client at every tick instant.

PREREQUISITE: C++11

INPUT:	client_i.txt where i=0,1,2... files denoting the positions of clients at different instants.
	number of clients, average speed

OUTPUT:	client_mob_i.txt where i=0,1,2.... files denoting the positions of clients at every tick time.

COMMAND: g++ -std=c++11 creating_tick_mobility.cpp
	 ./a.out <IN:no_of_clients> <IN:average speed>
---------------------------------------------------------------------------------------------------------------------------------------
Step 3:

PROGRAM: trial_sam.cpp

PURPOSE: For generating which packets and chunks are not being downloaded by play-back which helps us for generating the fdo.

PREREQUISITE: C++11

INPUT: client_mob_i.txt where i=0,1,2....files denoting the positions of clients at every tick time.
       number of paths to be checked, number of clients per path, average speed, total number of APs, total number of chunks, 		inter_ap_influence distance

OUTPUT: client_output_i.txt where i=0,1,2..... in the format x:y lines where x is the chunk number and y is the packet number 

COMMAND: g++ -std=c++11 trial_sam.cpp
         ./a.out <IN:no_of_paths> <IN:no_of_clients_per_path> <IN:speed> <IN:total_no_of_aps> <IN:no_of_chunks> <IN:inter_ap_distance>      
----------------------------------------------------------------------------------------------------------------------------------------
Step 4:

PROGRAM: fdogeneration.cpp

PURPOSE: for generating the fdo of each client

PREREQUISITE: C++11

INPUT: client_output_i.txt where i=0,1,2.... in the format x:y lines where x is the chunk number and y is the packet number 
       number of clients, total number of chunks

OUTPUT: fdo.txt 
	lines in the format x:y where x is the client number and y is the fdo of the client

COMMAND: g++ -std=c++11 fdogeneration.cpp
	./a.out <IN:number of clients> <IN:total_number_of_chunks>

---------------------------------------------------------------------------------------------------------------------------------------- 


