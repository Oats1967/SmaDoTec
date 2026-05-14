//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AdsDoseImpl.h
///
/// @file   AdsDoseImpl.h
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

#include "AdsClient/src/private/AdsDigitalInput.h"
#include "AdsClient/src/private/AdsDigitalOutput.h"
#include "AdsClient/src/private/AdsAnalogInput.h"
#include "AdsClient/src/private/AdsAnalogOutput.h"
#include "Logger/include/Logger.h"


class CAdsControlImpl
{
protected:
	CAdsDigitalInput	m_DigitalInput;
	CAdsDigitalOutput	m_DigitalOutput;
	CAdsAnalogInput		m_AnalogInput;
	CAdsAnalogOutput	m_AnalogOutput;

protected:
	virtual uint32_t  AddDigitalInput(LPSTR szSymbol, const uint16_t portAddress, CAdsDigitalInput::TCallback callback = nullptr)
	{
		uint32_t hdl = 0;
		auto result = AdsHelperGetHandle(portAddress, szSymbol, hdl);
		LOGDEBUG("szSymbol " << szSymbol << " Handle = " << hdl);

		if (result == ADS_OK)
		{
			m_DigitalInput.registerHandle(portAddress, hdl, callback);
		}
		return hdl;
	}

	virtual uint32_t AddDigitalOutput(LPSTR szSymbol, const uint16_t portAddress)
	{
		uint32_t hdl = 0;
		auto result = AdsHelperGetHandle(portAddress, szSymbol, hdl);
		if (result == ADS_OK)
		{
			m_DigitalOutput.registerHandle(portAddress, hdl );
		}
		return hdl;
	}
	virtual uint32_t AddAnalogInput(LPSTR szSymbol, const uint16_t portAddress)
	{
		uint32_t hdl = 0;
		auto result = AdsHelperGetHandle(portAddress, szSymbol, hdl);
		if (result == ADS_OK)
		{
			m_AnalogInput.registerHandle(portAddress, hdl );
		}
		return hdl;
	}

	virtual uint32_t AddAnalogOutput(LPSTR szSymbol, const uint16_t portAddress)
	{
		uint32_t hdl = 0;
		auto result = AdsHelperGetHandle(portAddress, szSymbol, hdl);
		if (result == ADS_OK)
		{
			m_AnalogOutput.registerHandle(portAddress, hdl );
		}
		return hdl;
	}

	virtual void Close()
	{
		m_DigitalInput.clear();
		m_DigitalOutput.clear();
		m_AnalogInput.clear();
		m_AnalogOutput.clear();
	}
public:
	CAdsControlImpl()
	{}

	virtual ~CAdsControlImpl()
	{ Close(); }
};

