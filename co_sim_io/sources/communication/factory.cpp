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
#include "includes/communication/local_socket_communication.hpp"
#include "includes/communication/socket_communication.hpp"

namespace CoSimIO {
namespace Internals {

std::unique_ptr<Communication> CommunicationFactory::Create(
    const Info& I_Settings,
    const std::shared_ptr<DataCommunicator> pDataComm) const
{
    // deliberately not providing a default
    const std::string comm_format = I_Settings.Get<std::string>("communication_format");

    const auto fcts = GetCommunicationCreateFunctions();

    auto comm_crate_fct = fcts.find(comm_format);

    if (comm_crate_fct != fcts.end()) {
        return comm_crate_fct->second(I_Settings, pDataComm);
    } else {
        std::stringstream err_msg;
        err_msg << "Unsupported communication format: " << comm_format << "!\nOnly the following option are available:";
        for (const auto& name_fct_pair : fcts) {
            err_msg << "\n    " << name_fct_pair.first;
        }
        CO_SIM_IO_ERROR << err_msg.str() << std::endl;
    }
}

CommunicationFactory::CommCreateFctsType CommunicationFactory::GetCommunicationCreateFunctions() const
{
    CommunicationFactory::CommCreateFctsType fcts;

    fcts["file"] = [](
        const Info& I_Settings,
        const std::shared_ptr<DataCommunicator> pDataComm){
            return CoSimIO::make_unique<FileCommunication>(I_Settings, pDataComm);};

    fcts["pipe"] = [](
        const Info& I_Settings,
        const std::shared_ptr<DataCommunicator> pDataComm){
            return CoSimIO::make_unique<PipeCommunication>(I_Settings, pDataComm);};

    fcts["local_socket"] = [](
        const Info& I_Settings,
        const std::shared_ptr<DataCommunicator> pDataComm){
            return CoSimIO::make_unique<LocalSocketCommunication>(I_Settings, pDataComm);};

    fcts["socket"] = [](
        const Info& I_Settings,
        const std::shared_ptr<DataCommunicator> pDataComm){
            return CoSimIO::make_unique<SocketCommunication>(I_Settings, pDataComm);};

    return fcts;
}

} // namespace Internals
} // namespace CoSimIO
