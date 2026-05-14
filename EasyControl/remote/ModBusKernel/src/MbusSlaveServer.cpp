//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MbusSlaveServer.cpp
///
/// @file   MbusSlaveServer.cpp
///
///
/// copyright (c) 2002-2006 FOCUS Software Engineering Pty Ltd, Australia.
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
/// @author    FOCUS Software Engineering 
///
//------------------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <windows.h>
#include <assert.h>
#include "BASE/types.h"

// Package header
#include "MbusSlaveServer.hpp"


/* Make sure that a char_t is signed! Unsigned might work, but it's not tested */
#if defined(SCHAR_MAX) && (SCHAR_MAX != 127)
#  error Sorry, but code assumes that char_t is signed!
#endif


/* Dummy function to verify .NET CLR version */
#ifdef _MANAGED
   extern "C" void _cdecl _check_commonlanguageruntime_version() {}
#endif


/**
 *
 * @brief The <i>FieldTalk</i> Modbus Slave Protocol Library's server engine
 * implements the most commonly used Modbus data functions as well as
 * some control functions. The functions to perform PLC program download
 * and other device specific functions are outside the scope of this library.
 *
 * All Bit Access and 16 Bits Access Modbus Function Codes have been
 * implemented. In addition the most frequently used Diagnostics Function
 * Codes have been implemented.
 *
 * The following table lists the functions supported by the slave:
 *
   * <table>
   *    <tr><td><b>Modbus Function Code</b></td><td><b>Current Terminology</b></td>
   *       <td><b>Classic Terminology</b></td></tr>
   * 
   *    <tr><td><b>16-bit Access</b></td></tr>
   *    <tr><td>3</td><td>Read Multiple Registers</td>
   *       <td>Read Holding Registers</td></tr>
   *    <tr><td>4</td><td>Read Input Registers</td>
   *       <td>Read Input Registers</td></tr>
   *    <tr><td>6</td><td>Write Single Register</td>
   *       <td>Preset Single Register</td></tr>
   *    <tr><td>16 (10 Hex)</td><td>Write Multiple Registers</td>
   *       <td>Preset Multiple Registers</td></tr>
   *    <tr><td>22 (16 Hex)</td><td>Mask Write Register</td>
   *       <td>Mask Write Register</td></tr>
   *    <tr><td>23 (17 Hex)</td><td>Read/Write Registers</td>
   *        <td>Read/Write Registers</td></tr>
   * 
   *    <tr><td><b>Bit access</b></td></tr>
   *    <tr><td>1</td><td>Read Coils</td><td>Read Coil Status</td></tr>
   *    <tr><td>2</td><td>Read Inputs Discretes</td>
   *       <td>Read Input Status</td></tr>
   *    <tr><td>5</td><td>Write Coil</td><td>Force Single Coil</td></tr>
   *    <tr><td>15 (0F Hex)</td><td>Force Multiple Coils</td>
   *       <td>Force Multiple Coils</td></tr>
   * 
   *    <tr><td><b>Diagnostics</b></td></tr>
   *    <tr><td>7</td><td>Read Exception Status</td>
   *       <td>Read Exception Status</td></tr>
   *    <tr><td>8 sub code 00</td><td>Diagnostics - Return Query
   *    Data</td>
   *       <td>Diagnostics - Return Query Data</td></tr>
   * </table>
 */
//@{
//@}


/*****************************************************************************
 * MbusSlaveServer class implementation
 *****************************************************************************/

/**
 * Constructs a MbusSlaveServer object and associates it with a Data
 * Provider.
 *
 * @param dataTablePtr Modbus data table pointer. Must point to a Data
 * Provider object derived from the MbusDataTableInterface class. The Data
 * Provider is the interface between your application data and the Modbus
 * network.
 */
MbusSlaveServer::MbusSlaveServer(MbusDataTableInterface *dataTablePtr)
{
//   (void) banner; // Avoid compiler warning
//   (void) ver; // Avoid compiler warning
   defaultDataTablePtr = dataTablePtr;
   totalCounter = 0UL;
   successCounter = 0UL;
   timeOut = 1000L; // Time-out in ms
}


/**
 * Destructor. Shuts down server and releases any resources.
 */
MbusSlaveServer::~MbusSlaveServer()
{
   shutdownServer();
}




