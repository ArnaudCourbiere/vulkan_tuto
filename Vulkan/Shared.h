#pragma once

#include <iostream>
#include <assert.h>
#include "Platform.h"

#ifdef _WIN32
std::wstring s2ws(const std::string& s);
#endif

void errorCheck(VkResult result);
