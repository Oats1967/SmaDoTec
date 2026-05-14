//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseData
///
/// @file   DoseData.cpp
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
#include "DoseData.h"
#include "Dose.h"



//**************************************************************************************
//**************************************************************************************
int32_t Dose_Load(const std::string& _filename)
{
	assert(DOSEPOINTER);
	return DOSEREF.Load(_filename);
}
//**************************************************************************************
//**************************************************************************************
int32_t Dose_Save	(const std::string& _filename)
{
	assert(DOSEPOINTER);
	return DOSEREF.Save(_filename);
}
//**************************************************************************************
//**************************************************************************************
int32_t Dose_SaveBak(const std::string& _filename)
{
	assert(DOSEPOINTER);
	return DOSEREF.SaveBak(_filename);
}
//**************************************************************************************
//**************************************************************************************
int32_t Dose_Activate()
{
	assert(DOSEPOINTER);
	return DOSEREF.Activate();
}
//**************************************************************************************
//**************************************************************************************
int32_t Dose_Restore(const std::string& _filename)
{
	assert(DOSEPOINTER);
	return DOSEREF.Restore(_filename);
}
//**************************************************************************************
//**************************************************************************************
int32_t Dose_ResetAll	(void)
{
	assert(DOSEPOINTER);
	DOSEREF.ResetAll();
	return DOSIERUNG_OK;
}
//**************************************************************************************
//**************************************************************************************
int32_t Dose_InitializeAll	(void)
{
	assert(DOSEPOINTER);
	DOSEREF.InitializeAll();
	return DOSIERUNG_OK;
}
//**************************************************************************************
//**************************************************************************************
int32_t Dose_Open	()
{
	assert(DOSEPOINTER == NULL);
	if ( DOSEPOINTER )
	{
		delete DOSEPOINTER;
		DOSEPOINTER = NULL;
	}
	DOSEPOINTER		= new CDose;
	assert(DOSEPOINTER != NULL);
	BOOL bErg = DOSEREF.Open ();
	return (bErg) ? DOSIERUNG_OK : DOSIERUNG_NOTOPEN;
}
//**************************************************************************************
//**************************************************************************************
int32_t Dose_Close	( void )
{
	assert(DOSEPOINTER);

	if (!DOSEPOINTER)
	{
		return DOSIERUNG_OK;
	}

	DOSEREF.Close ();
	delete DOSEPOINTER;
	DOSEPOINTER = NULL;
	return DOSIERUNG_OK;
}