/**
 * Associates a protocol object with a Data Provider and a slave
 * address.
 *
 * @param dataTablePtr Modbus data table pointer. Must point to a Data
 * Provider object derived from the MbusDataTableInterface class. The Data
 * Provider is the interface between your application data and the Modbus
 * network.
 * @return FTALK_SUCCESS on success or error code. See @ref buserror for a
 * list of error codes.
 */
int32_t MbusSlaveServer::addDataTable(int32_t slaveAddr, MbusDataTableInterface *dataTablePtr)
{
   if ((slaveAddr < -1) || (slaveAddr > 255))
      return (FTALK_ILLEGAL_ARGUMENT_ERROR);
   defaultDataTablePtr = dataTablePtr;
   return  (FTALK_SUCCESS);
}


/**
 * Shuts down the Modbus Server.
 *
 * This function also closes any associated serial ports or sockets.
 */
void MbusSlaveServer::shutdownServer()
{
}




/**
 * @internal
 * Helper routine for deprecated single data table startupServer methods.
 * It assigns the default data table to the slave address.
 *
 * @param slaveAddr Modbus slave address for server to listen on (-1 - 255).
 *                  0 is regarded as a valid value for a
 *                  MODBUS/TCP server address. A value of -1 means the server
 *                  disregards the slave address and listens to all slave
 *                  addresses. 0 or -1 is only valid for MODBUS/TCP!
 * @return FTALK_SUCCESS on success or error code. See @ref buserror for a
 * list of error codes.
 */
int32_t MbusSlaveServer::assignDefaultDataTable(int32_t slaveAddr)
{
   if ((slaveAddr < -1) || (slaveAddr > 255))
      return (FTALK_ILLEGAL_ARGUMENT_ERROR);

   //
   // API backward compatibility. At this stage we already know the data table
   // from the constructor call. We now assign it the data table to the
   // given slave address/unit ID.
   //
   return (FTALK_SUCCESS);
}


/**
 * @internal
 * Returns if server has at least one data table configured
 *
 * @retval true = data tables present
 * @retval false = no data tables added
 */
int32_t MbusSlaveServer::hasNoDataTable()
{
   return ( defaultDataTablePtr == 0);
}


/*****************************************************************************
 * Shared subroutines for Modbus functions
 *****************************************************************************/

/**
 * @internal
 * Generates an Illegal Data Address Exception reply message
 *
 * @param msgBufArr Data buffer for reply message
 */
int32_t MbusSlaveServer::replyIllegalDataAddress(char_t msgBufArr[])
{
   // Note: msgBufArr[0] contains slave address / unit ID
   msgBufArr[1] |= 0x80;
   msgBufArr[2] = 2; // Illegal Data Address exception response
   return (3);
}


/**
 * @internal
 * Generates an Invalid Length Exception reply message
 *
 * @param msgBufArr Data buffer for reply message
 */
int32_t MbusSlaveServer::replyInvalidLength(char_t msgBufArr[])
{
   // Note: msgBufArr[0] contains slave address / unit ID
   msgBufArr[1] |= 0x80;
   msgBufArr[2] = 4; // Illegal (response) Length exception
   return (3);
}


/*****************************************************************************
 * Message processor
 *****************************************************************************/

/**
 * @internal Processes a Modbus query message delivered by a master. Reads
 * and writes the data and generates the corresponding reply message. This
 * function realises the server engine of the Modbus slave.
 *
 * @param msgBufArr Data buffer with message
 * @param msgBufCnt Number of data bytes delivered by master
 * @return Number of data bytes to be replied to master
 */
