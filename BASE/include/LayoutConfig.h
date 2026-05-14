//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LayoutConfig
///
/// @file   LayoutConfig.h
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

#include "BASE/Base.h"
#include "BASE/include/GPoint.h"
#include "BASE/include/GRect.h"



namespace base
{
struct CCustomerLogoConfig
{
    std::string m_CustomerLogoPNG;
    IPoint      m_Location;
    BOOL m_bInit = FALSE;

    BOOL operator == (const CCustomerLogoConfig& cfg) const
    {
        return (m_CustomerLogoPNG == cfg.m_CustomerLogoPNG) &&
            (m_Location == cfg.m_Location);
    }
    BOOL operator != (const CCustomerLogoConfig& cfg) const
    {  return !(*this == cfg);   }

    CCustomerLogoConfig() : m_CustomerLogoPNG()
        , m_Location()
        , m_bInit{ FALSE }
    {}
};


class CLayoutConfig
{
    CCustomerLogoConfig  m_CustomerLogo;
    base::IPoint         m_PointLogo;
    base::IPoint         m_PointMessageBox;
    base::IPoint         m_PointLineBox;
    base::IPoint         m_PointServiceBox;
    base::IPoint         m_PointTotalizerBox;
    base::IRect          m_RectBarChart;
    BOOL                 m_LayoutRecipe;
    BOOL                 m_LCAuthorize;
    BOOL                 m_ProductNameEnable;
    BOOL                 m_RefillButtonEnable;
    BOOL                 m_bModified;

public:
    CLayoutConfig() :
        m_CustomerLogo()
        , m_PointLogo()
        , m_PointMessageBox()
        , m_PointLineBox()
        , m_PointServiceBox()
        , m_PointTotalizerBox()
        , m_RectBarChart()
        , m_LayoutRecipe{ TRUE }
        , m_LCAuthorize{ FALSE }
        , m_ProductNameEnable{ FALSE }
        , m_RefillButtonEnable{ FALSE }
        , m_bModified { TRUE }
    {}

    void SetModified(const BOOL _modified)
    {  m_bModified = _modified;   }

    BOOL IsModified(void) const
    {return  m_bModified;   }

    BOOL operator == (const CLayoutConfig&) const;
    BOOL operator != (const CLayoutConfig& rCfg) const
    { return ! operator==(rCfg);    }


    SETGETMODIFIED(const BOOL, LayoutRecipe)
    SETGETMODIFIED(const BOOL, LCAuthorize)
    SETGETMODIFIED(const CCustomerLogoConfig&, CustomerLogo)
    SETGETMODIFIED(const base::IPoint&, PointLogo)
    SETGETMODIFIED(const base::IPoint&, PointMessageBox)
    SETGETMODIFIED(const base::IPoint&, PointLineBox)
    SETGETMODIFIED(const base::IPoint&, PointServiceBox)
    SETGETMODIFIED(const base::IPoint&, PointTotalizerBox)
    SETGETMODIFIED(const base::IRect&,  RectBarChart)
    SETGETMODIFIED(const BOOL, ProductNameEnable)
    SETGETMODIFIED(const BOOL, RefillButtonEnable)
};

//*********************************************************************************************************************
//*********************************************************************************************************************
inline BOOL CLayoutConfig::operator == (const CLayoutConfig& cfg) const
{
    BOOL bEqual  = TRUE;
    bEqual      &= (m_PointLogo == cfg.m_PointLogo);
    bEqual      &= (m_PointMessageBox == cfg.m_PointMessageBox);
    bEqual      &= (m_PointLineBox == cfg.m_PointLineBox);
    bEqual      &= (m_PointServiceBox == cfg.m_PointServiceBox);
    bEqual      &= (m_PointTotalizerBox == cfg.m_PointTotalizerBox);
    bEqual      &= (m_RectBarChart == cfg.m_RectBarChart);
    bEqual      &= (m_LayoutRecipe == cfg.m_LayoutRecipe);
    bEqual      &= (m_LCAuthorize == cfg.m_LCAuthorize);
    bEqual      &= (m_ProductNameEnable == cfg.m_ProductNameEnable);
    bEqual      &= (m_RefillButtonEnable == cfg.m_RefillButtonEnable);
    return bEqual;
}


};

