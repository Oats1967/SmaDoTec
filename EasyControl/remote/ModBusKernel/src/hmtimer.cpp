//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of hmtimer.cpp
///
/// @file   hmtimer.cpp
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
/// 
///
/// These functions form a Windows specific implementation of a platform
/// independant interface layer for access to timers.
///
/// @warning These timer routines depend on the system clock. The system
/// clock must not be altered while any of these timers are in
/// use or inaccurate timing will occur.
///
/// @remark Windows versions currently supported by this file:
/// - Windows 95
/// - Windows 98
/// - Windows NT 4.0
/// - Windows 2000
///
///
///
/// @author    FOCUS Software Engineering 
///
//------------------------------------------------------------------------------------
#include <limits.h>
#include "BASE/types.h"

// HM C/C++ library header
#include "hmtimer.hpp"

// Version identifier
static char_t ver[] = "$Id: hmtimer.cpp,v 1.10 2004/08/29 03:20:21 henrik Exp $";



/*****************************************************************************
 * Helper functions
 *****************************************************************************/

/**
 * Returns time as a 64 bit FILETIME number.
 *
 * Win CE 4.2 does not provide the GetSystemTimeAsFileTime call, 
 * so we have to provide our own.
 */
static BOOL getSystemTimeAsFileTime(LPFILETIME lpFileTime)
{
   SYSTEMTIME systemTime;

   GetSystemTime(&systemTime);
   return (SystemTimeToFileTime(&systemTime, lpFileTime)); 
}


/*****************************************************************************
 * Timer implementation
 *****************************************************************************/


/**
 * Sleeps for a given amount of milliseconds.
 *
 * @param msTime Time in milliseconds
 * @remark The sleep time is only an approximate value and usually rounded
 * to the smallest base time resolution of the OS. It also depends on the
 * OS' task scheduling mechanism.
 */
void Timer::sleepMillis(unsigned long msTime)
{
   if (msTime == 0)
      return;

   // Handle this special case
   if (msTime == INFINITE)
   {
      msTime--;
      Sleep(1);
   }
   Sleep(msTime);
}


/**
 * Creates a Timer object and initialises internal data.
 */
Timer::Timer()
{
   (void) ver; // Avoid compiler warning
   startMillis(0);
}


/**
 * Creates a Timer object and starts the timer
 *
 * @param msTime Expiry time interval in milliseconds
 */
Timer::Timer(unsigned long msTime)
{
   startMillis(msTime);
}


/**
 * Tests if timer has expired.
 *
 * @return true if it is expired else false
 */
int32_t Timer::isExpired()
{
   FILETIME currentTime;

   getSystemTimeAsFileTime(&currentTime);
   return ((CompareFileTime(&currentTime, &expireTime) >= 0));
}


/**
 * Starts a timer. The timer will expire after a number of milliseconds has
 * elapsed.
 *
 * @param msTime Expiry time interval in milliseconds
 */
void Timer::startMillis(unsigned long msTime)
{
   getSystemTimeAsFileTime(&expireTime);
   expireLongLong += (LONGLONG) msTime * (LONGLONG) 10000;
}


/**
 * Queries how much longer the timer has to run for before expiry.
 *
 * @return Remaining time until expiry in system dependant format
 */
void Timer::remain(FILETIME *remainTimePtr)
{
   LONGLONG *remainLongLongPtr = (LONGLONG *) remainTimePtr;
   union
   {
      FILETIME currentTime;
      LONGLONG currentLongLong;
   };

   getSystemTimeAsFileTime(&currentTime);
   *remainLongLongPtr = expireLongLong - currentLongLong;
   if (*remainLongLongPtr < 0)
   {
      *remainLongLongPtr = 0;
   }
}


/**
 * Queries how much longer the timer has to run for before expiry.
 *
 * @return Remaining time until expiry in ms
 */
unsigned long Timer::left()
{
   union
   {
      FILETIME currentTime;
      LONGLONG currentLongLong;
   };
   LONGLONG leftLongLong;

   getSystemTimeAsFileTime(&currentTime);
   leftLongLong = expireLongLong - currentLongLong;
   if (leftLongLong > 0)
   {
      // Convert to millis
      leftLongLong = (leftLongLong + (LONGLONG) 5000) / (LONGLONG) 10000;
      if (leftLongLong > 0xFFFFFFFF)
         return (0xFFFFFFFF);
      return ((unsigned long) leftLongLong);
   }
   else
      return (0);
}


/**
 * Queries how much time has passed since the timer expired
 *
 * @return Time elapsed since expiry in ms
 */