int32_t MbusSlaveServer::processMessage(char_t msgBufArr[],
                                        int32_t msgBufCnt)
{
   int32_t i;
   int32_t ok;
   int32_t startRef;
   int32_t refCnt;
   int32_t replyCnt = 0;


   assert(  defaultDataTablePtr != NULL  );
   //
   // Function interpretation
   //
   switch (msgBufArr[1])
   {
      //
      // Modbus function 1 (01 hex),
      // Read Coil Status/Read Coils.
      //  &
      // Modbus function 2 (02 hex),
      // Read Inputs Status/Read Input Discretes.
      //
      // No broadcasts accepted for serial protocols, this is taken care
      // of in MbusRtuSlaveProtocol and MbusAsciiSlaveProtocol.
      //
      case 1:
      case 2:
      {
         int32_t numBytes;

         //
         // Implicit length check
         //
         if (msgBufCnt != 6)
         {
            replyCnt = replyInvalidLength(msgBufArr);
            break;
         }

         startRef = (uint16_t) ((msgBufArr[2] << 8) +
                                      (msgBufArr[3] & 0xFF));
         refCnt = (short) ((msgBufArr[4] << 8) +
                           (msgBufArr[5] & 0xFF));
         numBytes = (refCnt + 7) / 8;

         // Validate start reference and reference count
         ok = ((startRef >= 0) && (startRef <= 0xFFFF) &&
               (refCnt > 0) && (refCnt <= MAX_BITS));

         if (ok)
         {
            defaultDataTablePtr->lock();
            if (msgBufArr[1] == 2)
                ok = defaultDataTablePtr->readInputDiscretesTable(++startRef, bitArr, refCnt);
            else
                ok = defaultDataTablePtr->readCoilsTable(++startRef, bitArr, refCnt);
            defaultDataTablePtr->unlock();
         }
         if (ok)
         {
            msgBufArr[2] = (char_t) numBytes;

            //
            // Copy data into buffer
            //
            {
               int32_t byteIdx = 2; // same index as numBytes
               int32_t bitIdx;

               for (i = 0; i < refCnt; i++)
               {
                  bitIdx = i % 8;
                  if (bitIdx == 0)
                  {
                     byteIdx++;
                     msgBufArr[byteIdx] = 0;
                  }
                  if (bitArr[i])
                     msgBufArr[byteIdx] |= (char_t) (1 << bitIdx); // Set
               }
            }

            // Reply result data
            replyCnt = 3 + numBytes;
         }
         else
            replyCnt = replyIllegalDataAddress(msgBufArr);
      }
      break; // unreachable

      //
      // Modbus function 3 (03 hex),
      // Read Holding Registers/Read Multiple Registers.
      //  &
      // Modbus function 4 (04 hex),
      // Read Input Registers.
      //
      // No broadcasts accepted for serial protocols, this is taken care
      // of in MbusRtuSlaveProtocol and MbusAsciiSlaveProtocol.
      //
      case 3:
      case 4:
         //
         // Implicit length check
         //
         if (msgBufCnt != 6)
         {
            replyCnt = replyInvalidLength(msgBufArr);
            break;
         }

         startRef = (uint16_t) ((msgBufArr[2] << 8) +
                                      (msgBufArr[3] & 0xFF));
         refCnt = (short) ((msgBufArr[4] << 8) +
                           (msgBufArr[5] & 0xFF));
         // Validate start reference and reference count
         ok = ((startRef >= 0) && (startRef <= 0xFFFF) &&
               (refCnt > 0) && (refCnt <= MAX_WORDS)) ;

         if (ok)
         {
            defaultDataTablePtr->lock();
            if (msgBufArr[1] == 4)
                ok  = defaultDataTablePtr->readInputRegistersTable(++startRef, regArr, refCnt);
            else
                ok = defaultDataTablePtr->readHoldingRegistersTable(++startRef, regArr, refCnt);
            defaultDataTablePtr->unlock();
         }
         if (ok)
         {
            msgBufArr[2] = (char_t) (refCnt * 2);

            //
            // Copy data into buffer
            //
#ifdef __BIGENDIAN
            for (i = 0; i < refCnt; i++)
            {
               msgBufArr[3 + i * 2] = (char_t) (regArr[i] >> 8);
               msgBufArr[4 + i * 2] = (char_t) (regArr[i] & 0xFF);
            }
#else
            memcpy(&msgBufArr[3], regArr, refCnt * sizeof(uint16_t));
#endif
            // Reply result data
            replyCnt = 3 + refCnt * 2;
         }
         else
            replyCnt = replyIllegalDataAddress(msgBufArr);
      break;

      //
      // Modbus function 5 (05 hex),
      // Force Single Coil/Write Coil.
      //
      case 5:
         //
         // Implicit length check
         //
         if (msgBufCnt != 6)
         {
            replyCnt = replyInvalidLength(msgBufArr);
            break;
         }

         startRef = (uint16_t) ((msgBufArr[2] << 8) +
                                      (msgBufArr[3] & 0xFF));
         // Validate start reference and data
         ok = ((startRef >= 0) && (startRef <= 0xFFFF) &&
               (msgBufArr[5] == 0x00) &&
               ((msgBufArr[4] == 0x00) || (msgBufArr[4] == (char_t) -1)));

         //
         // Copy data into buffer
         //
         if (ok)
         {
            bitArr[0] = (msgBufArr[4] == (char_t) -1);
            defaultDataTablePtr->lock();
            ok = defaultDataTablePtr->writeCoilsTable(++startRef, bitArr, 1);
            defaultDataTablePtr->unlock();
         }

         if (ok)
            replyCnt = 6;
         else
         {
            // Send error reply message
            msgBufArr[1] |= 0x80;
            if ((msgBufArr[4] != 0x00) && (msgBufArr[4] != (char_t) -1))
               msgBufArr[2] = 3; // Illegal Data Value exception response
            else
               msgBufArr[2] = 2; // Illegal Data Address exception response
            replyCnt = 3;
         }
      break;

      //
      // Modbus function 6 (06 hex),
      // Preset Single Register/Write Single Register.
      //
      case 6:
         //
         // Implicit length check
         //
         if (msgBufCnt != 6)
         {
            replyCnt = replyInvalidLength(msgBufArr);
            break;
         }

         startRef = (uint16_t) ((msgBufArr[2] << 8) +
                                      (msgBufArr[3] & 0xFF));
         // Validate start reference and reference count
         ok = ((startRef >= 0) && (startRef <= 0xFFFF));

         //
         // Copy data into buffer
         //
         if (ok)
         {
            regArr[0] = (short) ((msgBufArr[4] << 8) +
                                 (msgBufArr[5] & 0xFF));
            defaultDataTablePtr->lock();
            ok = defaultDataTablePtr->writeHoldingRegistersTable(++startRef, regArr, 1);
            defaultDataTablePtr->unlock();
         }

         if (ok)
            replyCnt = 6;
         else
            replyCnt = replyIllegalDataAddress(msgBufArr);
      break;

      //
      //
      // Modbus function 7 (07 hex),
      // Read Exception Status (Serial Line only).
      //
      // No broadcasts accepted for serial protocols, this is taken care
      // of in MbusRtuSlaveProtocol and MbusAsciiSlaveProtocol.
      //
      case 7:
      {
         //
         // Implicit length check
         //
         if (msgBufCnt != 2)
         {
            replyCnt = replyInvalidLength(msgBufArr);
            break;
         }

         defaultDataTablePtr->lock();
         msgBufArr[2] = defaultDataTablePtr->readExceptionStatus();
         defaultDataTablePtr->unlock();
         replyCnt = 3;
      }
      break;

      //
      //
      // Modbus function 8 (08 hex),
      // Diagnostic
      //
      // No broadcasts accepted for serial protocols, this is taken care
      // of in MbusRtuSlaveProtocol and MbusAsciiSlaveProtocol.
      //
      case 8:
      {
         short subFunc = (short) ((msgBufArr[2] << 8) + (msgBufArr[3] & 0xFF));

         //
         // Check sub-function code
         //
         switch (subFunc)
         {
            //
            // Return query data 0 (00 hex).
            //
            case 0:
               // Loop back what has been received
               replyCnt = msgBufCnt;
            break;
            default:
               // Send error reply message
               msgBufArr[1] |= 0x80;
               msgBufArr[2] = 1; // Illegal Function exception response
               replyCnt = 3;
            break;
         }
      }
      break;

      //
      // Modbus function 15 (0F hex),
      // Force Multiple Coils.
      //
      case 15:
         //
         // Explicit length check
         //
         if (msgBufCnt != (7 + (msgBufArr[6] & 0xFF)))
         {
            replyCnt = replyInvalidLength(msgBufArr);
            break;
         }

         startRef = (uint16_t) ((msgBufArr[2] << 8) +
                                      (msgBufArr[3] & 0xFF));
         refCnt = (short) ((msgBufArr[4] << 8) +
                           (msgBufArr[5] & 0xFF));
         // Validate start reference and reference count
         ok = ((startRef >= 0) && (startRef <= 0xFFFF) &&
               ((msgBufArr[6] & 0xFF) == ((refCnt + 7) / 8)) &&
               (refCnt > 0) && (refCnt <= MAX_BITS));

         //
         // Copy data into buffer
         //
         if (ok)
         {
            {
               int32_t byteIdx = 6;
               int32_t bitIdx;
               for (i = 0; i < refCnt; i++)
               {
                  bitIdx = i % 8;
                  if (bitIdx == 0)
                     byteIdx++;
                  bitArr[i] = (((msgBufArr[byteIdx] >> bitIdx) & 0x01) == 0x01);
               }
            }
            defaultDataTablePtr->lock();
            ok = defaultDataTablePtr->writeCoilsTable(++startRef, bitArr, refCnt);
            defaultDataTablePtr->unlock();
         }
         if (ok)
            replyCnt = 6;
         else
            replyCnt = replyIllegalDataAddress(msgBufArr);
      break;

      //
      // Modbus function 16 (10 hex),
      // Preset Multiple Registers/Write Multiple Registers.
      //
      case 16:
         //
         // Explicit length check
         //
         if (msgBufCnt != (7 + (msgBufArr[6] & 0xFF)))
         {
            replyCnt = replyInvalidLength(msgBufArr);
            break;
         }

         startRef = (uint16_t) ((msgBufArr[2] << 8) +
                                      (msgBufArr[3] & 0xFF));
         refCnt = (short) ((msgBufArr[4] << 8) +
                           (msgBufArr[5] & 0xFF));
         // Validate start reference and reference count
         ok = ((startRef >= 0) && (startRef <= 0xFFFF) &&
               ((msgBufArr[6]  & 0xFF) == (refCnt * 2)) &&
               (refCnt > 0) && (refCnt <= MAX_WORDS));

         //
         // Copy data into buffer
         //
         if (ok)
         {
#ifdef __BIGENDIAN
             for (i = 0; i < refCnt; i++)
               regArr[i] = (short) ((msgBufArr[7 + i * 2] << 8) +
                                    (msgBufArr[8 + i * 2] & 0xFF));
#else
             memcpy(regArr, &msgBufArr[7], refCnt * sizeof(uint16_t));
#endif

            defaultDataTablePtr->lock();
            ok = defaultDataTablePtr->writeHoldingRegistersTable(++startRef, regArr, refCnt);
            defaultDataTablePtr->unlock();
         }

         if (ok)
            replyCnt = 6;
         else
            replyCnt = replyIllegalDataAddress(msgBufArr);
      break;

      //
      // Modbus function 22 (16 hex),
      // Mask Write Register.
      //
      case 22:
         //
         // Implicit length check
         //
         if (msgBufCnt != 8)
         {
            replyCnt = replyInvalidLength(msgBufArr);
            break;
         }

         startRef = (uint16_t) ((msgBufArr[2] << 8) +
                                      (msgBufArr[3] & 0xFF));
         // Validate start reference and reference count
         ok = ((startRef >= 0) && (startRef <= 0xFFFF));

         //
         // Copy data into buffer
         //
         if (ok)
         {
            // Get AND mask:
            regArr[1] = (short) ((msgBufArr[4] << 8) +
                                 (msgBufArr[5] & 0xFF));
            // Get OR mask:
            regArr[2] = (short) ((msgBufArr[6] << 8) +
                                 (msgBufArr[7] & 0xFF));
            defaultDataTablePtr->lock();
            ok = defaultDataTablePtr-> readHoldingRegistersTable(++startRef, regArr, 1);
            // The function’s algorithm is:
            // Result = (Current Contents AND And_Mask) OR (Or_Mask AND (NOT And_Mask))
            regArr[0] = (regArr[0] & regArr[1]) | (regArr[2] & ~regArr[1]);
            if (ok)
                ok = defaultDataTablePtr->writeHoldingRegistersTable(startRef, regArr, 1);
            defaultDataTablePtr->unlock();
         }

         if (ok)
            replyCnt = 8;
         else
            replyCnt = replyIllegalDataAddress(msgBufArr);
      break;

      //
      // Modbus function 23 (17 hex),
      // Read/Write Registers.
      //
      // No broadcasts accepted for serial protocols, this is taken care
      // of in MbusRtuSlaveProtocol and MbusAsciiSlaveProtocol.
      //
      case 23:
      {
         int32_t readRef;
         int32_t readCnt;
         int32_t writeRef;
         int32_t writeCnt;

         //
         // Explicit length check
         //
         if (msgBufCnt != (11 + (msgBufArr[10] & 0xFF)))
         {
            replyCnt = replyInvalidLength(msgBufArr);
            break;
         }

         readRef = (uint16_t) ((msgBufArr[2] << 8) +
                                     (msgBufArr[3] & 0xFF));
         readCnt = (short) ((msgBufArr[4] << 8) +
                            (msgBufArr[5] & 0xFF));
         writeRef = (uint16_t) ((msgBufArr[6] << 8) +
                                      (msgBufArr[7] & 0xFF));
         writeCnt = (short) ((msgBufArr[8] << 8) +
                             (msgBufArr[9] & 0xFF));
         // Validate start references and reference counts
         ok = ((readRef >= 0) && (readRef <= 0xFFFF) &&
               (readCnt > 0) && (readCnt <= MAX_WORDS) &&
               ((msgBufArr[10] & 0xFF) == (writeCnt * 2)) &&
               (writeRef >= 0) && (writeRef <= 0xFFFF) &&
               (writeCnt > 0) && (writeCnt <= MAX_WORDS));

         //
         // Write data into buffer
         //
        defaultDataTablePtr->lock();
        if (ok)
        {
#ifdef __BIGENDIAN
            for (i = 0; i < writeCnt; i++)
            regArr[i] = (short) ((msgBufArr[11 + i * 2] << 8) +
                                    (msgBufArr[12 + i * 2] & 0xFF));
#else
            memcpy(regArr, &msgBufArr[11], writeCnt * sizeof(uint16_t));
#endif
            ok = defaultDataTablePtr->writeHoldingRegistersTable(++writeRef, regArr, writeCnt);
        }

        if (ok)
            ok = defaultDataTablePtr->readHoldingRegistersTable(++readRef, regArr, readCnt);
        defaultDataTablePtr->unlock();
        if (ok)
        {
            msgBufArr[2] = (char_t) (readCnt * 2);

            //
            // Copy data into buffer
            //
#ifdef __BIGENDIAN
            for (i = 0; i < readCnt; i++)
            {
                msgBufArr[3 + i * 2] = (char_t)(regArr[i] >> 8);
                msgBufArr[4 + i * 2] = (char_t)(regArr[i] & 0xFF);
            }
#else
            memcpy(&msgBufArr[3], regArr, readCnt * sizeof(uint16_t));
#endif
            // Reply result data
            replyCnt = 3 + readCnt * 2;
        }
        else
            replyCnt = replyIllegalDataAddress(msgBufArr);
      }
      break;

      //
      // Illegal Function
      //
      // No broadcasts accepted for serial protocols, this is taken care
      // of in MbusRtuSlaveProtocol and MbusAsciiSlaveProtocol.
      //
      default:
         // Send error reply message
         msgBufArr[1] |= 0x80;
         msgBufArr[2] = 1; // Illegal Function exception response
         replyCnt = 3;
      break;
   }

   return (replyCnt);
}


