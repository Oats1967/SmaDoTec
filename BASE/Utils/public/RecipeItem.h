//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeItem
///
/// @file   RecipeItem.h
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

#include <fstream>
#include "BASE/Base.h"
#include "BASE/include/DoseType.h"

namespace base
{
namespace utils
{
//*****************************************************************************************
class CRecipeItem		// RecipeItem
{
private:
	int32_t					m_ID;					// Recipe-ID
	std::string				m_Name;
	uint64_t				m_QMNumber;
	float32_t				m_Setpoint;
	float32_t				m_RecipePercentage;
	float32_t				m_RefillMax;
	float32_t				m_RefillMin;
	float32_t				m_RefillAlarmLimit;
	BOOL					m_LocalMode;
	BOOL					m_Regenerat;
	float32_t				m_DosePerformance;
	float32_t				m_MaxSetpoint;
	eDoseType				m_DoseType;


	float32_t				m_Totalizer;			// Nur temporär zu Speicherzwecken, wird nicht in der Rezeptur abgespeichert
protected:

	friend class CRecipeItemList;

public:
	CRecipeItem(int32_t _lID = -1L);
	virtual ~CRecipeItem(void) = default;
	CRecipeItem(const CRecipeItem&) = default;
	CRecipeItem(CRecipeItem&&) = default;

	CRecipeItem& operator =		(const CRecipeItem&) = default;

	std::string MakeString(void) const;

	SETGET(int32_t, ID)
	SETGET(const std::string&, Name)
	SETGET(uint64_t, QMNumber)
	SETGET(float32_t, Setpoint)
	SETGET(float32_t, RecipePercentage)
	SETGET(float32_t, RefillMax)
	SETGET(float32_t, RefillMin)
	SETGET(float32_t, RefillAlarmLimit)
	SETGET(BOOL, LocalMode)
	SETGET(BOOL, Regenerat)
	SETGET(float32_t, DosePerformance)
	SETGET(float32_t, MaxSetpoint)
	SETGET(eDoseType, DoseType)
	SETGET(float32_t, Totalizer)

	void UpdateSetpoint(float32_t f)
	{
		m_Setpoint = m_RecipePercentage / 100.0f * f;
	}

	void UpdateAnteil(float32_t f)
	{
		m_RecipePercentage = (f > 0.0f) ? m_Setpoint / f * 100.0f : 0.0f;
	}

};




//------------------------------------------------------------------------------------
std::istream& operator>>(std::istream& is, CRecipeItem& dt);

//------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const CRecipeItem& dt);




};
};




