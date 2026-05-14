//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module VKSocket
///
/// @file   VKSocket.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winerror.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include "BASE/Task/public/Task.h"
#include "VKSocket.h"
#include "Logger/include/Logger.h"


#define WIN32_LEAN_AND_MEAN

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "2000"


static SOCKET ConnectSocket = INVALID_SOCKET;
static BOOL gSocketOpen = FALSE;

eSocketError VKSocket_Open()
{
    WSADATA wsaData;
    // Initialize Winsock
    assert(!gSocketOpen);
    if (gSocketOpen)
    {
        LOGERROR("Socket already open ... closing first");
        (void)VKSocket_Close();
    }

    auto iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) 
    {
        LOGERROR("WSAStartup failed with errorSocket : " << iResult);
        return eSocket_WSAStartup_failed;
    }

    struct addrinfo* result = NULL,
        *ptr = NULL,
        hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    const char* servername = "localhost";
    // Resolve the server address and port
    iResult = getaddrinfo(servername, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) 
    {
        LOGERROR("getaddrinfo failed with error : " << iResult);
        WSACleanup();
        return eSocket_getaddrinfo_failed;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) 
        {
            LOGERROR("socket failed with error: : " << WSAGetLastError());
            WSACleanup();
            return eSocket_ConnectSocket_invalid;
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
        LOGERROR("Unable to connect to server!");
        WSACleanup();
        return eSocket_ConnectSocket_invalid;
    }
    base::task::Sleep(500);
    gSocketOpen = TRUE;
    return eSocket_OK;
}
//************************************************************************************************************
//************************************************************************************************************
eSocketError VKSocket_Close()
{
    eSocketError result = eSocket_OK;
    // shutdown the connection since no more data will be sent
    auto iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        LOGERROR("shutdown failed with error: " << WSAGetLastError());
        result = eSocket_shutdown_failed;
    }
    else
    {
        char recvbuf[DEFAULT_BUFLEN];
        int recvbuflen = DEFAULT_BUFLEN;

        // Receive until the peer closes the connection
        do 
        {
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
            {
                LOGERROR("Bytes received" << iResult);
            }
            else if (iResult == 0)
            {
                LOGERROR("Connection closed");
            }
            else
            {
                LOGERROR("recv failed with error: " << WSAGetLastError());
            }

        } 
        while (iResult > 0);
        result = eSocket_OK;
    }
    iResult = closesocket(ConnectSocket);
    ConnectSocket = INVALID_SOCKET;
    WSACleanup();
    gSocketOpen = FALSE;
    return result;
}
//************************************************************************************************************
//************************************************************************************************************
eSocketError VKSocket_SendBuffer(LPCSTR send_buf, int length)
{
    if ( ! gSocketOpen)
    {
        return eSocket_NotOpen;
    }
    static BOOL locked = FALSE;

    assert(!locked);
    if (locked)
    {
        return eSocket_send_failed;
    }

    eSocketError result = eSocket_OK;
    if (length > 0)
    {
        if (ConnectSocket != INVALID_SOCKET)
        {
            locked = TRUE;
            LOGERROR("**** send socket ....***");
            auto iResult = send(ConnectSocket, send_buf, length, 0);
            if (iResult > 0)
            {
                LOGDEBUG("Socket send bytes = " << iResult);
            }
            else if (iResult == 0)
            {
                LOGERROR("No bytes sent");
            }
            else if (iResult == SOCKET_ERROR)
            {
                LOGERROR("send failed with error: " << WSAGetLastError());
                result = eSocket_send_failed;
            }
            locked = FALSE;
        }
        else
        {
            LOGERROR("Socket invalid !");
            result = eSocket_ConnectSocket_invalid;
        }
    }
    else
    {
        LOGERROR("Invalid buffer length for Socket !");
    }
    return result;
}
