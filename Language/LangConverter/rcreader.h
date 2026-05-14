#pragma once

#include "types.h"


int readExclude(const std::string& filenameIn, VectorString& strglist);
int readRC(const std::string& filenameIn, VectorString& strglist);
int writeRC(const std::string& filenameout, VectorString& strglist, const eLanguage eLang);


