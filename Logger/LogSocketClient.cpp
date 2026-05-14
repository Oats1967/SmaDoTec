#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27015"
#define SERVERNAME "localhost"


namespace logsocket
{

    SOCKET ConnectSocket = INVALID_SOCKET;

    int open(const char * servername)
    {
        WSADATA wsaData;
        struct addrinfo* result = NULL,
            * ptr = NULL,
            hints;

        // Validate the parameters
        // Initialize Winsock
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
            printf("WSAStartup failed with error: %d\n", iResult);
            return 1;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Resolve the server address and port
        iResult = getaddrinfo(servername, DEFAULT_PORT, &hints, &result);
        if (iResult != 0)
        {
            printf("getaddrinfo failed with error: %d\n", iResult);
            return 1;
        }

        // Attempt to connect to an address until one succeeds
        for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
        {
            // Create a SOCKET for connecting to server
            ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (ConnectSocket == INVALID_SOCKET)
            {
                printf("socket failed with error: %ld\n", WSAGetLastError());
                break;
            }

            // Connect to server.
            iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR)
            {
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }
        freeaddrinfo(result);

        if (ConnectSocket == INVALID_SOCKET)
        {
            printf("Unable to connect to server!\n");
            return 1;
        }
        return 0;
    }


    int write(const char* sendbuf)
    {
        int iResult = -1;
        // Send an initial buffer
        if (ConnectSocket != INVALID_SOCKET)
        {
            iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
            }
        }
        return iResult;
    }



    int close()
    {
        if (ConnectSocket != INVALID_SOCKET)
        {
            int iResult = shutdown(ConnectSocket, SD_SEND);
            if (iResult == SOCKET_ERROR)
            {
                printf("shutdown failed with error: %d\n", WSAGetLastError());
                return 1;
            }

            // Receive until the peer closes the connection
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
        }
        WSACleanup();
        return 0;
    }

};