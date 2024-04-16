#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <ctime>
#include <string>
#include <thread>
#include <vector>

using namespace std;

// Function generates random numbers between 0 and Max
int GetRandom( int Max )
{
    // Return number be*tween 0 and RAND_MAX
    return rand() % Max ;
}
// Function Calculates thread Execution time
// Thread id
void EcexutingThread( int id )
{
    // Time now
    auto now_time = chrono::system_clock::now();
    // Converting time to time now variable  ... Varaible of type time_t
    time_t sleeptime = chrono::system_clock::to_time_t(now_time);
    // Printng the time now ... using ctime to convert time into string
    cout << "thread " << id << " Started at : " << ctime(&sleeptime) ;
}
// Creating class foor the user
class User
{
private :
    int Password ;
    string Username ;
public :
    // Default construcor
    User( int Password, string Username ) : Password (Password), Username (Username) {}
    // Setters
    void SetPassword(int Password)
    {
        this->Password = Password ;
    }
    void SetUsername (string Username)
    {
        this->Username = Username ;
    }
    // Getters
    int GetPassword()
    {
        return Password ;
    }
    string GetUsername()
    {
        return Username ;
    }
    // Validation Function
    int Validation( int Passwordd, string Usernamee )
    {
        int PassCounter = 0;
        string passwordString = to_string(Password);
        int PassSize = passwordString.size() ;
        int UserCounter = 0;
        int UserSize = Username.size() ;
        string  ValidPass = to_string(Passwordd) ;
        //Password Validation

        for (int i=0; i< passwordString.size(); i++)
        {
            for (int j=0; j<ValidPass.size(); j++)
            {
                if (passwordString[i]== ValidPass[j])
                {
                    PassCounter++;
                }
            }
        }

        // Username  Validation

        for (int i=0; i<Username.size(); i++)
        {
            for (int j=0; j<Usernamee.size(); j++)
            {
                if (Username[i]== Usernamee[j])
                {
                    UserCounter++;
                }
            }
        }
        if (PassCounter == PassSize && UserCounter == UserSize )
        {
            cout << "Welcome " << Usernamee <<  endl ;
        }
        cout << "Error !" ;
        return 1;
    }
    int GetRandom( int Max )
    {
        // Return number between 0 and RAND_MAX
        return rand() % Max ;
    }
    //  void Display()
    //  {
    //
    //  }
} ;

int main()
{
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    // Server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(1025); // Port 1025
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); // IP address of the server

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        cerr << "Error connecting to server" << endl;
        return 1;
    }
    char buffer[1024] = {0} ;
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Message from server " << buffer << endl;

    // Send data to the server -
    const char *message = "Hello, server!"; // client 1st message
    ssize_t bytesSent = send(clientSocket, message, strlen(message), 0);
    if (bytesSent == -1)
    {
        cerr << "Error sending data to server" << endl;
        return 1;
    }
    // Receiving message from server
    buffer[1024] = {0} ;
    recv(clientSocket, buffer, sizeof(buffer), 0 );
    cout << "Message from server : " << buffer << endl;
    int Users;
    cin >> Users;
    send(clientSocket, reinterpret_cast<int*>(&Users), sizeof(Users), 0);

    int USERS = 0 ;
    // Creating array of threads we will need
    thread threads[Users];


    buffer[1024] = {0} ;
    memset(buffer, 0, sizeof(buffer));

    for (int i=0; i<Users; i++)
    {
        // Receiving response from server
        recv(clientSocket, buffer, sizeof(buffer), 0) ;
        // Printing the response
        cout << "Message from server: " << buffer << endl ;

        // Sending the answer
        char ans ;
        cin >> ans ;
        send(clientSocket, reinterpret_cast<char*>(&ans), sizeof(ans), 0);
        // Running thread
        threads[i] = thread(EcexutingThread, i);
        // If statemnet
        char Name [7];
        int password ;
        if(ans == 'y')
        {
            cout << "Please enter your password : "<< endl;
            cin  >> password ;
            send (clientSocket, reinterpret_cast<int*>(&password), sizeof(password), 0);
            cout << "Please enter your name : " << endl;
            cout << "Min & Max chars 7 " << endl ;

            for(int i=0; i<7; i++)
            {
                cin >> Name[i];
            }
            send(clientSocket, reinterpret_cast<char*>(&Name), sizeof(Name), 0);

            buffer[1024] = {0} ;
            recv(clientSocket, buffer, sizeof(buffer), 0 );
            cout << "Message from server : " << buffer << endl;
            char anss ;
            cin >> anss ;
            send (clientSocket, reinterpret_cast<int*>(&anss), sizeof(anss), 0);

            char NAME[7];
            int Password;

            if (anss == 'y')
            {
                cout << "Please enter your name  : " << endl ;
                for (int i=0; i<7; i++)
                {
                    cin >> NAME[i] ;
                }
                send(clientSocket , reinterpret_cast<char*>(&NAME), sizeof(NAME) , 0 );
                cout << "Please enter your password : " << endl ;
                cin >> Password;
                send(clientSocket , reinterpret_cast<int*>(&Password) , sizeof(Password) , 0 );
                // ----------------
                buffer[1024] = {0} ;
                recv(clientSocket ,buffer ,sizeof(buffer) , 0) ;
                cout << "Message from server : " << buffer << endl ;
            }
            else{
                char Respond[1024] ={0};
                recv(clientSocket , Respond ,sizeof(Respond) , 0 );
                cout << "Message from serevr : " << Respond << endl ;
            }
        }
        else
        {
            continue ;
        }

    }

    // Close sockets
    close(clientSocket);

    return 0;
}
