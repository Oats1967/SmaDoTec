//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsTypen.h
///
/// @file   AdsTypen.h
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

#include "BASE/Base.h"

#define MAXDOSEPERCLAMP 8
#define ADS_LC_MAXCLAMP     (GLOBALDOSEMAXCOUNT / MAXDOSEPERCLAMP)
#define ADS_LC_SLAVECOUNT   (GLOBALDOSEMAXCOUNT + ADS_LC_MAXCLAMP)


#define ADS_OK						 0
#define ADS_MPM_OK					 0
#define ADS_PROFIBUS_OK				 0
#define ADS_PROFIBUS_ACK		     0
#define ADS_MPM_ACK					 0
#define ADS_MPM_NAK					-1
#define ADS_MPM_NOK					-1
#define ADS_MPM_ERRORECXECUTE		-2
#define ADS_MPM_NOTOPEN				-3
#define ADS_MPM_ETB					-4
#define ADS_MPM_TIMEOUT				-5
#define ADS_MPM_INVALID				-6
#define ADS_MPM_PENDENT				-7
#define ADS_MPM_ZERO				-8
#define ADS_MPM_TARA				-9
#define ADS_MPM_INVALIDUNIT			-10
#define ADS_MPM_INVALIDNUMBER	    -11
#define ADS_MPM_INVALIDCODELENGTH   -12
#define ADS_MPM_INVALIDCODE         -13
#define ADS_MPM_SYNCERROR           -14
#define ADS_MPM_OVERFLOW            -15
#define ADS_MPM_INVALIDBUSADDRESS	-16
#define ADS_PROFIBUS_NAK	    -17
#define ADS_PROFIBUS_NOK				    -18
#define ADS_PROFIBUS_ERRORECXECUTE	    -19
#define ADS_PROFIBUS_NOTOPEN			    -20
#define ADS_PROFIBUS_ETB				    -21
#define ADS_PROFIBUS_TIMEOUT			    -22
#define ADS_PROFIBUS_INVALID			    -23
#define ADS_PROFIBUS_PENDENT			    -24
#define ADS_PROFIBUS_ZERO			    -25
#define ADS_PROFIBUS_TARA			    -26
#define ADS_PROFIBUS_INVALIDUNIT		   -27
#define ADS_PROFIBUS_INVALIDNUMBER	   -28
#define ADS_PROFIBUS_INVALIDCODELENGTH  -29
#define ADS_PROFIBUS_INVALIDCODE        -30
#define ADS_PROFIBUS_SYNCERROR          -31
#define ADS_REGISTER_ERROR          -32
#define ADS_UNREGISTER_ERROR          -33



#define ADS_NOTOPEN					ADS_MPM_NOTOPEN
#define ADS_SYNCERROR				ADS_MPM_SYNCERROR
#define ADS_ERROR					ADS_MPM_NAK
#define ADS_COM_ERROR				ADS_MPM_NAK


