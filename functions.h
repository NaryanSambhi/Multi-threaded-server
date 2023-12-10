//Naryan Sambhi 
// A5 Computer networks 

// goal: 
// write a multithreaded server
// - take in three different options 
// - take multiple clients 
// 1 - take one message 
// 2 - take multiple 
// 3 - send messages back

//Header file

std::string saveMessageToFile(const std::string& message, const std::string& filename);

std::string parseMessageSaveToFile(const std::string& message, const std::string& filename);

std::string readMessagesFromFile(const std::string& filename);

void NewConnection(int ConnectionSocket, sem_t& sem);