//Naryan Sambhi 
// A5 Computer networks 

// goal: 
// write a multithreaded server
// - take in three different options 
// - take multiple clients 
// 1 - take one message 
// 2 - take multiple 
// 3 - send messages back

//functions file


#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <sstream>

#include <semaphore.h>

#define messagesdbs "messages.csv"


////////////////////////////////////////////////////////////////////////////////////////////////////////////

//save to a csv file
std::string saveMessageToFile(const std::string& message, const std::string& filename) {
    // Open the file in append mode
    std::ofstream file(filename, std::ios::app);
    
    //error handling
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "error writing to server \0";
    }

    // Write the message to the file
    file << message << std::endl;
    // Close the file
    file.close();

    return "success writing to server \0";
}

std::string parseMessageSaveToFile(const std::string& message, const std::string& filename) {
    
    std::istringstream iss(message);
    std::string delimiter;
    int count = 0;
    std::string FullPost;

    //loop get line
    while (std::getline(iss, delimiter, '|')) {
        ++count;
        FullPost += delimiter + "|";

        // Save the message to the CSV file when three '|' characters are encountered
        if (count == 3) {
            saveMessageToFile(FullPost, filename);
            FullPost.clear();
            count = 0;  // Reset the count for the next message
        }
    }

    return "success heheh \0";
}



// Function to read all messages from the file
std::string readMessagesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string messages;
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            messages += line;
        }
        file.close();
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
        messages = "error reading from server \0";
    }

    return messages;
}





void NewConnection(int ConnectionSocket, sem_t& sem) {
    
    // Inside the connection handling block
    while (true) {

        // Receive data
        char buff[1000];
        int bytes = recv(ConnectionSocket, buff, sizeof(buff), 0);

        //error handling
        if (bytes == -1) {
            std::cout << "ERROR: failed to receive data" << std::endl;
            //error code here
        } else if (bytes == 0) {
            std::cout << "Client disconnected" << std::endl;
            // Close the connection and break out of the inner loop
            close(ConnectionSocket);
            break;
        }


        // decide what we will do with data 
        if (strcmp(buff, "1") == 0) {

            memset(buff, 0, sizeof(buff));

            // Receive 
            int bytes = recv(ConnectionSocket, buff, sizeof(buff), 0);

            //write
            sem_wait(&sem);
            std::string SaveMessage = saveMessageToFile(buff, messagesdbs);
            sem_post(&sem);

            // Send the result back to the client
            send(ConnectionSocket, SaveMessage.c_str(), SaveMessage.length(), 0);
            

        } 
        else if (strcmp(buff, "2") == 0) {

            memset(buff, 0, sizeof(buff));


            // Receive 
            int bytes = recv(ConnectionSocket, buff, sizeof(buff), 0);

            //write
            sem_wait(&sem);
            std::string SaveMessage = parseMessageSaveToFile(buff, messagesdbs);
            sem_post(&sem);

            // Send the result back to the client
            send(ConnectionSocket, SaveMessage.c_str(), SaveMessage.length(), 0);

        }
        else if (strcmp(buff, "3") == 0) {

            memset(buff, 0, sizeof(buff));


            sem_wait(&sem);

            // Retrieve all messages from the file
            std::string Messages = readMessagesFromFile(messagesdbs);
            
            sleep(1);

            sem_post(&sem);


            // Send the messages back to the client
            send(ConnectionSocket, Messages.c_str(), Messages.length(), 0);
            
        } 
        else {
            std::cout << "ERROR: Invalid option" << std::endl;        
            memset(buff, 0, sizeof(buff));
        }

        // Clear the buffer
        memset(buff, 0, sizeof(buff));
    }

}




////////////////////////////////////////////////////////////////////////////////////////////////////////////
