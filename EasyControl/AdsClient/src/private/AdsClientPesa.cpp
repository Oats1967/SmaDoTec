//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellIsPresent	( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellIsPresent( index );
	}
	return result;
}
	
//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellGetWeight	( const int32_t index, float *f)
{
	assert(f);
	*f = 0.0f;

	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellGetWeight( index, f);
	}
	return result;
}


//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellSetZero	( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellSetZero( index );
	}
	return result;
}


//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellDeleteZero	( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellDeleteZero( index );
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellSetReference	( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellSetReference( index );
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellDeleteReference	( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellDeleteReference( index );
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellSetWindowRes			( const int32_t index, int32_t iRes)
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellSetWindowRes( index, iRes);
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellSetWindowTime			( const int32_t index, int32_t iTime)
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellSetWindowTime( index, iTime);
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellSetNoiseControl			( const int32_t index, int32_t iRes)
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellSetNoiseControl( index, iRes);
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellSetNoiseControlWeight			( const int32_t index, int32_t iRes)
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellSetNoiseControlWeight( index, iRes);
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellSetAddress			( const int32_t index, int32_t iNew)
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellSetAddress( index, iNew);
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellSetTara ( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellSetTara( index );
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellClrTara ( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellClrTara( index );
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellGetSensorActive	( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellGetSensorActive( index );
	}
	return result;
}


//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellGetFullLoad			( const int32_t index, float *f)
{
	assert(f);
	*f = 0.0f;

	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellGetFullLoad( index, f);
	}
	return result;
}


//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellSetPPM					( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellSetPPM( index );
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellEEpromVerify	( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellEEpromVerify( index );
	}
	return result;
}

//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellPrepareWriteEEprom			( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellPrepareWriteEEprom( index );
	}
	return result;
}


//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellEnableEEpromWriteProtection ( const int32_t index, const BOOL b)
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellEnableEEpromWriteProtection( index, b);
	}
	return result;
}


//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellRegisterSCL ( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellRegisterSCL( index );
	}
	return result;
}


//***********************************************************************************************************************************
//***********************************************************************************************************************************
int32_t AdsClient_PesaCellReset ( const int32_t index )
{
	int32_t result = AdsClient_CheckDoseConditions();
	if (result == ADS_OK)
	{
		result = AdsPesaCellReset( index );
	}
	return result;
}