unsigned long Timer::over()
{
   union
   {
      FILETIME currentTime;
      LONGLONG currentLongLong;
   };
   LONGLONG overLongLong;

   getSystemTimeAsFileTime(&currentTime);
   overLongLong = currentLongLong - expireLongLong;
   if (overLongLong > 0)
   {
      // Convert to millis
      overLongLong = (overLongLong + (LONGLONG) 5000) / (LONGLONG) 10000;
      if (overLongLong > 0xFFFFFFFF)
         return (0xFFFFFFFF);
      return ((unsigned long) overLongLong);
   }
   else
      return (0);
}


/**
 * Sleeps until the timer expires.
 */
void Timer::sleepUntilExpiry()
{
   union
   {
      FILETIME remainTime;
      LONGLONG remainLongLong;
   };

   remain(&remainTime);
   remainLongLong = (remainLongLong + (LONGLONG) 5000) / (LONGLONG) 10000;

   // Sleep in 1 hour perts, because the time might not fit into a long
   while (remainLongLong > 3600000)
   {
      Sleep(3600000);
      remainLongLong -= 3600000;
   }
   if (remainLongLong > 0)
      Sleep((DWORD) remainLongLong);
}


//@}


/*****************************************************************************
 * Test case
 *****************************************************************************/

#ifdef TEST_RUN

#include <stdio.h>

int32_t main()
{
   Timer timer1, timer2;

   printf("Test Run for hmtimer.cpp\n\n");

   printf("Sleeping for 0 ms... ");
   Timer::sleepMillis(0);
   printf("OK\n"); // If we arrive here we succeeded

   printf("Left test ... ");
   timer1.startMillis(0);
   if (timer1.left() != 0)
   {
      printf("FAILED!\n");
      return (1);
   }
   timer1.startMillis(1);
   if (timer1.left() != 1)
   {
      printf("FAILED!\n");
      return (1);
   }
   timer1.startMillis(100);
   if (timer1.left() != 100)
   {
      printf("FAILED!\n");
      return (1);
   }
   timer1.startMillis(10000000);
   if (timer1.left() != 10000000)
   {
      printf("FAILED!\n");
      return (1);
   }
   timer1.startMillis(0xFFFFFFFE);
   if (timer1.left() != 0xFFFFFFFE)
   {
      printf("FAILED!\n");
      return (1);
   }
   timer1.startMillis(0xFFFFFFFF);
   if (timer1.left() != 0xFFFFFFFF)
   {
      printf("FAILED!\n");
      return (1);
   }
   printf("OK\n");

   printf("Sleeping for 100 ms... ");
   timer1.startMillis(150);
   timer2.startMillis(50);
   Timer::sleepMillis(100);
   if (timer1.isExpired() || !timer2.isExpired())
   {
      printf("FAILED!\n");
      return (1);
   }
   else
   {
      printf("OK\n");
   }

   printf("Antitest 1 ... ");
   timer1.startMillis(190);
   Timer::sleepMillis(200);
   if (!timer1.isExpired())
   {
      printf("FAILED!\n");
      return (1);
   }
   else
   {
      printf("OK\n");
   }

   printf("Antitest 2 ... ");
   timer1.startMillis(210);
   Timer::sleepMillis(200);
   if (timer1.isExpired())
   {
      printf("FAILED!\n");
      return (1);
   }
   else
   {
      printf("OK\n");
   }

   printf("Sleeping for 1000 ms... ");
   timer1.startMillis(1100);
   timer2.startMillis(900);
   Timer::sleepMillis(1000);
   if (timer1.isExpired() || !timer2.isExpired())
   {
      printf("FAILED!\n");
      return (1);
   }
   else
   {
      printf("OK\n");
   }

   printf("Sleeping for 500 ms... \n");
   timer1.startMillis(1000);
   timer2.startMillis(1100);
   Timer::sleepMillis(500);
   printf("Sleeping until expiry (another 500 ms) ...");
   timer1.sleepUntilExpiry();
   if (!timer1.isExpired() || timer2.isExpired())
   {
      printf("FAILED!\n");
      return (1);
   }
   else
   {
      printf("OK\n");
   }

   printf("Sleeping for 10000 ms... ");
   timer1.startMillis(11000);
   timer2.startMillis(9000);
   Timer::sleepMillis(10000);
   if (timer1.isExpired() || !timer2.isExpired())
   {
      printf("FAILED!\n");
      return (1);
   }
   else
   {
      printf("OK\n");
   }

   printf("Sleeping for 10 ms... ");
   timer1.startMillis(15);
   timer2.startMillis(5);
   Timer::sleepMillis(10);
   if (timer1.isExpired() || !timer2.isExpired())
   {
      printf("FAILED!\n");
      return (1);
   }
   else
   {
      printf("OK\n");
   }

   return (0);
}


#endif
