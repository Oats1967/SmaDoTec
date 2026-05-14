#ifndef _SIM_MODBUS
//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MbusTcpSlaveProtocol.cpp
///
/// The Server Engine of the MODBUS/TCP slave protocol is implemented in the
/// class MbusTcpSlaveProtocol. It provides functions to start-up and to
/// execute the server engine. This server engine can handle multiple master
/// connections and is implemented as a single threaded TCP server. Upon
/// receipt of a valid master query the server engine calls Data Provider
/// methods to exchange data with the user application.  For a more detailed
/// description which Modbus data and control functions have been
/// implemented in the server engine see section @ref mbusslave.
/// @note If the configured TCP port is below IPPORT_RESERVED (usually
/// 1024), the process has to run with root privilege! This applies if you
/// are using the default MODBUS/TCP port 502.
/// See section @ref mbustcpprotocol for some background information about
/// MODBUS/TCP.
/// See section @ref tcpintegrate for an example how to use the
/// MbusTcpSlaveProtocol class.
///
///
/// @file   MbusTcpSlaveProtocol.cpp
///
///
/// copyright (c) 2002-2006 FOCUS Software Engineering Pty Ltd, Australia.
///  All rights reserved. <www.focus-sw.com>
/// 
/// USE OF THIS SOFTWARE IS GOVERNED BY THE TERMS AND CONDITIONS OF A
/// SEPARATE LICENSE STATEMENT AND LIMITED WARRANTY.
/// 
/// IN PARTICULAR, YOU WILL INDEMNIFY AND HOLD FOCUS SOFTWARE ENGINEERING,
/// ITS RELATED COMPANIES AND ITS SUPPLIERS, HARMLESS FROM AND AGAINST ANY
/// CLAIMS OR LIABILITIES ARISING OUT OF THE USE, REPRODUCTION, OR
/// DISTRIBUTION OF YOUR PROGRAMS, INCLUDING ANY CLAIMS OR LIABILITIES
/// ARISING OUT OF OR RESULTING FROM THE USE, MODIFICATION, OR DISTRIBUTION
/// OF PROGRAMS OR FILES CREATED FROM, BASED ON, AND/OR DERIVED FROM THIS
/// SOURCE CODE FILE.
///
///
/// @author    FOCUS Software Engineering 
///
//------------------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "BASE/types.h"

// Package header
#include "hmlib/include/hmtracelog.h"
#include "hmlib/include/hmtcpip.h"
#include "include/MbusTcpSlaveProtocol.hpp"

// Version identifier

int32_t kDummy = 0; // Nicht loeschen

/*****************************************************************************
 * MbusTcpSlaveProtocol class implementation
 *****************************************************************************/


//------------------------------------------------------------------------------------
///  @brief   MbusTcpSlaveProtocol
///
///			  Constructs a MbusTcpSlaveProtocol object.
///
///  The association with a Data Provider is done after construction using the
///  addDataTable method.
///
///  @author  FOCUS Software Engineering 
///
///  @class        MbusTcpSlaveProtocol
///
///  @param[in] none
///  @return none
///
//------------------------------------------------------------------------------------
MbusTcpSlaveProtocol::MbusTcpSlaveProtocol() :  MbusSlaveServer(NULL)
{
   int32_t i;

   portNo = 502; // TCP/IP port number, defaults to 502
   listenSocket = INVALID_SOCKET;
   for (i = 0; i < MAX_CONNECTIONS; i++)
      connectionSocketArr[i] = INVALID_SOCKET;
   validateMasterIpAddrFunc = NULL;
   masterIsConnected = 0;
   bufferArr[0] = 0;
}


/**
 * Constructs a MbusTcpSlaveProtocol object data and
 * associates it with a Data Provider.
 *
 * Function is kept for compatibility with previous API versions, do not use
 * for new implementations.
 *
 * @param dataTablePtr Modbus data table pointer. Must point to a Data
 * Provider object derived from the MbusDataTableInterface class. The Data
 * Provider is the interface between your application data and the Modbus
 * network.
 * @deprecated This function is deprecated. The preferred way of assigning a
 * dataTable is using the default constructor and configuring data table
 * and slave address using addDataTable method.
 */
MbusTcpSlaveProtocol::MbusTcpSlaveProtocol(
   MbusDataTableInterface *dataTablePtr):
   MbusSlaveServer(dataTablePtr)
{
   int32_t i;

   portNo = 502; // TCP/IP port number, defaults to 502
   listenSocket = INVALID_SOCKET;
   for (i = 0; i < MAX_CONNECTIONS; i++)
      connectionSocketArr[i] = INVALID_SOCKET;
   validateMasterIpAddrFunc = NULL;
   masterIsConnected = 0;
}


