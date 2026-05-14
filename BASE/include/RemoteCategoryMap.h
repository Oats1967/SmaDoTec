//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteCategoryMap
///
/// @file   RemoteCategoryMap.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <cassert>
#include "XMLStringEnumMap.h"
#include "BASE/include/GenericRemoteConfig.h"


#define CATMAP_QUOTE(name) #name
#define CATMAP_STR(macro) CATMAP_QUOTE(macro)
#define CATMAP_FILL(_a) {  RemoteCategory::_a , std::string(CATMAP_STR(_a)) }

namespace base
{
    using TRemoteCategoryMap = CXMLStringEnumMap< RemoteCategory>;
    class CRemoteCategoryMap : public TRemoteCategoryMap
    {
        public:
            CRemoteCategoryMap() : TRemoteCategoryMap
            (
                {
                        CATMAP_FILL(W_FEEDERTYPE),
                        CATMAP_FILL(W_SETPOINT),
                        CATMAP_FILL(W_FEEDERTYPE),
                        CATMAP_FILL(W_SETPOINT),
                        CATMAP_FILL(W_MASSFLOW),
                        CATMAP_FILL(W_NETWEIGHT),
                        CATMAP_FILL(W_TOTALIZER),
                        CATMAP_FILL(W_RECIPE_PERCENTAGE),
                        CATMAP_FILL(W_DEVIATION),
                        CATMAP_FILL(W_ENCODERROTATION),
                        CATMAP_FILL(W_NOMINALENCODERROTATION),
                        CATMAP_FILL(W_ACTUALDOSEPERFORMANCE),
                        CATMAP_FILL(W_DRIVECOMMAND),
                        CATMAP_FILL(W_DOSEPERFORMANCE),
                        CATMAP_FILL(W_TARAWEIGHT),
                        CATMAP_FILL(W_EMPTYFEEDERSPEED),
                        CATMAP_FILL(W_REFILLTIME),
                        CATMAP_FILL(W_REFILLLIMITMIN),
                        CATMAP_FILL(W_REFILLLIMITMAX),
                        CATMAP_FILL(W_REFILLLIMITALARM),
                        CATMAP_FILL(W_LCCORRECTIONFACTOR),
                        CATMAP_FILL(W_CALIB_COUNT),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND1),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE1),
                        CATMAP_FILL(W_CALIB_DURATION1),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND2),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE2),
                        CATMAP_FILL(W_CALIB_DURATION2),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND3),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE3),
                        CATMAP_FILL(W_CALIB_DURATION3),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND4),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE4),
                        CATMAP_FILL(W_CALIB_DURATION4),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND5),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE5),
                        CATMAP_FILL(W_CALIB_DURATION5),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND6),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE6),
                        CATMAP_FILL(W_CALIB_DURATION6),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND7),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE7),
                        CATMAP_FILL(W_CALIB_DURATION7),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND8),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE8),
                        CATMAP_FILL(W_CALIB_DURATION8),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND9),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE9),
                        CATMAP_FILL(W_CALIB_DURATION9),
                        CATMAP_FILL(W_CALIB_DRIVECOMMAND10),
                        CATMAP_FILL(W_CALIB_DOSEPERFORMANCE10),
                        CATMAP_FILL(W_CALIB_DURATION10),
                        CATMAP_FILL(W_SAMPLEINTERVAL),
                        CATMAP_FILL(W_PADDLESPEED),
                        CATMAP_FILL(W_REFILLFEEDERSPEED),
                        CATMAP_FILL(W_PIDGAIN),
                        CATMAP_FILL(W_MASSFLOWFILTER),
                        CATMAP_FILL(W_GATEFILTER),
                        CATMAP_FILL(W_ALARMNOISELIMIT),
                        CATMAP_FILL(W_ALARMSTATUS),
                        CATMAP_FILL(W_PROCESSSTATUS),
                        CATMAP_FILL(W_COUNTER),
                        CATMAP_FILL(W_LINESETPOINT),
                        CATMAP_FILL(W_LINEALARMSTATUS),
                        CATMAP_FILL(W_LINEPROCESSSTATUS),

                        CATMAP_FILL(R_CONTROL),
                        CATMAP_FILL(R_DOSECMDSTART),
                        CATMAP_FILL(R_DOSECMDSTOP),
                        CATMAP_FILL(R_DOSECMDTOTCLEAR),
                        CATMAP_FILL(R_DOSECMDALARMCLEAR),
                        CATMAP_FILL(R_DOSECMDALARMACK),
                        CATMAP_FILL(R_DOSECMDCALIBRATE),
                        CATMAP_FILL(R_DOSECMDTARE),
                        CATMAP_FILL(R_DOSECMDREFILLOFF),
                        CATMAP_FILL(R_DOSECMDREFILLON),
                        CATMAP_FILL(R_DOSECMDINLINE),
                        CATMAP_FILL(R_DOSECMDLOCAL),
                        CATMAP_FILL(R_DOSECMDGRAV),
                        CATMAP_FILL(R_DOSECMDVOL),
                        CATMAP_FILL(R_DOSECMDREFILLRELEASE),

                        CATMAP_FILL(R_SETPOINT),
                        CATMAP_FILL(R_TARAWEIGHT),
                        CATMAP_FILL(R_EMPTYFEEDERSPEED),
                        CATMAP_FILL(R_RECIPE_PERCENTAGE),
                        CATMAP_FILL(R_DOSEPERFORMANCE),
                        CATMAP_FILL(R_ALARMMASSFLOWHIGH),
                        CATMAP_FILL(R_ALARMMASSFLOWLOW),
                        CATMAP_FILL(R_ALARMDRIVECOMMANDHIGH),
                        CATMAP_FILL(R_ALARMDRIVECOMMANDLOW),
                        CATMAP_FILL(R_REFILLTIME),
                        CATMAP_FILL(R_REFILLLIMITMIN),
                        CATMAP_FILL(R_REFILLLIMITMAX),
                        CATMAP_FILL(R_REFILLLIMITALARM),
                        CATMAP_FILL(R_LCCORRECTIONFACTOR),
                        CATMAP_FILL(R_GATEFILTER),
                        CATMAP_FILL(R_CALIB_COUNT),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND1),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE1),
                        CATMAP_FILL(R_CALIB_DURATION1),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND2),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE2),
                        CATMAP_FILL(R_CALIB_DURATION2),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND3),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE3),
                        CATMAP_FILL(R_CALIB_DURATION3),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND4),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE4),
                        CATMAP_FILL(R_CALIB_DURATION4),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND5),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE5),
                        CATMAP_FILL(R_CALIB_DURATION5),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND6),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE6),
                        CATMAP_FILL(R_CALIB_DURATION6),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND7),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE7),
                        CATMAP_FILL(R_CALIB_DURATION7),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND8),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE8),
                        CATMAP_FILL(R_CALIB_DURATION8),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND9),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE9),
                        CATMAP_FILL(R_CALIB_DURATION9),
                        CATMAP_FILL(R_CALIB_DRIVECOMMAND10),
                        CATMAP_FILL(R_CALIB_DOSEPERFORMANCE10),
                        CATMAP_FILL(R_CALIB_DURATION10),

                        CATMAP_FILL(R_ALARMREACTIONDELAY),
                        CATMAP_FILL(R_ALARMSTARTREACTIONDELAY),
                        CATMAP_FILL(R_STARTUPDELAY),
                        CATMAP_FILL(R_SAMPLEINTERVAL),
                        CATMAP_FILL(R_PADDLESPEED),
                        CATMAP_FILL(R_REFILLFEEDERSPEED),
                        CATMAP_FILL(R_PIDGAIN),
                        CATMAP_FILL(R_MASSFLOWFILTER),
                        CATMAP_FILL(R_ALARMNOISELIMIT),
                        CATMAP_FILL(R_LINECONTROL),
                        CATMAP_FILL(R_LINESETPOINT),
                        CATMAP_FILL(R_LINEPERCENTAGE),
                        CATMAP_FILL(R_LINECMDSTART),
                        CATMAP_FILL(R_LINECMDSTOP),
                        CATMAP_FILL(R_LINECMDTOTCLEAR),
                        CATMAP_FILL(R_MASK),
                        CATMAP_FILL(C_PLACEHOLDER)
                }
            )
            {
                assert(_S32(m_Map.size()) == _S32(RemoteCategory::S7_MAXENTRIES));
            }
    };
};