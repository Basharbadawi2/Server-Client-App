#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <ctime>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
using namespace std;

int GetRandom(int Max)
{
    // Return number be*tween 0 and RAND_MAX
    return rand() % Max;
}

void EcexutingThread(int id)
{
    // Time now
    auto now_time = chrono::system_clock::now();
    // Converting time to time now variable  ... Varaible of type time_t
    time_t sleeptime = chrono::system_clock::to_time_t(now_time);
    // Printng the time now ... using ctime to convert time into string
    cout << "thread " << id << " Started at : " << ctime(&sleeptime);
}


int main()
{
    // Creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    // Server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(1025); // Port 1025

    // Binding socket to address
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        cerr << "Error binding socket" << endl;
        return 1;
    }

    // Listening for incoming connections
    if (listen(serverSocket, 5) == -1)
    {
        cerr << "Error listening" << endl;
        return 1;
    }

    cout << "Server started listening for connections..." << endl;

    // Accept incoming connections
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSocket == -1)
    {
        cerr << "Error accepting connection" << endl;
        return 1;
    }

    // Send response to client --
    // Sending in form of pointer to the variable
    const char *response = "Hello client!\n";
    ssize_t bytesSent = send(clientSocket, response, strlen(response), 0);
    if (bytesSent == -1)
    {
        cerr << "Error sending response to client" << endl;
        return 1;
    }

    char buffer[1024] = {0};
    memset(buffer, 0, sizeof(buffer));

    // Receive data from client -
    // Receive in form of buffer
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == -1)
    {
        cerr << "Error receiving data from client" << endl;
        return 1;
    }

    cout << "Message from client: " << buffer << endl;

    // Sedning response to client
    response = " Enter the number of users: ";
    send(clientSocket, response, strlen(response), 0);
    if (bytesSent == -1)
    {
        cerr << "Error sending response to client" << endl;
        return 1;
    }

    // Reciving number of users
    buffer[1024] = {0};
    memset(buffer, 0, sizeof(buffer));

    int receivedUsers;
    recv(clientSocket, &receivedUsers, sizeof(receivedUsers), 0);
    cout << "The number of users are " << receivedUsers << endl;

    // Receivnig data from client -------

    int USERS = 0 ;
    thread threads[receivedUsers] ;
    // Loop for processing users
    for (int i=0; i<receivedUsers; i++)
    {
        // Sending response to the client -----
        response = "Do you want to register ?(y/n) ";
        send(clientSocket, response, strlen(response), 0);

        // Reciving message from the client -------

        char ans ;
        recv(clientSocket, &ans, sizeof(ans), 0);
        cout << "Your answer is " << ans << endl;
        // Running thread
        threads[i] = thread(EcexutingThread, i);
        // If statement
        buffer[1024] = {0} ; // Name

        if (ans == 'y')
        {
            int password;
            USERS++;
            cout << USERS << " Users is ready" << endl ;
            recv(clientSocket, &password, sizeof(password), 0);
            cout << USERS << " St user password is set ! " << endl ;
            recv(clientSocket, &buffer, sizeof(buffer), 0 ); // Name
            cout << USERS << "St user name is set ! " << endl ;
            char anss ;
            send(clientSocket, "Do you want to log in ? (y/n) ", strlen("Do you want to log in ? (y/n) "), 0 );
            recv(clientSocket, &anss, sizeof(anss), 0);
            cout << "Your answer is " << anss << endl;

            // Login proccess
            int counter = 0;
            int counter2 = 0;
            char bufferr[1024] = {0};
            int Password;
            if (anss == 'y')
            {
                recv(clientSocket, &bufferr, sizeof(bufferr), 0 ) ;
                // Validation

                for (int i=0; i<sizeof(bufferr); i++)
                {
                    for (int j=0; j<sizeof(buffer); j++)
                    {
                        if (bufferr[i] == buffer[j])
                        {
                            counter++;
                        }
                    }
                }
                // Convert integers to strings
                recv(clientSocket, &Password, sizeof(Password), 0 );

                string passStr = to_string(password);
                string PassStr = to_string(Password);
                for (int i=0; i<passStr.size(); i++)
                {
                    for(int j=0; j<PassStr.size(); j++)
                    {
                        if(passStr[i]==PassStr[j])
                        {
                            counter2++;
                        }
                    }
                }
                cout << counter2 << endl ;
                cout << counter << endl ;
                if(counter2 == sizeof(password))
                {
                    send(clientSocket, "Welcome", strlen("Welcome"), 0) ;

                }
                else
                {
                    send(clientSocket, "Error ! log in again  ", strlen("Error ! log in again  "), 0);

                }
            }
            else
            {
                continue ;
            }
        }

        else
        {
            continue ;
        }
        // -------------
    }


    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