//------------------------------------------------------------------------------------
///  @brief   shutdownServer
///
///			  Shuts down the Modbus server.
///           This function closes all TCP/IP connections to MODBUS/TCP masters and
///			  releases any system resources associated with the connections
///
///  @author  Detlef Hafer
///
///  @class        MbusTcpSlaveProtocol
///
///  @param[in] none
///  @return none
///
//------------------------------------------------------------------------------------
void MbusTcpSlaveProtocol::shutdownServer()
{
   int32_t i;

   //
   // Close connection sockets
   //
   for (i = 0; i < MAX_CONNECTIONS; i++)
      if (connectionSocketArr[i] != INVALID_SOCKET)
      {
         shutdown(connectionSocketArr[i], SD_BOTH);
         closesocket(connectionSocketArr[i]);
         connectionSocketArr[i] = INVALID_SOCKET;
      }

   //
   // Close listening socket
   //
   if (listenSocket == INVALID_SOCKET)
      return;
   shutdown (listenSocket, SD_BOTH);
   closesocket(listenSocket);
#ifdef _WINSOCKAPI_
   // Unload the Winsock API
   WSACleanup();
#endif
   listenSocket = INVALID_SOCKET;
   masterIsConnected = 0;
   if (defaultDataTablePtr)
   {
       defaultDataTablePtr->Close();
   }
}


//------------------------------------------------------------------------------------
///  @brief   setPort
///
/// Sets the TCP port number to be used by the protocol.
///
/// @remark Usually the port number remains unchanged and defaults to
/// 502. In this case no call to this function is necessary. However if
/// the port number has to be different from 502 this function must be
/// called <i>before</i> starting the server with startupServer().
/// 
/// @note If the configured TCP port is below IPPORT_RESERVED (usually
/// 1024), the process has to run with root privilege!
///
///  @author  Detlef Hafer
///
///  @class        MbusTcpSlaveProtocol
///
///  @param[in] portNo , Port number the server shall listen on
///  @return FTALK_SUCCESS Success, FTALK_ILLEGAL_STATE_ERROR Server already running
///
//------------------------------------------------------------------------------------
int32_t MbusTcpSlaveProtocol::setPort(uint16_t _portNo)
{
   if (isStarted())
      return (FTALK_ILLEGAL_STATE_ERROR);
   this->portNo = (uint16_t)_portNo;
   return (FTALK_SUCCESS);
}


/**
 * This function installs a callback handler for validating a
 * master's IP address. Pass a pointer to a function with checks a master's
 * IP address and either accepts or rejects a master's connection.
 *
 * @param masterIpAddrSz IPv4 Internet host address string
 * in the standard numbers-and-dots notation.
 *
 * @return Returns 1 to accept a connection or 0 to reject it.
 */
void MbusTcpSlaveProtocol::
installIpAddrValidationCallBack(int32_t (*f) (char_t * masterIpAddrSz))
{
   validateMasterIpAddrFunc = f;
}


/**
 * Returns if server has been started up
 *
 * @retval true = started
 * @retval false = shutdown
 */
int32_t MbusTcpSlaveProtocol::isStarted()
{
   return (listenSocket != INVALID_SOCKET);
}


/**
 * Checks if a Modbus master is polling periodically
 *
 * @retval true = A master is polling at a frequency higher than the
 * master transmit time-out value
 * @retval false = No master is polling within the time-out period
 * @note The master transmit time-out value must be set > 0 in order for this
 * function to work.
 */
int32_t MbusTcpSlaveProtocol::getConnectionStatus()
{
   return masterIsConnected;
}


/**
 * Puts the Modbus server into operation.
 * The server accepts connections on any interface.
 *
 * This function opens a TCP/IP socket, binds the configured TCP port to the
 * Modbus/TCP protocol and initialises the server engine.
 *
 * @note If the configured TCP port is below IPPORT_RESERVED (usually
 * 1024), the process has to run with root privilege!
 * @return FTALK_SUCCESS on success or error code. See @ref buserror for a
 * list of error codes.
 */
int32_t MbusTcpSlaveProtocol::startupServer()
{
   return (startupServer((char_t *) NULL));
}


