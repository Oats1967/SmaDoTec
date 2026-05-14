#pragma once

#include <string>
#include "types.h"

using headerstring = std::array<std::wstring, 4>;

const headerstring& getHeader(eLanguage language);


