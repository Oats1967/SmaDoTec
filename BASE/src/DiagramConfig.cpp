//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DiagramConfig
///
/// @file   DiagramConfig.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <ctime>
#include <cassert>
#include "BASE/include/DiagramConfig.h"


namespace base
{
//****************************************************************************************************
//****************************************************************************************************
uint32_t CDiagramConfig::GetSelectionCount() const
{
    auto flags = m_DiagramSelect.allflags;
    uint32_t count = 0;
    while (flags)
    {
        count += (flags & 1U);
        flags >>= 1U;
    }
    return count;
}
//****************************************************************************************************
//****************************************************************************************************
tViewMultipleSelect CDiagramConfig::ExtractSelection() const
{
    auto flags = m_DiagramSelect.flags;

    tViewMultipleSelect _viewselect;
    if (flags.bitVIEWDOSEPERFORMANCE)
    {
        _viewselect.push_back(base::eMassflowSelect::eVIEWDOSEPERFORMANCE);
    }
    if (flags.bitVIEWDRIVECOMMAND)
    {
        _viewselect.push_back(base::eMassflowSelect::eVIEWDRIVECOMMAND);
    }
    if (flags.bitVIEWMASSFLOW)
    {
        _viewselect.push_back(base::eMassflowSelect::eVIEWMASSFLOW);
    }
    if (flags.bitVIEWROTSPEED)
    {
        _viewselect.push_back(base::eMassflowSelect::eVIEWROTSPEED);
    }
    if (flags.bitVIEWSETPOINT)
    {
        _viewselect.push_back(base::eMassflowSelect::eVIEWSETPOINT);
    }
    if (flags.bitVIEWTOTALIZER)
    {
        _viewselect.push_back(base::eMassflowSelect::eVIEWTOTALIZER);
    }
    if (flags.bitVIEWWEIGHT)
    {
        _viewselect.push_back(base::eMassflowSelect::eVIEWWEIGHT);
    }
    return std::move(_viewselect);
}
//***********************************************************************************************
//***********************************************************************************************
base::eMassflowSelect CDiagramConfig::GetSingleSelection() const
{
    const auto& flags = m_DiagramSelect.flags;

    base::eMassflowSelect eSelect = base::eMassflowSelect::eVIEWMAX;
    if (flags.bitVIEWDOSEPERFORMANCE)
        eSelect = base::eMassflowSelect::eVIEWDOSEPERFORMANCE;
    else if (flags.bitVIEWDRIVECOMMAND)
        eSelect = base::eMassflowSelect::eVIEWDRIVECOMMAND;
    else if (flags.bitVIEWMASSFLOW)
        eSelect = base::eMassflowSelect::eVIEWMASSFLOW;
    else if (flags.bitVIEWROTSPEED)
        eSelect = base::eMassflowSelect::eVIEWROTSPEED;
    else if (flags.bitVIEWSETPOINT)
        eSelect = base::eMassflowSelect::eVIEWSETPOINT;
    else if (flags.bitVIEWTOTALIZER)
        eSelect = base::eMassflowSelect::eVIEWTOTALIZER;
    else if (flags.bitVIEWWEIGHT)
        eSelect = base::eMassflowSelect::eVIEWWEIGHT;
    else
    {
        assert(FALSE);
    }
    return eSelect;
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramConfig::SetSingleSelection(const base::eMassflowSelect iSelect)
{
    m_DiagramSelect.allflags = 0;
    auto& flags = m_DiagramSelect.flags;

    switch (iSelect)
    {
    case base::eMassflowSelect::eVIEWDOSEPERFORMANCE:
        flags.bitVIEWDOSEPERFORMANCE = true;
        break;
    case  base::eMassflowSelect::eVIEWDRIVECOMMAND:
        flags.bitVIEWDRIVECOMMAND = true;
        break;
    case base::eMassflowSelect::eVIEWMASSFLOW:
        flags.bitVIEWMASSFLOW = true;
        break;
    case  base::eMassflowSelect::eVIEWROTSPEED:
        flags.bitVIEWROTSPEED = true;
        break;
    case base::eMassflowSelect::eVIEWSETPOINT:
        flags.bitVIEWSETPOINT = true;
        break;
    case base::eMassflowSelect::eVIEWTOTALIZER:
        flags.bitVIEWTOTALIZER = true;
        break;
    case base::eMassflowSelect::eVIEWWEIGHT:
        flags.bitVIEWWEIGHT = true;
        break;
    default:
        assert(FALSE);
        break;
    }
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramConfig::ClearSelection()
{
    m_DiagramSelect.allflags = 0;
}
//***********************************************************************************************
//***********************************************************************************************
base::eSelectionType CDiagramConfig::GetSelectionType() const
{
    const uint32_t count = GetSelectionCount();
    return (count > 1U) ? base::eSelectionType::eSelectionMultiple : ((count > 0U) ? base::eSelectionType::eSelectionSingle : base::eSelectionType::eSelectionUnknown);
}

};


