//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module BusProtocolErrors.c
///
/// @file   BusProtocolErrors.c
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "BASE/types.h"
#include "hmplatf.h"

/* Platform header */
#if defined(__PALMOS__)
#  include <PalmOS.h>
#else
#  include <stdlib.h>
#endif

/* Include FieldTalk package header */
#include "BusProtocolErrors.h"

/* UNICODE support (MS VC++, Win CE) */
#if defined (_UNICODE)
#  define STR_TYPE L
#else
#  define STR_TYPE
#endif


#define FTALK_PORT_ALREADY_BOUND_TEXT       STR_TYPE"TCP Port alread bound"
#define FTALK_SUCCESS_TEXT                  STR_TYPE"Operation was successful"
#define FTALK_ILLEGAL_ARGUMENT_ERROR_TEXT   STR_TYPE"Illegal argument error"
#define FTALK_ILLEGAL_STATE_ERROR_TEXT      STR_TYPE"Illegal state error"
#define FTALK_EVALUATION_EXPIRED_TEXT       STR_TYPE"Evaluation expired"
#define FTALK_NO_DATA_TABLE_ERROR_TEXT      STR_TYPE"No data table configured"
#define FTALK_ILLEGAL_SLAVE_ADDRESS_TEXT    STR_TYPE"Slave address 0 is illegal in this context"
#define FTALK_IO_ERROR_CLASS_TEXT           STR_TYPE"I/O error class"
#define FTALK_IO_ERROR_TEXT                 STR_TYPE"I/O error"
#define FTALK_OPEN_ERR_TEXT                 STR_TYPE"Port or socket open error"
#define FTALK_PORT_ALREADY_OPEN_TEXT        STR_TYPE"Serial port already open"
#define FTALK_TCPIP_CONNECT_ERR_TEXT        STR_TYPE"TCP/IP connection error"
#define FTALK_CONNECTION_WAS_CLOSED_TEXT    STR_TYPE"TCP/IP connection was closed by remote peer"
#define FTALK_SOCKET_LIB_ERROR_TEXT         STR_TYPE"Socket library error"
#define FTALK_LISTEN_FAILED_TEXT            STR_TYPE"Listen failed"
#define FTALK_FILEDES_EXCEEDED_TEXT         STR_TYPE"File descriptors exceeded"
#define FTALK_PORT_NO_ACCESS_TEXT           STR_TYPE"No permission to access serial or TCP port"
#define FTALK_PORT_NOT_AVAIL_TEXT           STR_TYPE"TCP Port not available"
#define FTALK_BUS_PROTOCOL_ERROR_CLASS_TEXT STR_TYPE"Fieldbus protocol error class"
#define FTALK_CHECKSUM_ERROR_TEXT           STR_TYPE"Checksum error"
#define FTALK_INVALID_FRAME_ERROR_TEXT      STR_TYPE"Invalid frame error"
#define FTALK_INVALID_REPLY_ERROR_TEXT      STR_TYPE"Invalid reply error"
#define FTALK_REPLY_TIMEOUT_ERROR_TEXT      STR_TYPE"Reply time-out"
#define FTALK_SEND_TIMEOUT_ERROR_TEXT       STR_TYPE"Send time-out"
#define FTALK_MBUS_EXCEPTION_RESPONSE_TEXT  STR_TYPE"Modbus exception response"
#define FTALK_MBUS_ILLEGAL_FUNCTION_RESPONSE_TEXT STR_TYPE"Illegal Function exception response"
#define FTALK_MBUS_ILLEGAL_ADDRESS_RESPONSE_TEXT  STR_TYPE"Illegal Data Address exception response"
#define FTALK_MBUS_ILLEGAL_VALUE_RESPONSE_TEXT    STR_TYPE"Illegal Data Value exception response"
#define FTALK_MBUS_SLAVE_FAILURE_RESPONSE_TEXT    STR_TYPE"Slave Device Failure exception response"

#define FTALK_UNKNOWN_ERROR                       STR_TYPE"Unknown error"


struct TextEntry
{
   int32_t errorNo;
   TCHAR *textMsg;
};


