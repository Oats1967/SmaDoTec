//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MbusSlaveServer.hpp
///
/// @file   MbusSlaveServer.hpp
///
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
#include "BusProtocolErrors.h"
#include "MbusDataTableInterface.hpp"


/*****************************************************************************
 * MbusSlaveServer class declaration
 *****************************************************************************/

/**
 * @brief Base class which implements the Modbus&reg; server engine
 *
 * This class realises the server engine. The server engines processes
 * Modbus messages, parses the function codes and upon receipt of a valid
 * master query it calls Data Provider methods to exchange data with the
 * user application.  For a more detailed description which Modbus data and
 * control functions have been implemented in the server engine see section
 * @ref mbusslave.
 *
 * @see MbusSlaveServer
 * @see mbusslave
 */
class MbusSlaveServer
{
  protected:

   enum
   {
      MAX_WORDS = 125,
      MAX_BITS  = 2000,

#if ((MAX_BITS + 7) / 8) > (MAX_WORDS * 2)
      MAX_DATA_SIZE = 10 + (MAX_BITS + 7) / 8
#else
      MAX_DATA_SIZE = 10 + MAX_WORDS * 2
#endif
   };

   volatile ULONG totalCounter;
   volatile ULONG successCounter;
   long timeOut;   // Time-out in ms
   MbusDataTableInterface *defaultDataTablePtr;


  private:
   uint16_t regArr[MAX_WORDS];
   char_t bitArr[MAX_BITS];


  protected:

    MbusSlaveServer(MbusDataTableInterface *dataTablePtr = NULL);


  public:

   virtual ~MbusSlaveServer();


   /**************************************************************************
    * Server Functions
    **************************************************************************/

   /**
    * @name Server Management Functions
    * @ingroup mbusslave
    */
   //@{

   int32_t addDataTable(int32_t slaveAddr, MbusDataTableInterface *dataTablePtr);

   /**
    * Modbus slave server loop
    *
    * This server loop must be called continuously. It must not be
    * blocked. The server has to be started before calling the serverLoop()
    * method.
    *
    * In most cases the server loop is executed in an endless loop:
    * @code
    * while (1)
    * {
    *   mbusProtocol.serverLoop();
    *   doOtherStuff();
    * }
    * @endcode
    *
    * @return FTALK_SUCCESS on success or error code. See @ref buserror for a
    * list of error codes.
    */
   virtual int32_t serverLoop() = 0;

   virtual void shutdownServer();

   /**
    * Returns if server has been started up
    *
    * @retval true = started
    * @retval false = shutdown
   */
   virtual int32_t isStarted() = 0;

   virtual int32_t getConnectionStatus() = 0;

   //@}


   /**************************************************************************
    * Protocol Configuration
    **************************************************************************/

   /**
    * @name Protocol Configuration
    * @ingroup mbusslave
    */
   //@{

  public:

    long setTimeout(long timeOut);


   /**
    * Returns the master time-out supervision value.
    *
    * @remark The time-out value is indicative only and not guaranteed to be
    * maintained. How precise it is followed depends on the operating system
    * used, it's scheduling priority and it's system timer resolution.
    * @return Timeout value in ms
    */
   long getTimeout()
   {
      return (timeOut);
   }

   //@}


   /**************************************************************************
    * Transmission Statistic Functions
    **************************************************************************/

   /**
    * @name Transmission Statistic Functions
    * @ingroup mbusslave
    */
   //@{

  public:

   /**
    * Returns how often a message transfer has been executed.
    *
    * @return Counter value
    */
   unsigned long getTotalCounter() { return (totalCounter); }

   /**
    * Resets total message transfer counter.
    */
   void resetTotalCounter() { totalCounter = 0UL; }

   /**
    * Returns how often a message transfer was successful.
    *
    * @return Counter value
    */
   unsigned long getSuccessCounter() { return (successCounter); }

   /**
    * Resets successful message transfer counter.
    */
   void resetSuccessCounter() { successCounter = 0UL; }

   //@}


   /**************************************************************************
    * Utility routines
    **************************************************************************/

  public:

   /**
    * @name Utility Functions
    */
   //@{

   static char_t *getPackageVersion();

   //@}


   /**************************************************************************
    * Internal subroutines
    **************************************************************************/

  protected:

   int32_t hasNoDataTable();

   int32_t assignDefaultDataTable(int32_t slaveAddr);

   int32_t processMessage(char_t msgBufArr[], int32_t msgBufCnt);


 private:

   int32_t replyIllegalDataAddress(char_t msgBufArr[]);

   int32_t replyInvalidLength(char_t msgBufArr[]);


  private:

   // Disable default operator and copy constructor
   MbusSlaveServer &operator= (MbusSlaveServer &);
   MbusSlaveServer (const MbusSlaveServer &);

};



