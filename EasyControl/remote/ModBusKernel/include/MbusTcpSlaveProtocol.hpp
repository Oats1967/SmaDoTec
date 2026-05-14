//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module #include "remote/ModBusKernel/MbusTcpSlaveProtocol.hpp"
///
/// @file   #include "remote/ModBusKernel/MbusTcpSlaveProtocol.hpp"
///
/// @coypright 2002-2006 FOCUS Software Engineering Pty Ltd, Australia.
///  All rights reserved. <www.focus-sw.com>
/// 
/// USE OF THIS SOFTWARE IS GOVERNED BY THE TERMS AND CONDITIONS OF A
/// SEPARATE LICENSE STATEMENT AND LIMITED WARRANTY.
/// 
///IN PARTICULAR, YOU WILL INDEMNIFY AND HOLD FOCUS SOFTWARE ENGINEERING,
///ITS RELATED COMPANIES AND ITS SUPPLIERS, HARMLESS FROM AND AGAINST ANY
/// CLAIMS OR LIABILITIES ARISING OUT OF THE USE, REPRODUCTION, OR
/// DISTRIBUTION OF YOUR PROGRAMS, INCLUDING ANY CLAIMS OR LIABILITIES
/// ARISING OUT OF OR RESULTING FROM THE USE, MODIFICATION, OR DISTRIBUTION
/// OF PROGRAMS OR FILES CREATED FROM, BASED ON, AND/OR DERIVED FROM THIS
/// SOURCE CODE FILE.
///
///
/// @author    FOCUS Software Engineering Pty Ltd, Australia.
///
//------------------------------------------------------------------------------------
#pragma once

// Package header
#include "hmtcpip.h"
#include "MbusSlaveServer.hpp"


/*****************************************************************************
 * Definitions
 *****************************************************************************/

/**
 * Maximum concurrent TCP/IP connections handled by server engine
 *
 * @ingroup mbusslavetcp
 */
#define MAX_CONNECTIONS 4

#if MAX_CONNECTIONS >= (FD_SETSIZE - 4) // stdin, stdout, stderr + listener
#  error MAX_CONNECTIONS too large!
#endif


/*****************************************************************************
 * MbusTcpSlaveProtocol class declaration
 *****************************************************************************/

/**
 * MODBUS/TCP Slave Protocol class
 *
 * This class realises the MODBUS/TCP slave protocol. It provides functions
 * to start-up and to execute the server engine. This server engine can
 * handle multiple master connections and is implemented as a single
 * threaded TCP server. Upon receipt of a valid master query the server
 * engine calls Data Provider methods to exchange data with the user
 * application.  For a more detailed description which Modbus data and
 * control functions have been implemented in the server engine see
 * section @ref mbusslave.
 *
 * @ingroup mbusslavetcp
 * @see mbusslave, MbusSlaveServer
 */
class MbusTcpSlaveProtocol: public MbusSlaveServer
{

  protected:

   enum
   {
      // TCP prefix
      PREFIX_LEN = 6,
      // Header is TCP prefix + 1 byte function + 1 byte address
      HDR_LEN = PREFIX_LEN + 2,
      // Frame is same as header for this protocol
      FRAME_LEN = HDR_LEN,
      // TCP max. message size is data size + frame length:
      MAX_MSG_SIZE = MAX_DATA_SIZE + FRAME_LEN
   };

   char_t bufferArr[MAX_MSG_SIZE];


  public:

   /**
    * @name MODBUS/TCP Server Management Functions
    */
   //@{

   MbusTcpSlaveProtocol();

   MbusTcpSlaveProtocol(MbusDataTableInterface *dataTablePtr);

   int32_t startupServer();

   int32_t startupServer(LPCSTR const hostName);

   int32_t startupServer(int32_t slaveAddr);

   int32_t startupServer(int32_t slaveAddr, LPCSTR const hostName);

   void shutdownServer();

   int32_t serverLoop();

   int32_t isStarted();

   int32_t getConnectionStatus();

   int32_t setPort(uint16_t portNo);

   void installIpAddrValidationCallBack(int32_t (*f) (char_t * masterIpAddrSz));


   /**
    * Returns the TCP port number used by the protocol.
    *
    * @return Port number used by the protocol
    */
   uint16_t getPort() { return (portNo); }

   //@}


  protected:

   SOCKET listenSocket;
   SOCKET connectionSocketArr[MAX_CONNECTIONS];
   uint16_t portNo; // TCP/IP port number
   int32_t masterIsConnected;

   int32_t replyMessage(char_t msgBufArr[], int32_t msgBufLen);
   int32_t (*validateMasterIpAddrFunc) (char_t * masterIpAddrSz);


  private:

   // Disable default operator and copy constructor
      MbusTcpSlaveProtocol& operator= (MbusTcpSlaveProtocol&) = delete;
   MbusTcpSlaveProtocol (const MbusTcpSlaveProtocol &) = delete;

};

