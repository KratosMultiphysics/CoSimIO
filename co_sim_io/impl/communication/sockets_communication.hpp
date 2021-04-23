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

#ifndef CO_SIM_IO_SOCKETS_COMMUNICATION_INCLUDED
#define CO_SIM_IO_SOCKETS_COMMUNICATION_INCLUDED

// System includes

// External includes
#define ASIO_NO_DEPRECATED
#define ASIO_STANDALONE
#include <asio.hpp>

// Project includes
#include "communication.hpp"

namespace CoSimIO {
namespace Internals {

class SocketsCommunication : public Communication
{
public:
    explicit SocketsCommunication(const Info& I_Settings) : Communication(I_Settings)
    {
        CO_SIM_IO_ERROR << "Sockets Communication is not implemented yet" << std::endl;
    }

    Info ConnectDetail(const Info& I_Info) override
    {
        return Info();
    };

    Info DisconnectDetail(const Info& I_Info) override
    {
        return Info();
    };
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_SOCKETS_COMMUNICATION_INCLUDED
