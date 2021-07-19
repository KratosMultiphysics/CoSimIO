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

#ifndef CO_SIM_IO_MACROS_INCLUDED
#define CO_SIM_IO_MACROS_INCLUDED

/* This file defines macros that are used inside the CoSimIO
Note that they are only defined here if they haven't been defined before.
This makes it possible to override them to use macros that are coming from
the code where the CoSimIO is included
*/

// Exception macros
#ifndef CO_SIM_IO_ERROR
    #include "exception.hpp"
    #define CO_SIM_IO_ERROR throw CoSimIO::Exception("Error: ", CO_SIM_IO_CODE_LOCATION)
    #define CO_SIM_IO_ERROR_IF(conditional) if (conditional) CO_SIM_IO_ERROR
    #define CO_SIM_IO_ERROR_IF_NOT(conditional) if (!(conditional)) CO_SIM_IO_ERROR

    // debug exception macros
    #ifdef CO_SIM_IO_DEBUG
        #define CO_SIM_IO_DEBUG_ERROR CO_SIM_IO_ERROR
        #define CO_SIM_IO_DEBUG_ERROR_IF(conditional) CO_SIM_IO_ERROR_IF(conditional)
        #define CO_SIM_IO_DEBUG_ERROR_IF_NOT(conditional)  CO_SIM_IO_ERROR_IF_NOT(conditional)
    #else
        #define CO_SIM_IO_DEBUG_ERROR if(false) CO_SIM_IO_ERROR
        #define CO_SIM_IO_DEBUG_ERROR_IF(conditional) if(false) CO_SIM_IO_ERROR_IF(conditional)
        #define CO_SIM_IO_DEBUG_ERROR_IF_NOT(conditional) if(false) CO_SIM_IO_ERROR_IF_NOT(conditional)
    #endif
#endif

// Logging macros
#ifndef CO_SIM_IO_INFO
    #include <iostream>
    #define CO_SIM_IO_INFO(label) std::cout << label << ": "
    #define CO_SIM_IO_INFO_IF(label, conditional) if (conditional) CO_SIM_IO_INFO(label)
#endif

#endif // CO_SIM_IO_MACROS_INCLUDED
