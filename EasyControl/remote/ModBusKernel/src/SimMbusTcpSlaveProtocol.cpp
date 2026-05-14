#ifdef _SIM_MODBUS
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
#include <cassert>
#include "BASE/include/Endian.h"

// Package header
#include "hmtracelog.h"
#include "hmtcpip.h"
#include "MbusTcpSlaveProtocol.hpp"


MbusTcpSlaveProtocol::MbusTcpSlaveProtocol() :  MbusSlaveServer(NULL)
{
}

MbusTcpSlaveProtocol::MbusTcpSlaveProtocol(
   MbusDataTableInterface *dataTablePtr):
   MbusSlaveServer(dataTablePtr)
{
}

void MbusTcpSlaveProtocol::shutdownServer()
{
}


int32_t MbusTcpSlaveProtocol::setPort(uint16_t portNo)
{
   portNo = (uint16_t) portNo;
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
   return (FTALK_SUCCESS);
}


int32_t MbusTcpSlaveProtocol::startupServer(int32_t slaveAddr)
{
   int32_t result;

   result = assignDefaultDataTable(slaveAddr);
   if (result == FTALK_SUCCESS)
      result = startupServer((char_t *) NULL);
   return (result);
}


int32_t MbusTcpSlaveProtocol::startupServer(int32_t slaveAddr,
                                        LPCSTR const hostName)
{
   int32_t result = assignDefaultDataTable(slaveAddr);
   if (result == FTALK_SUCCESS)
      result = startupServer(hostName);
   return (result);
}

#define __HIBYTE(a) (((a)>>8) & 0xFF)
#define __LOBYTE(a) ((a) & 0xFF)
#define SWAPWORD(a) ((__LOBYTE(a) << 8) | __HIBYTE(a))


int32_t MbusTcpSlaveProtocol::serverLoop()
{
#if 0
    char msgBuffer[1024];
    int32_t rcount = 6;
    msgBuffer[0] = 0;
    msgBuffer[1] = 16;
    uint16_t startref;
    uint16_t size;

    //----------------------------------------------------
    //Start
    startref = 1024U;
    size = 4;
    msgBuffer[2] = (startref >> 8U) & 0xFF;
    msgBuffer[3] = startref & 0xFF;

    msgBuffer[4] = (size >> 8U) & 0xFF;
    msgBuffer[5] = size & 0xFF;
    msgBuffer[6] = 2*size;

    {
        uint16_t* w = (uint16_t*)&msgBuffer[7];
        for (int i = 0; i < size;i++)
        {
            w[i] = SWAPWORD(1);
        }
        processMessage(msgBuffer, 7 + msgBuffer[6]);
    }

    //----------------------------------------------------
    //Stop
    startref = 19U;
    msgBuffer[2] = (startref >> 8U) & 0xFF;
    msgBuffer[3] = startref & 0xFF;
    processMessage(msgBuffer, 7 + msgBuffer[6]);

    //----------------------------------------------------
    //TotClear
    startref = 27U;
    msgBuffer[2] = (startref >> 8U) & 0xFF;
    msgBuffer[3] = startref & 0xFF;
    processMessage(msgBuffer, 7 + msgBuffer[6]);


    //----------------------------------------------------
    //Setpoint
    startref = 367U;
    size = 8;
    msgBuffer[2] = (startref >> 8U) & 0xFF;
    msgBuffer[3] = startref & 0xFF;

    msgBuffer[4] = (size >> 8U) & 0xFF;
    msgBuffer[5] = size & 0xFF;
    msgBuffer[6] = 2 * size;

    static int k = 4;
    {
        base::CEndianBase<float32_t> b(TRUE);
        float32_t* w = (float32_t*)&msgBuffer[7];
        for (int i = 0; i < 4; i++)
        {
            w[i] = b.Get((i + 1) * k);
        }
        processMessage(msgBuffer, 7 + msgBuffer[6]);
    }
    k++;

    //----------------------------------------------------
    //LineSetpoint
    startref = 115U;
    size         = 2;
    msgBuffer[0] = 0;
    msgBuffer[1] = 16;
    msgBuffer[2] = (startref >> 8U) & 0xFF;
    msgBuffer[3] = startref & 0xFF;

    msgBuffer[4] = (size >> 8U) & 0xFF;
    msgBuffer[5] = size & 0xFF;
    msgBuffer[6] = 2 * size;

    static uint32_t lk = 100;
    {
        base::CEndianBase<float32_t> b(TRUE);
        float32_t* w = (float32_t*)&msgBuffer[7];
        *w = b.Get(lk);
        processMessage(msgBuffer, 7 + msgBuffer[6]);
    }

    //----------------------------------------------------
//LineSetpoint
    startref = 400U;
    size = 2;
    msgBuffer[0] = 0;
    msgBuffer[1] = 3;
    msgBuffer[2] = (startref >> 8U) & 0xFF;
    msgBuffer[3] = startref & 0xFF;

    msgBuffer[4] = (size >> 8U) & 0xFF;
    msgBuffer[5] = size & 0xFF;
    msgBuffer[6] = 2 * size;

    {
        processMessage(msgBuffer, 6);
        base::CEndianBase<float32_t> b(TRUE);
        float32_t* w = (float32_t*)&msgBuffer[3];
        float32_t v = b.Get(*w);
        assert(v == lk);
    }
    lk = (lk + 20) % 500;
#endif
    return (FTALK_SUCCESS);
}


#endif