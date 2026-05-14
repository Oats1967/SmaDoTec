#pragma once

#include "types.h"


int replacelines(const VectorCSVLang& strglist, const eLanguage langIx, VectorString& lines, VectorString& notFound, const VectorString& exclude);
int replaceheader(VectorString& lines, const eLanguage langIx);