static const struct TextEntry entryListArr[] =
{
   {FTALK_SUCCESS, FTALK_SUCCESS_TEXT},
   {FTALK_ILLEGAL_ARGUMENT_ERROR, FTALK_ILLEGAL_ARGUMENT_ERROR_TEXT},
   {FTALK_ILLEGAL_STATE_ERROR, FTALK_ILLEGAL_STATE_ERROR_TEXT},
   {FTALK_EVALUATION_EXPIRED, FTALK_EVALUATION_EXPIRED_TEXT},
   {FTALK_NO_DATA_TABLE_ERROR, FTALK_NO_DATA_TABLE_ERROR_TEXT},
   {FTALK_ILLEGAL_SLAVE_ADDRESS, FTALK_ILLEGAL_SLAVE_ADDRESS_TEXT},

   {FTALK_IO_ERROR_CLASS, FTALK_IO_ERROR_CLASS_TEXT},
   {FTALK_IO_ERROR, FTALK_IO_ERROR_TEXT},
   {FTALK_OPEN_ERR, FTALK_OPEN_ERR_TEXT},
   {FTALK_PORT_ALREADY_OPEN, FTALK_PORT_ALREADY_OPEN_TEXT},
   {FTALK_TCPIP_CONNECT_ERR, FTALK_TCPIP_CONNECT_ERR_TEXT},
   {FTALK_CONNECTION_WAS_CLOSED, FTALK_CONNECTION_WAS_CLOSED_TEXT},
   {FTALK_SOCKET_LIB_ERROR, FTALK_SOCKET_LIB_ERROR_TEXT},
   {FTALK_PORT_ALREADY_BOUND, FTALK_PORT_ALREADY_BOUND_TEXT},
   {FTALK_LISTEN_FAILED, FTALK_LISTEN_FAILED_TEXT},
   {FTALK_FILEDES_EXCEEDED, FTALK_FILEDES_EXCEEDED_TEXT},
   {FTALK_PORT_NO_ACCESS, FTALK_PORT_NO_ACCESS_TEXT},
   {FTALK_PORT_NOT_AVAIL, FTALK_PORT_NOT_AVAIL_TEXT},

   {FTALK_BUS_PROTOCOL_ERROR_CLASS, FTALK_BUS_PROTOCOL_ERROR_CLASS_TEXT},
   {FTALK_CHECKSUM_ERROR, FTALK_CHECKSUM_ERROR_TEXT},
   {FTALK_INVALID_FRAME_ERROR, FTALK_INVALID_FRAME_ERROR_TEXT},
   {FTALK_INVALID_REPLY_ERROR, FTALK_INVALID_REPLY_ERROR_TEXT},
   {FTALK_REPLY_TIMEOUT_ERROR, FTALK_REPLY_TIMEOUT_ERROR_TEXT},
   {FTALK_SEND_TIMEOUT_ERROR, FTALK_SEND_TIMEOUT_ERROR_TEXT},
   {FTALK_MBUS_EXCEPTION_RESPONSE, FTALK_MBUS_EXCEPTION_RESPONSE_TEXT},
   {FTALK_MBUS_ILLEGAL_FUNCTION_RESPONSE, FTALK_MBUS_ILLEGAL_FUNCTION_RESPONSE_TEXT},
   {FTALK_MBUS_ILLEGAL_ADDRESS_RESPONSE, FTALK_MBUS_ILLEGAL_ADDRESS_RESPONSE_TEXT},
   {FTALK_MBUS_ILLEGAL_VALUE_RESPONSE, FTALK_MBUS_ILLEGAL_VALUE_RESPONSE_TEXT},
   {FTALK_MBUS_SLAVE_FAILURE_RESPONSE, FTALK_MBUS_SLAVE_FAILURE_RESPONSE_TEXT},
   {-1, NULL} /* Last entry mark */
};


/**
 * Returns an error text string for a given error code
 *
 * @param errCode FieldTalk error code
 * @return Error text string
 * @ingroup buserror
 */
TCHAR * getBusProtocolErrorText(int32_t errCode)
{
   int32_t i = 0;

   while (entryListArr[i].textMsg != NULL)
   {
      if (entryListArr[i].errorNo == errCode)
         return (entryListArr[i].textMsg);
      i++;
   }
   return (FTALK_UNKNOWN_ERROR);
}

