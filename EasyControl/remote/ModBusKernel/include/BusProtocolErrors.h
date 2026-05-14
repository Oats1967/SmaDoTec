//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of BusProtocolErrors.h
///
/// @file   BusProtocolErrors.h
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
#ifndef _BUSPROTOCOLERRORS_H_INCLUDED
#define _BUSPROTOCOLERRORS_H_INCLUDED


/* UNICODE support (MS VC++, Win CE) */
#if defined (_UNICODE)
#  ifndef _TCHAR_DEFINED
      typedef wchar_t TCHAR;
#     define _TCHAR_DEFINED
#  endif
#else
#  ifndef _TCHAR_DEFINED
      typedef char_t TCHAR;
#     define _TCHAR_DEFINED
#  endif
#endif




/**
 * Operation was successful
 *
 * This return codes indicates no error.
 *
 * @ingroup buserror
 */
#define FTALK_SUCCESS                   0


/**
 * @name Fatal API Errors
 * 
 * Errors of this class typically indicate a programming mistake.
 * 
 */
/*@{*/

/**
 * Illegal argument error
 *
 * A parameter passed to the function returning this error code is invalid
 * or out of range.
 *
 * @ingroup buserror
 */
#define FTALK_ILLEGAL_ARGUMENT_ERROR    1

/**
 * Illegal state error
 *
 * The function is called in a wrong state. This return code is returned by
 * all functions if the protocol has not been opened succesfully yet.
 *
 * @ingroup buserror
 */
#define FTALK_ILLEGAL_STATE_ERROR      2


/**
 * Evaluation expired
 *
 * This version of the library is a function limited evaluation version
 * and has now expired.
 *
 * @ingroup buserror
 */
#define FTALK_EVALUATION_EXPIRED       3


/**
 * No data table configured
 *
 * The slave has been started without adding a data table. A data table must
 * be added by either calling addDataTable or passing it as
 * a constructor argument.
 *
 * @ingroup buserror
 */
#define FTALK_NO_DATA_TABLE_ERROR       4


/**
 * Slave address 0 illegal for serial protocols
 *
 * A slave address or unit ID of 0 is used as broadcast address for ASCII
 * and RTU protocol and therefor illegal.
 *
 * @ingroup buserror
 */
#define FTALK_ILLEGAL_SLAVE_ADDRESS     5

/*@}*/


/**
 * @name Fatal I/O Errors
 * 
 * Errors of this class signal a problem in conjunction with the I/O
 * system.
 * If errors of this class occur, the operation must be aborted and the
 * protocol closed.
 */
/*@{*/

/**
 * I/O error class
 *
 * Errors of this class signal a problem in conjunction with the I/O
 * system.
 *
 * @ingroup buserror
 */
#define FTALK_IO_ERROR_CLASS           64

/**
 * I/O error
 *
 * The underlaying I/O system reported an error.
 *
 * @ingroup buserror
 */
#define FTALK_IO_ERROR                 65

/**
 * Port or socket open error
 *
 * The TCP/IP socket or the serial port could not be opened. In case of a
 * serial port it indicates that the serial port does not exist on the
 * system.
 *
 * @ingroup buserror
 */
#define FTALK_OPEN_ERR                 66

/**
 * Serial port already open
 *
 * The serial port defined for the open operation is already opened by
 * another application.
 *
 * @ingroup buserror
 */
#define FTALK_PORT_ALREADY_OPEN        67

/**
 * TCP/IP connection error
 *
 * Signals that the TCP/IP connection could not be established.
 * Typically this error occurs when a host does not exist on the network or
 * the IP address or host name is wrong. The remote host must also listen
 * on the appropriate port.
 *
 * @ingroup buserror
 */
#define FTALK_TCPIP_CONNECT_ERR        68

/**
 * Remote peer closed TCP/IP connection
 *
 * Signals that the TCP/IP connection was closed by the remote peer or is
 * broken.
 *
 * @ingroup buserror
 */
#define FTALK_CONNECTION_WAS_CLOSED    69

/**
 * Socket library error
 *
 * The TCP/IP socket library (e.g. WINSOCK) could not be loaded or the DLL
 * is missing or not installed.
 *
 * @ingroup buserror
 */
#define FTALK_SOCKET_LIB_ERROR         70

/**
 * TCP port already bound
 *
 * Indicates that the specified TCP port cannot be bound. The port might
 * already be taken by another application or hasn't been released yet by
 * the TCP/IP stack for re-use.
 *
 * @ingroup buserror
 */
#define FTALK_PORT_ALREADY_BOUND       71

/**
 * Listen failed
 *
 * The listen operation on the specified TCP port failed..
 *
 * @ingroup buserror
 */
#define FTALK_LISTEN_FAILED            72

/**
 * File descriptors exceeded
 *
 * Maximum number of usable file descriptors exceeded.
 *
 * @ingroup buserror
 */
