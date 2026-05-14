//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MbusDataTableInterface.hpp
///
/// @file   MbusDataTableInterface.hpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/types.h"


/*****************************************************************************
 * MbusDataTableInterface class declaration
 *****************************************************************************/

class MbusDataTableInterface
{
protected:
    BOOL						m_bInitialized;


public:
   MbusDataTableInterface(void) : m_bInitialized(FALSE)
   {}

   virtual ~MbusDataTableInterface(void) = default;


public:
    virtual int32_t readHoldingRegistersTable(int32_t startRef, uint16_t regArr[],  int32_t refCnt)
    {
      (void) startRef;
      (void) regArr;
      (void) refCnt;
      return (0); // Slave will send an exception for unsupported range
    }


   virtual int32_t writeHoldingRegistersTable(int32_t startRef,  const uint16_t regArr[], int32_t refCnt) = 0
   {
      (void) startRef;
      (void) regArr;
      (void) refCnt;
      return (0); // Slave will send an exception for unsupported range
   }


   virtual int32_t readInputRegistersTable(int32_t startRef, uint16_t regArr[], int32_t refCnt)
   {
      (void) startRef;
      (void) regArr;
      (void) refCnt;
      return (0); // Slave will send an exception for unsupported range
   }



   virtual int32_t readCoilsTable(int32_t startRef, char_t bitArr[], int32_t refCnt)
   {
      (void) startRef;
      (void) bitArr;
      (void) refCnt;
      return (0); // Slave will send an exception for unsupported range
   }

   virtual int32_t writeCoilsTable(int32_t startRef, const char_t bitArr[],  int32_t refCnt)
   {
      (void) startRef;
      (void) bitArr;
      (void) refCnt;
      return (0); // Slave will send an exception for unsupported range
   }



   virtual int32_t readInputDiscretesTable(int32_t startRef,  char_t bitArr[], int32_t refCnt)
   {
      (void) startRef;
      (void) bitArr;
      (void) refCnt;
      return (0); // Slave will send an exception for unsupported range
   }


   void SetInitialized(BOOL b)
   { m_bInitialized = b; }

   BOOL GetInitialized(void) const
   { return m_bInitialized; }

   virtual void unlock()
   {}


   virtual void lock()
   {}


   virtual void timeOutHandler()
   {}

   virtual char_t readExceptionStatus()
   {  return (0);   }

   virtual BOOL Open ()
   {
       return FALSE;
   }

   virtual BOOL Close()
   {
       return FALSE;
   }

};



