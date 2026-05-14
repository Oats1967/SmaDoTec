/**
 * @internal
 * @file hmtcpip.h
 *
 * File to include TCP/IP related header files platform independent
 *
 * @if NOTICE
 *
 * $Id: hmtcpip.h,v 1.17 2006/10/19 06:39:24 henrik Exp $
 *
 * Copyright (c) 2003-2006 FOCUS Software Engineering Pty Ltd, Australia.
 * All rights reserved. <www.focus-sw.com>
 *
 * USE OF THIS SOFTWARE IS GOVERNED BY THE TERMS AND CONDITIONS OF A
 * SEPARATE LICENSE STATEMENT AND LIMITED WARRANTY.
 *
 * IN PARTICULAR, YOU WILL INDEMNIFY AND HOLD FOCUS SOFTWARE ENGINEERING,
 * ITS RELATED COMPANIES AND ITS SUPPLIERS, HARMLESS FROM AND AGAINST ANY
 * CLAIMS OR LIABILITIES ARISING OUT OF THE USE, REPRODUCTION, OR
 * DISTRIBUTION OF YOUR PROGRAMS, INCLUDING ANY CLAIMS OR LIABILITIES
 * ARISING OUT OF OR RESULTING FROM THE USE, MODIFICATION, OR DISTRIBUTION
 * OF PROGRAMS OR FILES CREATED FROM, BASED ON, AND/OR DERIVED FROM THIS
 * SOURCE CODE FILE.
 *
 * @endif
 */


#ifndef _HMTCPIP_H_INCLUDED
#define _HMTCPIP_H_INCLUDED


/* HM platform detection */
#include "hmplatf.h"

/* Include TCP/IP related header files */
#if defined(__WIN32__)
#  if defined(UNDER_RTSS) /* RTX */
#    include <winsock2.h>
#  else
#    include <winsock.h>
#  endif
#  include <windows.h>
#elif defined(__VXWORKS__)
#  include <vxWorks.h>
#  include <sockLib.h>
#  include <ioLib.h>
#  include <netinet/tcp.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#elif defined(__PALMOS__)
#  include <PalmOS.h>
#  include <sys_socket.h>
#elif defined(__MACOS__) && !defined(__MACOSX__)
#  include <utime.h> /* Part of Metrowerks libs */
#  error Mac not yet supported!
#elif defined(__UNIX__)
#  include <unistd.h>
#  include <time.h>
#  include <sys/time.h>
#  include <sys/select.h>
#  include <sys/socket.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <netinet/tcp.h>
#  include <netinet/in.h>
#  include <errno.h>
#  include <sys/ioctl.h> /* Should contain FIONBIO */
#  if !defined(FIONBIO) /* Solaris and other UNIXes defines it here instead */
#  include <sys/filio.h>
#  endif
#else
#  error "Platform not yet supported. Please add a new condition to this #if block or update hmplatf.h"
#endif


/*****************************************************************************
 * Some definitions which are not available on every platform
 *****************************************************************************/

#ifndef INVALID_SOCKET
#  define INVALID_SOCKET ((SOCKET)(~0))
#endif

#ifndef SOCKET_ERROR
#  define SOCKET_ERROR (-1)
#endif

#ifndef SD_BOTH
#  define SD_BOTH 2
#endif

#if defined(__socklen_t_defined) || defined(socklen_t) || defined(HAVE_SOCKLEN_T) || defined(_SOCKLEN_T_DECLARED)
   typedef socklen_t SOCK_LEN_TYPE;
/* QNX 4.25 and TCP/IP 4.25 have int socklen */
#elif defined(__QNX4__) || defined(HAVE_SOCKLEN_U)
   typedef unsigned int SOCK_LEN_TYPE;
#else
   typedef int SOCK_LEN_TYPE;
#endif

#if !defined(_WINSOCKAPI_)
   typedef int SOCKET;
#endif

#if defined(__SOLARIS__) && !defined(INADDR_NONE)
#  define INADDR_NONE (-1)
#endif

#if defined(_WINSOCKAPI_)
#  define SOCKET_EWOULDBLOCK WSAEWOULDBLOCK
#  define SOCKET_EINPROGRESS WSAEINPROGRESS
#  define SOCKET_EINTR WSAEINTR
#  define socketErrno WSAGetLastError()
#else
#  define SOCKET_EWOULDBLOCK EWOULDBLOCK
#  define SOCKET_EINPROGRESS EINPROGRESS
#  define SOCKET_EINTR EINTR
#  define ioctlsocket ioctl
#  define closesocket close
#  define socketErrno errno
#endif


/* UNICODE support (MS VC++, Win CE) */
#if defined(_UNICODE)
#  ifndef _TCHAR_DEFINED
      typedef wchar_t TCHAR;
#     define _TCHAR_DEFINED
#  endif
#else
#  ifndef _TCHAR_DEFINED
      typedef char TCHAR;
#     define _TCHAR_DEFINED
#  endif
#endif


#endif /* ifdef ..._H_INCLUDED */
