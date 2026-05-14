//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeItemList
///
/// @file   RecipeItemList.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <math.h>
#include <array>
#include <sstream>
#include <algorithm>
#include "BASE/Utils/public/RecipeItemList.h"
#include "BASE/Utils/public/Memory.h"


namespace base
{
namespace utils
{
constexpr char_t DELIMITERCOMMA		= ',';
constexpr char_t DELIMITERSEMICOLON	= ';';
constexpr char_t EOL				= '\n';


//**********************************************************************************************
//**********************************************************************************************
CRecipeItemList::CRecipeItemList(int32_t lID) : CGenericList < CRecipeItem >(lID)
, m_Name("Linierezept")
, m_QMNumber(1234567U)
, m_RecipeSetpoint(0.0f)
, m_RecipePercentage(0.0f)
, m_LineSetpoint(0.0f)
, m_RegeneratPercentage(0.0f)
, m_RegeneratSetpoint(0.0f)
, m_LokalSetpoint(0.0f)
{
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecipeItemList::LoadStream(std::ifstream& file_in)
{
	BOOL result = FALSE;
	constexpr int32_t cDefaultlength = 256U;
	constexpr int32_t arraysize = 7U;

	std::array<std::string, arraysize> aArray;

	std::string aStr(cDefaultlength, 0);
	std::string sz;
	std::getline(file_in, sz);
	std::stringstream ainput(sz);

	int32_t acount = 0;
	while ((acount < _S32(aArray.size())) && std::getline(ainput, aStr, DELIMITERSEMICOLON))
	{
		aArray[acount++] = aStr;
	}
	if (acount >= arraysize)
	{
		m_Name = aArray[0];
		m_QMNumber				= ustoull(aArray[1]);
		m_RecipeSetpoint		= ustof(aArray[2]);
		m_RecipePercentage		= ustof(aArray[3]);
		m_LineSetpoint			= ustof(aArray[4]);
		m_RegeneratPercentage	= ustof(aArray[5]);
		m_LokalSetpoint			= ustof(aArray[6]);
		result = CGenericList::LoadStream(file_in);
	}
	return result;
}
//**********************************************************************************************
//**********************************************************************************************
std::string CRecipeItemList::MakeString(void) const
{
	std::stringstream stringStream;

	stringStream << m_Name << DELIMITERSEMICOLON;
	stringStream << m_QMNumber << DELIMITERSEMICOLON;
	stringStream << m_RecipeSetpoint << DELIMITERSEMICOLON;
	stringStream << m_RecipePercentage << DELIMITERSEMICOLON;
	stringStream << m_LineSetpoint << DELIMITERSEMICOLON;
	stringStream << m_RegeneratPercentage << DELIMITERSEMICOLON;
	stringStream << m_LokalSetpoint << DELIMITERSEMICOLON;
	stringStream << EOL;
	return stringStream.str();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecipeItemList::SaveStream(std::ofstream& file_out)
{
	file_out << MakeString();
	return CGenericList::SaveStream(file_out);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
float32_t CRecipeItemList::SummeRegeneratSetpoint(void) const
{
	float32_t fSumme = 0.0f;

	auto update = [&fSumme](const auto& item) 
	{ 
		if ((!item.GetLocalMode()) && (item.GetRegenerat())) 
		{ 
			fSumme += item.GetSetpoint(); 
		} 
	};

	std::for_each(m_ItemList.cbegin(), m_ItemList.cend(), update);
	return fSumme;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
float32_t CRecipeItemList::SumRecipeSetpoint(void) const
{
	float32_t fSumme = 0.0f;

	auto update = [&fSumme](const auto& item)
	{
		if (( ! item.GetLocalMode()) && ( ! item.GetRegenerat()))
		{
			fSumme += item.GetSetpoint();
		}
	};

	std::for_each(m_ItemList.cbegin(), m_ItemList.cend(), update);
	return fSumme;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
float32_t CRecipeItemList::SumLocalModeSetpoint(void) const
{
	float32_t fSumme = 0.0f;

	auto update = [&fSumme](const auto& item) 
	{ 
		if ( item.GetLocalMode() )  
		{ 
			fSumme += item.GetSetpoint(); 
		} 
	};
	std::for_each(m_ItemList.cbegin(), m_ItemList.cend(), update);
	return fSumme;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::SumSetpoint(void)
{
	m_RecipeSetpoint = SumRecipeSetpoint();
	m_LokalSetpoint = SumLocalModeSetpoint();
	m_RegeneratSetpoint = SummeRegeneratSetpoint();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::SumPercentage(void)
{
	m_RecipePercentage = SumRecipePercentage();
	m_RegeneratPercentage = SumRegeneratPercentage();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
float32_t CRecipeItemList::SumRecipePercentage(void) const
{
	float32_t fSumme = 0.0f;

	auto update = [&fSumme](const auto& item) 
	{ 
		if ( ( ! item.GetLocalMode()) && ( ! item.GetRegenerat())) 
		{ 
			fSumme += item.GetRecipePercentage(); 
		} 
	};
	std::for_each(m_ItemList.cbegin(), m_ItemList.cend(), update);
	return fSumme;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
float32_t CRecipeItemList::SumRegeneratPercentage(void) const
{
	float32_t fSumme = 0.0f;

	auto update = [&fSumme](const auto& item) 
	{ 
		if ( ( ! item.GetLocalMode() ) && ( item.GetRegenerat() )) 
		{ 
			fSumme += item.GetRecipePercentage(); 
		} 
	};
	std::for_each(m_ItemList.cbegin(), m_ItemList.cend(), update);
	return fSumme;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecipeItemList::CalcRecipeSum(void) const
{
	auto it = std::find_if(m_ItemList.cbegin(), m_ItemList.cend(), [](const auto& item) -> BOOL { return (!item.GetLocalMode()); });
	BOOL bLinie = it != m_ItemList.cend();
	return 	((!bLinie && (fabs(m_RecipePercentage) < 0.2)) || (fabs(100.0f - m_RecipePercentage) < 0.2));
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::SetSetpoint(int32_t n, float32_t f)
{
	auto& item = GetItem(n);
	item.SetSetpoint(f);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateLineSetpoint(int32_t n)
{
	auto& item = GetItem(n);
	if ( ! item.GetLocalMode())
	{
		item.UpdateSetpoint(m_LineSetpoint);
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateLineSetpoint(void)
{
	float32_t fSetpoint = m_LineSetpoint;

	auto update = [&fSetpoint]( auto& item) 
	{ 
		if ( ! item.GetLocalMode() ) 
		{ 
			item.UpdateSetpoint(fSetpoint); 
		} 
	};
	std::for_each(m_ItemList.begin(), m_ItemList.end(), update);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateRecipeSetpoint(void)
{
	float32_t fSumme = 0;
	float32_t fSetpoint = m_RecipeSetpoint;

	auto update = [&fSetpoint, &fSumme](auto& item)
	{
		if ( ( ! item.GetLocalMode() ) && ( ! item.GetRegenerat() ))
		{
			item.UpdateSetpoint(fSetpoint);
			fSumme += item.GetSetpoint();
		}
	};

	std::for_each(m_ItemList.begin(), m_ItemList.end(), update);
	m_RecipeSetpoint = fSumme;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateRecipeSetpoint(int32_t n)
{
	auto& item  = GetItem(n);
	if ( ( ! item.GetLocalMode()) && (!item.GetRegenerat()))
	{
		item.UpdateSetpoint(m_RecipeSetpoint);
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateRegeneratSetpoint(void)
{
	float32_t fSumme = 0;
	float32_t fSetpoint = m_LineSetpoint;

	auto update = [&fSetpoint, &fSumme](auto& item)
	{
		if (( ! item.GetLocalMode() ) && ( item.GetRegenerat() ))
		{
			item.UpdateSetpoint(fSetpoint);
			fSumme += item.GetSetpoint();
		}
	};

	std::for_each(m_ItemList.begin(), m_ItemList.end(), update);
	m_RegeneratSetpoint = fSumme;
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateRegeneratSetpoint(int32_t n)
{
	auto& item = GetItem(n);
	if (( ! item.GetLocalMode() ) && ( item.GetRegenerat() ))
	{
		item.UpdateSetpoint(m_LineSetpoint);
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::SetRecipePercentage(int32_t n, float32_t f)
{
	auto& item = GetItem(n);
	item.SetRecipePercentage( ( ! item.GetLocalMode() ) ? f : 0.0F);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateRecipePercentage(int32_t n)
{
	auto& item = GetItem(n);
	if ( item.GetLocalMode() )
	{
		item.SetRecipePercentage(0.0F);
	}
	else if ( ! item.GetRegenerat() )
	{
		item.UpdateAnteil(m_RecipeSetpoint);
	}
	else
	{}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateRecipePercentage(void)
{
	float32_t fSetpoint = m_RecipeSetpoint;

	auto update = [&fSetpoint](auto& item)
	{
		if ( item.GetLocalMode() )
		{
			item.SetRecipePercentage(0.0F);
		}
		else if ( ! item.GetRegenerat() )
		{
			item.UpdateAnteil(fSetpoint);
		}
		else
		{}
	};

	std::for_each(m_ItemList.begin(), m_ItemList.end(), update);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateLocalMode(int32_t n, BOOL b)
{
	auto& item = GetItem(n);
	if (item.GetLocalMode() != b)
	{
		item.SetLocalMode(b);
		item.SetRecipePercentage(0.0f);
		if ( item.GetRegenerat() )
		{
			item.UpdateSetpoint(m_LineSetpoint);
		}
		else
		{
			item.UpdateSetpoint(m_RecipeSetpoint);
		}
		Summe();
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateSingleSetpoint(int32_t n, float32_t f)
{
	// Liniensollwert updaten, Lokalsollwert leit gleich
	auto& item = GetItem(n);
	if ( item.GetLocalMode() )
	{
		item.SetSetpoint(f);
	}
	else
	{
		const float32_t fDiff = f - item.GetSetpoint();
		item.SetSetpoint(f);
		m_LineSetpoint	 += fDiff;
		if (item.GetRegenerat())
		{
			m_RegeneratSetpoint += fDiff;
			UpdateRegeneratPercentage();
		}
		else
		{
			// Kein Regenerat
			m_RecipeSetpoint += fDiff;
			UpdateRecipePercentage();
		}
	}
	Summe();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateEinzelAnteil(int32_t n, float32_t f)
{
	// Liniensollwert updaten, Lokalsollwert leit gleich
	auto& item = GetItem(n);
	if ( ! item.GetLocalMode())
	{
		item.SetRecipePercentage(f);
		if (item.GetRegenerat())
		{
			const float32_t fSAlt = item.GetSetpoint();
			item.UpdateSetpoint(m_LineSetpoint);;
			float32_t fDiff = item.GetSetpoint() - fSAlt;
			m_RecipeSetpoint -= fDiff;
			UpdateRecipeSetpoint();
		}
		else
		{
			// Kein Regenerat
			item.UpdateSetpoint(__max(m_LineSetpoint - m_RegeneratSetpoint, 0));
		}
		Summe();
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateRegenerat()
{
	SumSetpoint();
	m_RecipeSetpoint = __max(m_LineSetpoint, m_RecipeSetpoint) - m_RegeneratSetpoint;
	UpdateRecipeSetpoint();
	Summe();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateRegeneratPercentage(void)
{
	float32_t fSetpoint = m_LineSetpoint;

	auto update = [&fSetpoint](auto& item) 
	{ 
		if ( item.GetLocalMode() ) 
		{ 
			item.SetRecipePercentage(0.0F);
		} 
		else if (item.GetRegenerat()) 
		{ 
			item.UpdateAnteil(fSetpoint);
		}
	};

	std::for_each(m_ItemList.begin(), m_ItemList.end(), update);
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::GetDoseStatistics(CDoseStatistics& rStatistics)
{
	rStatistics.Clear();
	std::for_each(m_ItemList.cbegin(), m_ItemList.cend(), [&rStatistics](const CRecipeItem& rItem) { rStatistics[_S32(rItem.GetDoseType())]++; });
#if _DEBUG
	auto total = rStatistics.CalcTotal();
	assert(total == GetCount());
#else
	(void)rStatistics.CalcTotal();
#endif
}
//*********************************************************************************************************************
//*********************************************************************************************************************
void CRecipeItemList::UpdateLineSetpoint(float32_t fLNeu)
{
	// Liniensollwert updaten, Lokalsollwert bleibt gleich
	SetLineSetpoint(fLNeu);
	UpdateRegeneratSetpoint();
	SetRecipeSetpoint(__max(fLNeu - m_RegeneratSetpoint, 0));
	UpdateRecipeSetpoint();
	Summe();
}
//*********************************************************************************************************************
//*********************************************************************************************************************
BOOL CRecipeItemList::IsRegenerat(int32_t n) const
{
	const auto& item  = GetItem(n);
	return item.GetRegenerat();
}


};
};


