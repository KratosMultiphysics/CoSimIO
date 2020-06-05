//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Philipp Bucher (https://github.com/philbucher)
//

#ifndef CO_SIM_IO_FILESYSTEM_INC_H_INCLUDED
#define CO_SIM_IO_FILESYSTEM_INC_H_INCLUDED

/* This file selects which implementation of std::filesystem to be used
std::filesystem is part of C++17
When using only C++11 the alternative implementation from
"https://github.com/gulrak/filesystem" is used
*/

// To dynamically select std::filesystem where available, you could use:
#if defined(__cplusplus) && __cplusplus >= 201703L
    #if __has_include(<filesystem>) // has_include is C++17
        #include <filesystem>
        namespace fs = std::filesystem;
    #elif __has_include(<experimental/filesystem>)
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #else
        #define NOMINMAX
        #include "../../external_libraries/ghc/filesystem.hpp"
        namespace fs = ghc::filesystem;
    #endif
#else // not C++17
    #define NOMINMAX
    #include "../../external_libraries/ghc/filesystem.hpp"
    namespace fs = ghc::filesystem;
#endif

#endif // CO_SIM_IO_FILESYSTEM_INC_H_INCLUDED
