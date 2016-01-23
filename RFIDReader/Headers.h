#ifndef HEADERS_H
#define HEADERS_H

#pragma once
#pragma comment(lib, "stapi.lib")

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
#include <string>
#include <stdexcept>
using std::string;

#include "SkyeTekAPI.h"
#include "SkyeTekProtocol.h"
#pragma once

#endif