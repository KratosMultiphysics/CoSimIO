// Kratos   ______     _____ _           ________
//         / ____/___ / ___/(_)___ ___  /  _/ __ |
//        / /   / __ \\__ \/ / __ `__ \ / // / / /
//       / /___/ /_/ /__/ / / / / / / // // /_/ /
//       \____/\____/____/_/_/ /_/ /_/___/\____/
//      Kratos CoSimulationApplication
//
//  License:		 BSD License, see license.txt
//
//  Main authors:    Philipp Bucher (https://github.com/philbucher)
//

#ifndef KRATOS_CO_SIM_IO_MACROS_H_INCLUDED
#define KRATOS_CO_SIM_IO_MACROS_H_INCLUDED

/* This file defines macros that are used inside the CoSimIO
Note that they are only defined here if they haven't been defined before.
This makes it possible to override them to use macros that are coming from
the code where the CoSimIO is included
*/

#ifndef KRATOS_CO_SIM_ERROR
    #include <iostream>
    #include <stdexcept>
    struct err { // helper struct to mimic behavior of KRATOS_ERROR
    err() {std::cout << "Error: ";}
    ~err() noexcept(false) { throw std::exception(); } // destructors are noexcept by default
    };
    #define KRATOS_CO_SIM_ERROR (err(), std::cout)
#endif

#ifndef KRATOS_CO_SIM_ERROR_IF
    #define KRATOS_CO_SIM_ERROR_IF(conditional) if (conditional) KRATOS_CO_SIM_ERROR
#endif

#ifndef KRATOS_CO_SIM_ERROR_IF_NOT
    #define KRATOS_CO_SIM_ERROR_IF_NOT(conditional) if (!conditional) KRATOS_CO_SIM_ERROR
#endif

#ifndef KRATOS_CO_SIM_INFO
    #include <iostream>
    #define KRATOS_CO_SIM_INFO(label) std::cout << label << ": "
#endif

#ifndef KRATOS_CO_SIM_INFO_IF
    #define KRATOS_CO_SIM_INFO_IF(label, conditional) if (conditional) KRATOS_CO_SIM_INFO(label)
#endif

#endif // KRATOS_CO_SIM_IO_MACROS_H_INCLUDED