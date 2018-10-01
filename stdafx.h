// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#include <cstdlib>    // C standard library - for EXIT_SUCCESS, system(), srand(), rand(), size_t
#include <cstdint>    // C standard integer data types - for uint_fast8_t, uint16_t
#include <cctype>     // C char type libraries - for isdigit()
#include <cwchar>     // C wide char libraries - for wcspcy_s
#include <ctime>      // C time library - for time()

#include <ios>        // C++ streams base class  - for ios::beg
#include <iostream>   // C++ input/output stream - for std::cout, std::cin, std::endl;
#include <fstream>    // C++ file streams        - for ifstream
#include <iomanip>    // C++ input/output manipulators - for setw(), setfill()
#include <string>     // C++ strings - for std::string, stoi(), to_string(), getline()

#include <windows.h>  // Windows API - for HANDLE, Sleep(), FOREGROUND_X color codes