/*****************************************************************************
 * Protocol Configuration
 *****************************************************************************/

/**
 * Configures master transmit time-out supervision. The slave can monitor
 * whether a master is actually transmitting characters or not.
 * This function sets the transmit time-out to the specified value.
 * A value of 0 disables the time-out,
 * which stops time-out notifications being sent to the Data Provider.
 *
 * @remark The time-out value is indicative only and not guaranteed to be
 * maintained. How precise it is followed depends on the operating system
 * used, it's scheduling priority and it's system timer resolution.
 * @note The time-out does not check if a master is sending valid frames.
 * @param timeOut Timeout value in ms (Range: 0 - 100000), 0 disables
 * time-out
 * @retval FTALK_SUCCESS Success
 * @retval FTALK_ILLEGAL_ARGUMENT_ERROR Argument out of range
 */
long MbusSlaveServer::setTimeout(long _timeOut)
{
   if ((_timeOut < 0) || (_timeOut > 100000))
      return (FTALK_ILLEGAL_ARGUMENT_ERROR);
   this->timeOut = _timeOut;

   return (FTALK_SUCCESS);
}


/*****************************************************************************
 * Utility routines
 *****************************************************************************/

/**
 * Returns the package version number.
 *
 * @return Package version string
 * @ingroup mbusslave
 */

