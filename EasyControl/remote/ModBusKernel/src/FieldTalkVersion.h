//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of FieldTalkVersion.h
///
/// @file   FieldTalkVersion.h
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

/* Version identifiers */
static char_t banner[] = "" /* No UNICODE here */
#ifdef EVAL_VERSION
                       "FieldTalk(tm) EVALUATION VERSION\n"
#else
                       "FieldTalk(tm)\n"
#endif
                       "Copyright (c) 2002-2006 FOCUS Software Engineering Pty"
                       " Ltd, Australia.\nAll rights reserved. "
                       "<www.focus-sw.com>";
#ifdef PACKAGE_VERSION
#if defined(_UNICODE)
#define PACKAGE_VERSION_STR2(arg) L#arg
#else
#define PACKAGE_VERSION_STR2(arg) #arg
#endif
#define PACKAGE_VERSION_STR(arg) PACKAGE_VERSION_STR2(arg)
static TCHAR packageVersion[] = PACKAGE_VERSION_STR(PACKAGE_VERSION); ///< Package version
#else
#if defined(_UNICODE)
static TCHAR packageVersion[] = L"2.4.0"; ///< Package version
#else
static TCHAR packageVersion[] = "2.4.0"; ///< Package version
#endif
#endif