/**
 * Puts the Modbus server into operation.
 *
 * The server accepts connections only on the interfaces which match the
 * supplied hostname or IP address. This method allows to run different
 * servers on multiple interfaces (so called multihomed servers).
 *
 * This function opens a TCP/IP socket, binds the configured TCP port to the
 * Modbus/TCP protocol and initialises the server engine.
 *
 * @note If the configured TCP port is below IPPORT_RESERVED (usually
 * 1024), the process has to run with root privilege!
 * @param hostName  String with IP address for a specific host interface or
 *                  NULL if connections are accepted on any interface
 * @return FTALK_SUCCESS on success or error code. See @ref buserror for a
 *         list of error codes.
 */
int32_t MbusTcpSlaveProtocol::startupServer(LPCSTR const hostName)
{
   int32_t result;
   int32_t tcpOption;
   struct sockaddr_in hostAddress;

   if (isStarted())
      return (FTALK_ILLEGAL_STATE_ERROR);

   if (hasNoDataTable())
      return (FTALK_NO_DATA_TABLE_ERROR);

   //
   // Special treatment for the Win32 platform, needs to load WinSock DLL
   //
#ifdef _WINSOCKAPI_
   WSADATA wsaData;

   result = WSAStartup(0x0101, &wsaData);
   if (result != 0)
      return (FTALK_SOCKET_LIB_ERROR);
#endif

   //
   // Open socket
   //
   listenSocket = socket(PF_INET, SOCK_STREAM, 0);
   if (listenSocket == INVALID_SOCKET)
   {
      shutdownServer();
      return (FTALK_OPEN_ERR);
   }

   //
   // Configure listen socket options (we ignore errors here)
   //
#ifdef SO_REUSEADDR
   tcpOption = 1; // Enable option
   setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR,
              (char_t *) &tcpOption, sizeof (tcpOption));
#endif

   //
   // Binding the listen socket to the port
   //
   memset(&hostAddress, 0, sizeof(hostAddress));
   hostAddress.sin_family = AF_INET;
   if ((hostName == NULL) || (hostName[0] == '\0'))
      hostAddress.sin_addr.s_addr = htonl(INADDR_ANY);
   else
   {
      hostAddress.sin_addr.s_addr = inet_addr((char_t *) hostName);
#if !defined(__VXWORKS__) // We don't support host name resolving with VxWorks
      if (hostAddress.sin_addr.s_addr == INADDR_NONE)
      {
         struct hostent *hostInfo;

         hostInfo = gethostbyname((char_t *) hostName);

         if (hostInfo == NULL)
            return (FTALK_TCPIP_CONNECT_ERR);
         hostAddress.sin_addr = *(struct in_addr *) hostInfo->h_addr;
      }
#endif
   }
   hostAddress.sin_port = htons(portNo);
   result = bind(listenSocket, (struct sockaddr *) &hostAddress,
                 sizeof (hostAddress));
   if (result == SOCKET_ERROR)
   {
      shutdownServer();
      switch (socketErrno)
      {
#ifdef _WINSOCKAPI_
         case WSAEACCES:
         return (FTALK_PORT_NO_ACCESS);
         case WSAEADDRINUSE:
         return (FTALK_PORT_ALREADY_BOUND);
         case WSAEADDRNOTAVAIL:
         default:
         return (FTALK_PORT_NOT_AVAIL);
#else
         case ENOTCONN: // Linux 7.2 reports this error no if no root privilege
         case EACCES:
         return (FTALK_PORT_NO_ACCESS);
         case EADDRINUSE:
         return (FTALK_PORT_ALREADY_BOUND);
         case EADDRNOTAVAIL:
         default:
         return (FTALK_PORT_NOT_AVAIL);
#endif
      }
   }

   //
   // Start listening to incoming connections
   //
   result = listen(listenSocket,
               ((MAX_CONNECTIONS < SOMAXCONN) ? MAX_CONNECTIONS : SOMAXCONN));
   if (result == SOCKET_ERROR)
   {
      shutdownServer();
      return (FTALK_LISTEN_FAILED);
   }
    if (defaultDataTablePtr)
    {
        defaultDataTablePtr->Open();
    }
   return (FTALK_SUCCESS);
}