#define FTALK_FILEDES_EXCEEDED         73

/**
 * No permission to access serial port or TCP port
 *
 * You don't have permission to access the serial port or TCP port. Run the
 * program as root. If the error is related to a serial port, change the
 * access privilege. If it is related to TCP/IP use TCP port number which
 * is outside the IPPORT_RESERVED range.
 *
 * @ingroup buserror
 */
#define FTALK_PORT_NO_ACCESS           74

/**
 * TCP port not available
 *
 * The specified TCP port is not available on this machine.
 *
 * @ingroup buserror
 */
#define FTALK_PORT_NOT_AVAIL           75

/*@}*/


/**
 * @name Communication Errors
 * 
 * Errors of this class indicate either communication faults or Modbus
 * exceptions reported by the slave device.
 */
/*@{*/

/**
 * Fieldbus protocol error class
 *
 * Signals that a fieldbus protocol related error has
 * occured. This class is the general class of errors produced by
 * failed or interrupted data transfer functions. It is also produced when
 * receiving invalid frames or exception responses.
 *
 * @ingroup buserror
 */
#define FTALK_BUS_PROTOCOL_ERROR_CLASS 128

/**
 * Checksum error
 *
 * Signals that the checksum of a received frame is invalid.
 * A poor data link typically causes this error.
 *
 * @ingroup buserror
 */
#define FTALK_CHECKSUM_ERROR           129

/**
 * Invalid frame error
 *
 * Signals that a received frame does not correspond either by structure or
 * content to the specification or does not match a previously sent query
 * frame. A poor data link typically causes this error.
 *
 * @ingroup buserror
 */
#define FTALK_INVALID_FRAME_ERROR      130

/**
 * Invalid reply error
 *
 * Signals that a received reply does not correspond
 * to the specification.
 *
 * @ingroup buserror
 */
#define FTALK_INVALID_REPLY_ERROR      131

/**
 * Reply time-out
 *
 * Signals that a fieldbus data transfer timed out. This can occur if the
 * slave device does not reply in time or does not reply at all. A wrong
 * unit adress will also cause this error. In some occasions this exception
 * is also produced if the characters received don't constitute a complete
 * frame.
 *
 * @ingroup buserror
 */
#define FTALK_REPLY_TIMEOUT_ERROR      132

/**
 * Send time-out
 *
 * Signals that a fieldbus data send timed out. This can only occur if the
 * handshake lines are not properly set.
 *
 * @ingroup buserror
 */
#define FTALK_SEND_TIMEOUT_ERROR       133

/**
 * @brief Modbus<sup>&reg;</sup> exception response
 *
 * Signals that a Modbus exception response was received. Exception
 * responses are sent by a slave device instead of a normal response
 * message if it received the query message correctly but cannot handle the
 * query. This error usually occurs if a master queried an invalid or
 * non-existing data address or if the master used a Modbus function, which
 * is not supported by the slave device.
 *
 * @ingroup buserror
 */
#define FTALK_MBUS_EXCEPTION_RESPONSE  160

/**
 * Illegal Function exception response
 *
 * Signals that an Illegal Function exception response (code 01) was
 * received. This exception response is sent by a slave device instead of a
 * normal response message if a master sent a Modbus function, which is not
 * supported by the slave device.
 *
 * @ingroup buserror
 */
#define FTALK_MBUS_ILLEGAL_FUNCTION_RESPONSE 161

/**
 * Illegal Data Address exception response
 *
 * Signals that an Illegal Data Address exception response (code 02)
 * was received. This exception response is sent by a slave device instead
 * of a normal response message if a master queried an invalid or
 * non-existing data address.
 *
 * @ingroup buserror
 */
#define FTALK_MBUS_ILLEGAL_ADDRESS_RESPONSE 162

/**
 * Illegal Data Value exception response
 *
 * Signals that a Illegal Value exception response was (code 03)
 * received. This exception response is sent by a slave device instead of
 * a normal response message if a master sent a data value, which is not an
 * allowable value for the slave device.
 *
 * @ingroup buserror
 */
#define FTALK_MBUS_ILLEGAL_VALUE_RESPONSE 163

/**
 * Slave Device Failure exception response
 *
 * Signals that a Slave Device Failure exception response (code 04) was
 * received. This exception response is sent by a slave device instead of a
 * normal response message if an unrecoverable error occured while
 * processing the requested action. This response is also sent if the
 * request would generate a response whose size exceeds the allowable data
 * size.
 *
 * @ingroup buserror
 */
#define FTALK_MBUS_SLAVE_FAILURE_RESPONSE 164

/*@}*/


/*****************************************************************************
 * Function prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined (_WINDLL) || defined(__DLL__)
   __declspec(dllexport)
#endif
TCHAR * getBusProtocolErrorText(int32_t errCode);


#ifdef __cplusplus
}
#endif


#endif // ifdef ..._H_INCLUDED


