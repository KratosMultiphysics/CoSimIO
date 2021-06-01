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

#ifndef CO_SIM_IO_ERROR
    #include "exception.hpp"
    #define CO_SIM_IO_ERROR throw CoSimIO::Exception("Error: ", CO_SIM_IO_CODE_LOCATION)
#endif

#ifndef CO_SIM_IO_ERROR_IF
    #include "exception.hpp"
    #define CO_SIM_IO_ERROR_IF(conditional) if (conditional) throw CoSimIO::Exception("Error: ", CO_SIM_IO_CODE_LOCATION)
#endif

#ifndef CO_SIM_IO_ERROR_IF_NOT
    #include "exception.hpp"
    #define CO_SIM_IO_ERROR_IF_NOT(conditional) if (!(conditional)) throw CoSimIO::Exception("Error: ", CO_SIM_IO_CODE_LOCATION)
#endif

#ifndef CO_SIM_IO_INFO
    #include <iostream>
    #define CO_SIM_IO_INFO(label) std::cout << label << ": "
#endif

#ifndef CO_SIM_IO_INFO_IF
    #define CO_SIM_IO_INFO_IF(label, conditional) if (conditional) CO_SIM_IO_INFO(label)
#endif

#endif // CO_SIM_IO_MACROS_INCLUDED
