#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

bool isEndTransmission( std::string& aMessage )
{
    size_t endChar = aMessage.find( '\q' );

    if( endChar != std::string::npos )
        return true;

    return false;
}

void clientService( const int aClientSocket )
{
    std::string message;
    bool  isEnd          = false;

    while( !isEnd )
    {
        std::cout << "Enter message : ";

        std::getline( std::cin, message );

        isEnd = isEndTransmission( message );

        write( aClientSocket, ( void* )(  message.c_str() ), sizeof( message ) );

        memset( ( void* )( message.c_str() ), 0, 0 );

        read(aClientSocket, ( void* )( message.c_str() ),  sizeof( message ) );

        std::cout << "Echo from server : " << message << std::endl;
    }
}

int main()
{
    int         clientSocket = 0;
    int         serverPort   = 54000;
    sockaddr_in serverAddr;
    hostent*    hostAddr     = nullptr;

    std::cout << "initialization start" << std::endl;

    // Create socket
    clientSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if( clientSocket <= 0 )
    {
        std::cout << "Can`t create socket!" << std::endl;
        return -1;
    }

    // Connect to server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons( serverPort );
    hostAddr              = gethostbyname( "localhost" );
    if( hostAddr == nullptr )
    {
        std::cout << "Unknown address!" << std::endl;
        return -2;
    }

    memcpy( &serverAddr.sin_addr, hostAddr->h_addr_list[0], hostAddr->h_length );

    if( connect( clientSocket, (sockaddr*)&serverAddr, sizeof( serverAddr ) ) != 0 )
    {
        std::cout << "Can not connect to server" << std::endl;
        return -3;
    }

    std::cout << "Client has been connected to server" << std::endl;

    clientService( clientSocket );

    return 0;
}
