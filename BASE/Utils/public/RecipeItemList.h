//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeItemList.h
///
/// @file   RecipeItemList.h
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


#include "BASE/Utils/public/GenericList.h"
#include "BASE/Utils/public/RecipeItem.h"
#include "BASE/include/DoseStatistics.h"

namespace base
{
namespace utils
{

class CRecipeItemList : public CGenericList < CRecipeItem >
{
private:
	std::string			m_Name;
	uint64_t		    m_QMNumber;
	float32_t		    m_LineSetpoint;

private: // the following values will be calculated
	float32_t		    m_RecipeSetpoint;
	float32_t           m_RegeneratSetpoint;
	float32_t		    m_RecipePercentage;
	float32_t           m_RegeneratPercentage;
	float32_t           m_LokalSetpoint;

private:
	void CopyItems(const CRecipeItemList& dest)
	{
		m_Name = dest.m_Name;
		m_QMNumber = dest.m_QMNumber;
		m_LineSetpoint = dest.m_LineSetpoint;
		m_RecipeSetpoint = dest.m_RecipeSetpoint;
		m_RegeneratSetpoint = dest.m_RegeneratSetpoint;
		m_RecipePercentage = dest.m_RecipePercentage;
		m_RegeneratPercentage = dest.m_RegeneratPercentage;
		m_LokalSetpoint = dest.m_LokalSetpoint;
	}

	std::string MakeString(void) const;

protected:
	BOOL LoadStream(std::ifstream& file_in) override;
	BOOL SaveStream(std::ofstream& file_out) override;

public:
	CRecipeItemList(int32_t lID = -1L);
	CRecipeItemList(const CRecipeItemList& dest): CGenericList < CRecipeItem >(dest)
	{	CopyItems(dest);	}

	CRecipeItemList& operator =	(const CRecipeItemList& dest)
	{
		CGenericList < CRecipeItem >::operator = (dest);
		CopyItems(dest);
		return *this;
	}

	~CRecipeItemList(void) = default;

	SETGET(const std::string&, Name)
	SETGET(uint64_t, QMNumber)
	SETGET(float32_t, RecipeSetpoint)
	SETGET(float32_t, RecipePercentage)
	SETGET(float32_t, LineSetpoint)
	SETGET(float32_t, RegeneratSetpoint)
	SETGET(float32_t, RegeneratPercentage)
	SETGET(float32_t, LokalSetpoint)

public:
	BOOL CalcRecipeSum(void) const;
	float32_t SumLocalModeSetpoint(void) const;
	float32_t SumRecipeSetpoint(void) const;
	float32_t SummeRegeneratSetpoint(void) const;
	void SumSetpoint(void);

	float32_t SumRecipePercentage(void) const;
	float32_t SumRegeneratPercentage(void) const;
	void SumPercentage(void);

	void Summe(void)
	{
		SumSetpoint(); 
		SumPercentage();
	}

	void SetRecipePercentage(int32_t n, float32_t f);
	void UpdateRecipePercentage(int32_t n);
	void UpdateRecipePercentage(void);

	void UpdateRegeneratPercentage(void);

	void SetSetpoint(int32_t n, float32_t f);

	void UpdateRecipeSetpoint(int32_t n);
	void UpdateRegeneratSetpoint(int32_t n);
	void UpdateRecipeSetpoint(void);
	void UpdateRegeneratSetpoint(void);

	void UpdateLocalMode(int32_t n, BOOL b);

	void UpdateLineSetpoint(int32_t);
	void UpdateLineSetpoint(void);

	void UpdateSingleSetpoint(int32_t n, float32_t f);
	void UpdateEinzelAnteil(int32_t n, float32_t f);
	void UpdateLineSetpoint(float32_t fLNeu);
	void UpdateRegenerat(void);

	BOOL IsRegenerat(int32_t) const;

	void GetDoseStatistics(CDoseStatistics& rStatistics);
};


};
};




