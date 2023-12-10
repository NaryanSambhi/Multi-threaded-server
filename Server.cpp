//Naryan Sambhi 
// A5 Computer networks 

// goal: 
// write a multithreaded server
// - take in three different options 
// - take multiple clients 
// 1 - take one message 
// 2 - take multiple 
// 3 - send messages back

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <sstream>

#include <semaphore.h>
#include <thread>

#define messagesdbs "messages.csv"

#include "functions.h"

// How to compile: 
// g++ server.cpp functions.cpp -o server -lpthread

//How to run:
//./server

int main (void) {

   // Starting
    std::cout << "starting server" <<std::endl;

    // our deadlock prevention mechanism 
    sem_t sem;
    if (sem_init(&sem, 0, 1) == -1) {
        std::cerr << "Error initializing semaphore" << std::endl;
        return 1;
    }


    // Create the server Socket
    int ServerSocket;
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServerSocket == -1) {
        std::cout << "ERROR: Failed to create ServerSocket" << std::endl;
        return 0;
    }

    // Bind server socket to a port
    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(27000);
    if (bind(ServerSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr)) == -1) {
        close(ServerSocket);
        std::cout << "ERROR: Failed to bind ServerSocket" << std::endl;
        return 0;
    }

    // configure server socket for incoming connections
    if (listen(ServerSocket, 1) == -1) {
        close(ServerSocket);
        std::cout << "ERROR: listen failed to configure ServerSocket" << std::endl;
        return 0;
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Accept and handle connections in a loop
while (true) {

    //Creat connection socket and form connection 

    int ConnectionSocket;
    ConnectionSocket = accept(ServerSocket, NULL, NULL);

    if (ConnectionSocket == -1) {
        std::cout << "ERROR: Failed to accept a new connection" << std::endl;
        
        continue; 


        //close(ServerSocket);
        //return 0;
    }

    //We are now connected to a new client
    std::cout << "Connected to a new client" <<std::endl;


    
    //hand client over to a thread
    std::thread connectionThread(NewConnection, ConnectionSocket, std::ref(sem));
    //making it a "background" thread, so we can continue our main program independently
    connectionThread.detach();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Cleanup - close the server socket (this part will be unreachable in this version)
close(ServerSocket);

sem_destroy(&sem);

return 0;
}

