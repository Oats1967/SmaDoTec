/**
 * @internal
 * @file hmtracelog.h
 *
 * Simple trace logging for debugging
 *
 * @if NOTICE
 *
 * $Id: hmtracelog.h,v 1.1 2003/05/20 08:04:56 henrik Exp $
 *
 * Copyright (c) 2003 FOCUS Software Engineering Pty Ltd, Australia.
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


#ifndef _HMTRACELOG_H_INCLUDED
#define _HMTRACELOG_H_INCLUDED


#ifdef TRACELOG
#  include <stdio.h>
#endif

#ifdef TRACELOG
#  define TRACELOG1(a)                printf("TRACELOG: " a)
#  define TRACEADD1(a)                printf(a)
#  define TRACELOG2(a,b)              printf("TRACELOG: " a, b)
#  define TRACEADD2(a,b)              printf(a, b)
#  define TRACELOG3(a,b,c)            printf("TRACELOG: " a, b, c)
#  define TRACELOG4(a,b,c,d)          printf("TRACELOG: " a, b, c, d)
#  define TRACELOG5(a,b,c,d,e)        printf("TRACELOG: " a, b, c, d, e)
#else
#  define TRACELOG1(a)                ((void) 0)
#  define TRACEADD1(a)                ((void) 0)
#  define TRACELOG2(a,b)              ((void) 0)
#  define TRACEADD2(a,b)              ((void) 0)
#  define TRACELOG3(a,b,c)            ((void) 0)
#  define TRACELOG4(a,b,c,d)          ((void) 0)
#  define TRACELOG5(a,b,c,d,e)        ((void) 0)
#endif


#endif /* ifdef ..._H_INCLUDED */

