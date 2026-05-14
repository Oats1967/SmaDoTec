#include <cassert>
#include "Dose.h"
#include "DoseDataImplMacro.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t System_GetDosierItems				(uint32_t *s)
{
	assert(DOSEPOINTER);
	return DOSEREF.System_GetDosierItems(s);
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int32_t System_SetDosierItems			(uint32_t n)
{
	assert(DOSEPOINTER);
	assert(n <= GLOBALDOSEMAXCOUNT);
	return DOSEREF.System_SetDosierItems(n);
}