/**
 * Puts the Modbus server into operation using a single slave address and
 * data table. The server accepts connections on any interface.
 *
 * Function is kept for compatibility with previous API versions, do not use
 * for new implementations.
 *
 * @note If the configured TCP port is below IPPORT_RESERVED (usually
 * 1024), the process has to run with root privilege!
 * @param slaveAddr Modbus slave address for server to listen on (-1 - 255).
 *                  0 is regarded as a valid value for a
 *                  MODBUS/TCP server address. A value of -1 means the server
 *                  disregards the slave address and listens to all slave
 *                  addresses.
 * @return FTALK_SUCCESS on success or error code. See @ref buserror for a
 * list of error codes.
 * @deprecated This function is deprecated. The preferred way of assigning a
 * slave address is using the default constructor and configuring data table
 * and slave address using addDataTable method.
 */
int32_t MbusTcpSlaveProtocol::startupServer(int32_t slaveAddr)
{
   int32_t result;

   result = assignDefaultDataTable(slaveAddr);
   if (result == FTALK_SUCCESS)
      result = startupServer((char_t *) NULL);
   return (result);
}


/**
 * Puts the Modbus server into operation using a single slave address and
 * data table.
 *
 * Function is kept for compatibility with previous API versions, do not use
 * for new implementations.
 *
 * @param slaveAddr Modbus slave address for server to listen on (-1 - 255).
 *                  0 is regarded as a valid value for a
 *                  MODBUS/TCP server address. A value of -1 means the server
 *                  disregards the slave address and listens to all slave
 *                  addresses.
 * @param hostName  String with IP address for a specific host interface or
 *                  NULL if connections are accepted on any interface
 * @return FTALK_SUCCESS on success or error code. See @ref buserror for a
 *         list of error codes.
 * @deprecated This function is deprecated. The preferred way of assigning a
 * slave address is using the default constructor and configuring data table
 * and slave address using addDataTable method.
 */
int32_t MbusTcpSlaveProtocol::startupServer(int32_t slaveAddr,
                                        LPCSTR const hostName)
{
   int32_t result = assignDefaultDataTable(slaveAddr);
   if (result == FTALK_SUCCESS)
      result = startupServer(hostName);
   return (result);
}


/**
 * MODBUS/TCP slave server loop.
 *
 * This server loop must be called continuously. It must not be
 * blocked. The server has to be started before calling the serverLoop()
 * method. This server engine can handle multiple TCP/IP connections at the
 * same time.
 *
 * @return FTALK_SUCCESS on success or error code. See @ref buserror for a
 * list of error codes.
 */
