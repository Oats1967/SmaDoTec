//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AdsProfiAdapter
///
/// @file   AdsProfiAdapter.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include "AdsProfiAdapter.h"
#include "BASE/Utils/public/xml/ProfibusDeviceXml.h"
#include "AdsHelper.h"
#include "Logger/include/Logger.h"

using namespace base;


//*****************************************************************************************************************************
//*****************************************************************************************************************************
CAdsProfiAdapter::~CAdsProfiAdapter()
{
	Close();
}
//*****************************************************************************************************************************
//*****************************************************************************************************************************
BOOL CAdsProfiAdapter::Open(const std::string& _path2Config)
{
	// Lese Portnummer ein
	Close();
	xml::CProfiBusDeviceXml xml;
	auto result = xml.Load(_path2Config);
	if (!result)
	{
		LOGERROR("Error loading " << _path2Config);
		return FALSE;
	}
	const auto& rList = xml.Get();
	c_PortAddress = _U16(rList.GetTaskId());
	const auto& cTotalMap = rList.GetList();
	for (const auto& item : cTotalMap)
	{
		char szName[256];
		strcpy_s(szName, item.m_TaskItem.c_str());
		uint32_t hdl = 0;
		auto iResult = AdsHelperGetHandle(c_PortAddress, szName, hdl);
		if (iResult == ADS_OK)
		{
			if (item.m_Direction == RemoteDirection::Write)
			{
				m_WriteMap.insert({ item.m_Category, hdl });
			}
			else
			{
				m_ReadMap.insert({ item.m_Category, hdl });
			}
		}
		else
		{
			LOGDEBUG("Not found" << szName);
		}
	}
	return TRUE;
}
//*****************************************************************************************************************************
void CAdsProfiAdapter::ReleaseHandle(std::map<base::RemoteCategory, uint32_t>& rmap)
{
	for (auto& item : rmap)
	{
		AdsHelperReleaseHandle(c_PortAddress, item.second);
	}
	rmap.clear();
}
//*****************************************************************************************************************************
//*****************************************************************************************************************************
BOOL CAdsProfiAdapter::Close()
{
	if (c_PortAddress)
	{
		ReleaseHandle(m_WriteMap);
		ReleaseHandle(m_ReadMap);
		c_PortAddress = 0;
	}
	return TRUE;
}
//*****************************************************************************************************************************
//*****************************************************************************************************************************
int32_t CAdsProfiAdapter::WriteData(int32_t DBnum, int32_t , int32_t length, const void* buffer)
{
	int32_t result = -1;
	const auto it = m_WriteMap.find(static_cast<base::RemoteCategory>(DBnum));
	if (it != m_WriteMap.cend())
	{
		result = AdsHelperStreamOutput(c_PortAddress, it->second, (const char_t*)buffer, length);
	}
	return result;
}
//*****************************************************************************************************************************
//*****************************************************************************************************************************
int32_t CAdsProfiAdapter::ReadData(int32_t DBnum, int32_t , int32_t length, void* buffer)
{
	int32_t result = -1;
	uint32_t len = _U32(length);
	const auto it = m_ReadMap.find(static_cast<base::RemoteCategory>(DBnum));
	if (it != m_ReadMap.cend())
	{
		result = AdsHelperStreamInput(c_PortAddress, it->second, (char_t*)buffer, len);
	}
	return result;
}
