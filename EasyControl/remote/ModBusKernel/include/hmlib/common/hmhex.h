/**
 * @internal
 * @file hmhex.h
 *
 * Hex conversion utilities header file
 *
 * @if NOTICE
 *
 * $Id: hmhex.h,v 1.3 2002/12/09 04:31:32 henrik Exp $
 *
 * Copyright (c) 2002 FOCUS Software Engineering, Australia.
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


#ifndef _HMHEX_H_INCLUDED
#define _HMHEX_H_INCLUDED


/*****************************************************************************
 * Function prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void strHexValPrint (char *buffer, unsigned long val, int type);

char loNibbletoHex(char val);

char hiNibbletoHex(char val);

int hexToNibble(char nibble);

int hexToByte(char hiNibble, char loNibble);


#ifdef __cplusplus
}
#endif


#endif /* ifdef ..._H_INCLUDED */