int32_t MbusTcpSlaveProtocol::serverLoop()
{
   int32_t result;
   int32_t sockIdx;
   int32_t recvResult;
   int32_t sendResult;
   fd_set fdSet;
   timeval timeVal;
   SOCKET maxFileDes;
   int32_t replyCnt;
   int32_t tcpOption;


    if ( ! isStarted())
    {
      return (FTALK_ILLEGAL_STATE_ERROR);
    }

    if ( hasNoDataTable() )
    {
      return (FTALK_NO_DATA_TABLE_ERROR);
    }

   //
   // Prepare file descriptor set for select call
   //
   FD_ZERO (&fdSet);
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127)
#endif
   FD_SET (listenSocket, &fdSet);
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
   maxFileDes = listenSocket;
   for (sockIdx = 0; sockIdx < MAX_CONNECTIONS; sockIdx++)
   {
      if (connectionSocketArr[sockIdx] != INVALID_SOCKET)
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127)
#endif
         FD_SET (connectionSocketArr[sockIdx], &fdSet);
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
      if (connectionSocketArr[sockIdx] > maxFileDes)
         maxFileDes = connectionSocketArr[sockIdx];
   }

   //
   // Block until accept request or received data or time-out
   //
   timeVal.tv_sec = (long) timeOut / 1000L;
   timeVal.tv_usec = ((long) timeOut % 1000L) * 1000L;
   if (timeOut == 0)
      result = select((int32_t) maxFileDes + 1, &fdSet, NULL, NULL, NULL);
   else
      result = select((int32_t) maxFileDes + 1, &fdSet, NULL, NULL, &timeVal);
   if (result == SOCKET_ERROR)
      return (FTALK_FILEDES_EXCEEDED);

   //
   // Check for time-out
   //
   if (result == 0)
   {
      TRACELOG1("Slave poll time-out!\n");
      masterIsConnected = 0;
      if ( defaultDataTablePtr != NULL)
        defaultDataTablePtr->timeOutHandler();
   }

   //
   // Connection accept request
   //
   if (FD_ISSET (listenSocket, &fdSet))
   {
      // Search a free socket
      for (sockIdx = 0; sockIdx < MAX_CONNECTIONS; sockIdx++)
      {
         if (connectionSocketArr[sockIdx] == INVALID_SOCKET)
         {
            struct sockaddr_in peerAddr;
            SOCK_LEN_TYPE peerAddrLen = sizeof(peerAddr);

            // Yes, socket is free, try to accept a connection on it
            connectionSocketArr[sockIdx] = accept(listenSocket,
                                                  (struct sockaddr *) &peerAddr,
                                                  &peerAddrLen);
            if (connectionSocketArr[sockIdx] != INVALID_SOCKET)
            {
               //
               // Check if connection shall be accepted
               //
               if (validateMasterIpAddrFunc != NULL)
               {
                  if (!validateMasterIpAddrFunc(inet_ntoa(peerAddr.sin_addr)))
                  {
                     shutdown(connectionSocketArr[sockIdx], SD_BOTH);
                     closesocket(connectionSocketArr[sockIdx]);
                     connectionSocketArr[sockIdx] = INVALID_SOCKET;
                     TRACELOG2("Connection rejected on slot %d\n", sockIdx);
                  }
               }

               //
               // Set socket options (we ignore errors here, not critical)
               //
#ifdef TCP_NODELAY
               tcpOption = 1; // Enable option
               setsockopt(connectionSocketArr[sockIdx],
                          IPPROTO_TCP, TCP_NODELAY,
                          (char_t *) &tcpOption, sizeof (tcpOption));
#endif
#ifdef SO_SNDBUF
               tcpOption = MAX_MSG_SIZE;
               setsockopt(connectionSocketArr[sockIdx],
                          SOL_SOCKET, SO_SNDBUF,
                          (char_t *) &tcpOption, sizeof (tcpOption));
#endif
#ifdef SO_RCVBUF
               tcpOption = MAX_MSG_SIZE;
               setsockopt(connectionSocketArr[sockIdx],
                          SOL_SOCKET, SO_RCVBUF,
                          (char_t *) &tcpOption, sizeof (tcpOption));
#endif
#ifdef SO_LINGER
               tcpOption = 0; // Disable option = discard unsent data when closing
               setsockopt(connectionSocketArr[sockIdx],
                          SOL_SOCKET, SO_LINGER,
                          (char_t *) &tcpOption, sizeof (tcpOption));
#endif
               TRACELOG2("Connection accepted on slot %d\n", sockIdx);
            }
            break; // Leave for loop
         }
      }
   }

   //
   // Data received on socket
   //
   for (sockIdx = 0; sockIdx < MAX_CONNECTIONS; sockIdx++)
      if (connectionSocketArr[sockIdx] != INVALID_SOCKET)
         if (FD_ISSET (connectionSocketArr[sockIdx], &fdSet))
         {
            recvResult = recv (connectionSocketArr[sockIdx],(char_t *) bufferArr, sizeof (bufferArr), 0);
			sendResult = 0;
			replyCnt = 0;
            
            //
            // Process client message
            //
            if (recvResult >= PREFIX_LEN) // Process only minimum message sizes
            {
               short dataLen = (short) ((bufferArr[4] << 8) | (bufferArr[5] & 0xFF));
               // Validate length before processing message
               if (((dataLen + PREFIX_LEN) == recvResult) && (defaultDataTablePtr != NULL))
               {
                  replyCnt = processMessage(&bufferArr[PREFIX_LEN],
                                            recvResult - PREFIX_LEN);
                  // The first two bytes (msg id) are returned untouched
                  bufferArr[2] = 0; // protocol identifier
                  bufferArr[3] = 0; // protocol identifier
                  bufferArr[4] = (char_t) ((replyCnt) >> 8);
                  bufferArr[5] = (char_t) ((replyCnt) & 0xFF);
                  sendResult = send(connectionSocketArr[sockIdx],
                                    (char_t *) bufferArr,
                                    replyCnt + PREFIX_LEN, 0);
                  masterIsConnected = 1;
               }
            }

            //
            // Check for disconnection and errors
            //
            if ((recvResult < PREFIX_LEN) ||
                (sendResult != replyCnt + PREFIX_LEN))
            {
               //
               // Free socket
               //
               shutdown(connectionSocketArr[sockIdx], SD_BOTH);
               closesocket(connectionSocketArr[sockIdx]);
               connectionSocketArr[sockIdx] = INVALID_SOCKET;
               if (recvResult == 0)
                  TRACELOG2("Disconnected slot %d nicely by other peer.\n",
                            sockIdx);
               else
                  TRACELOG2("Forced disconnection on slot %d!\n", sockIdx);
            }
         }

   return (FTALK_SUCCESS);
}
#endif

