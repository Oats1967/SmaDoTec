/**
 * @internal
 * @file hmcrc16.h
 *
 * 16-bit CRC calculation routine header file
 *
 * @if NOTICE
 *
 * $Id: hmcrc16.h,v 1.6 2003/06/16 14:10:10 henrik Exp $
 *
 * Copyright (c) 2002-2003 FOCUS Software Engineering, Australia.
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


#ifndef _HMCRC16_H_INCLUDED
#define _HMCRC16_H_INCLUDED


/* Platform header */
#include <stdlib.h>


/*****************************************************************************
 * Function prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


unsigned short calcCrc16(const char byteArr[], size_t byteCnt,
                         unsigned char startVal);

unsigned short addCrc16(char byteVal, unsigned short oldCrc);


#ifdef __cplusplus
}
#endif


#endif /* ifdef ..._H_INCLUDED */
