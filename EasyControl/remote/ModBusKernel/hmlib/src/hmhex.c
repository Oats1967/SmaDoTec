/**
 * @internal
 * @file hmhex.c
 *
 * Hex conversion utilities
 *
 * @if NOTICE
 *
 * $Id: hmhex.c,v 1.6 2003/09/21 02:39:06 henrik Exp $
 *
 * Copyright (c) 2002-2003 FOCUS Software Engineering Pty Ltd, Australia.
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


/* HM C/C++ library header */
#include "hmlib/include/hmhex.h"


/*****************************************************************************
 * Hex routines implementation
 *****************************************************************************/

/**
 * Prints a byte/word/dword into a string buffer.
 *
 * @param buffer points to char string where result is to be printed.
 *        Must be of size type + 1!
 * @param val Hex value to be printed
 * @param type 1=byte, 2=16bit word, 4=32bit int
 */
void strHexValPrint (char *buffer, unsigned long val, int type)
{
   unsigned long tempVal;

   if (!buffer)
      return;
   if ((type != 1) && (type != 2) && (type != 4))
      return;
   type *= 2;
   buffer[type] = 0;
   do
   {
      type--;
      tempVal = (val & 0x0F);
      if (tempVal <= 9)
         buffer[type] = (char) (tempVal + '0');
      else
         buffer[type] = (char) (tempVal + 'A' - 10);
      val = val >> 4;
   }
   while (type > 0);
}


/**
 * Encodes the low nibble of a byte into a hexadecimal character.
 *
 * @param val Byte value
 * @return Hexadecimal character
 */
char loNibbletoHex(char val)
{
   char tempVal;

   tempVal = (char) (val & (char) 0x0F);
   if (tempVal <= 9)
      return ((char) (tempVal + '0'));
   else
      return ((char) (tempVal + 'A' - 10));
}


/**
 * Encodes the high nibble of a char into a hexadecimal character.
 *
 * @param val Byte value
 * @return Hexadecimal character
 */
char hiNibbletoHex(char val)
{
   char tempVal;

   val = (char) (val >> 4);
   tempVal = (char) (val & (char) 0x0F);
   if (tempVal <= 9)
      return ((char) (tempVal + '0'));
   else
      return ((char) (tempVal + 'A' - 10));
}


/**
 * Decodes a hexadecimal character into a binary
 * value. Valid characters are '0'-'9' and 'A'-'F'.
 *
 * @param nibble Character with nibble
 * @return Binary value or -1 if a parameter is not a valid hexadecimal character
 */
int hexToNibble(char nibble)
{
   if ((nibble >= '0') && (nibble <= '9'))
      nibble -= (char) '0';
   else
   {
      if ((nibble >= 'A') && (nibble <= 'F'))
         nibble = (char) (nibble - ('A' - 10));
      else
         return (-1); /* Invalid hexadecimal character */
   }
   return (nibble);
}


/**
 * Decodes a high/low nibble pair of hexadecimal characters into a
 * binary value. Valid characters are '0'-'9' and 'A'-'F'.
 *
 * @param hiNibble Character with high nibble of byte
 * @param loNibble Character with low nibble of byte
 * @return Binary value or -1 if a parameter is not a valid hexadecimal
 * character
 */
int hexToByte(char hiNibble, char loNibble)
{

   int hi, lo;

   hi = hexToNibble(hiNibble);
   lo = hexToNibble(loNibble);
   if ((hi == -1) || (lo == -1))
      return (-1);
   return ((hi << 4) | lo);
}

/*@}*/


/*****************************************************************************
 * Test case
 *****************************************************************************/

#ifdef TEST_RUN

#include <stdio.h>

void expect(int cond)
{
   if (!cond)
   {
      printf("FAILED!\n");
      exit (1);
   }
}


int main()
{
   printf("Test Run for hmhex.c\n\n");

   expect(hexToNibble('1') == 1);
   expect(hexToNibble('2') == 2);
   expect(hexToNibble('3') == 3);
   expect(hexToNibble('4') == 4);
   expect(hexToNibble('5') == 5);
   expect(hexToNibble('6') == 6);
   expect(hexToNibble('7') == 7);
   expect(hexToNibble('8') == 8);
   expect(hexToNibble('9') == 9);
   expect(hexToNibble('9') == 9);
   expect(hexToNibble('0') == 0);
   expect(hexToNibble('A') == 0xA);
   expect(hexToNibble('B') == 0xB);
   expect(hexToNibble('C') == 0xC);
   expect(hexToNibble('D') == 0xD);
   expect(hexToNibble('E') == 0xE);
   expect(hexToNibble('F') == 0xF);
   expect(hexToNibble('a') == -1);
   expect(hexToNibble('b') == -1);
   expect(hexToNibble('c') == -1);
   expect(hexToNibble('e') == -1);
   expect(hexToNibble('e') == -1);
   expect(hexToNibble('f') == -1);
   expect(hexToNibble(' ') == -1);
   expect(hexToNibble(':') == -1);
   expect(hexToNibble('X') == -1);
   expect(hexToNibble('x') == -1);
   expect(hexToNibble(0xa) == -1);
   expect(hexToNibble(0xd) == -1);

   expect(hexToByte('1', '2') == 0x12);
   expect(hexToByte('A', 'B') == 0xAB);
   expect(hexToByte('0', '0') == 0x00);
   expect(hexToByte('F', 'F') == 0xFF);
   expect(hexToByte('F', '0') == 0xF0);
   expect(hexToByte('0', 'F') == 0x0F);
   expect(hexToByte(0xF, 0xF) == -1);
   expect(hexToByte(0, 0) == -1);
   expect(hexToByte(-1, -1) == -1);

   printf("OK\n");

   return (0);
}


#endif

