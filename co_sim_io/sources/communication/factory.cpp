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

// System includes

// Project includes
#include "includes/communication/factory.hpp"
#include "includes/communication/file_communication.hpp"
#include "includes/communication/pipe_communication.hpp"
#include "includes/communication/socket_communication.hpp"

namespace CoSimIO {
namespace Internals {

std::unique_ptr<Communication> CreateCommunication(
    const Info& I_Settings,
    const std::shared_ptr<DataCommunicator> pDataComm)
{
    // deliberately not providing a default
    const std::string comm_format = I_Settings.Get<std::string>("communication_format");

    if (comm_format == "file") {
        return CoSimIO::make_unique<FileCommunication>(I_Settings, pDataComm);
    } else if (comm_format == "pipe") {
        return CoSimIO::make_unique<PipeCommunication>(I_Settings, pDataComm);
    } else if (comm_format == "socket") {
        return CoSimIO::make_unique<SocketCommunication>(I_Settings, pDataComm);
    } else {
        CO_SIM_IO_ERROR << "Unsupported communication format: " << comm_format << std::endl;
    }
}

} // namespace Internals
} // namespace CoSimIO
