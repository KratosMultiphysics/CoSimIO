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

#ifndef CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED
#define CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED

// System includes

// Project includes
#include "communication.hpp"

namespace CoSimIO {
namespace Internals {

namespace { // helpers namespace



} // helpers namespace


class CO_SIM_IO_API PipeCommunication : public Communication
{
public:
    PipeCommunication(
        const Info& I_Settings,
        std::shared_ptr<DataCommunicator> I_DataComm);

    ~PipeCommunication() override
    {
        if (GetIsConnected()) {
            CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
            Info tmp;
            Disconnect(tmp);
        }
    }

private:

    fs::path mPipeName;
    int mPipe;

    Info ConnectDetail(const Info& I_Info) override;

    Info DisconnectDetail(const Info& I_Info) override;

    Info ImportInfoImpl(const Info& I_Info) override;

    Info ExportInfoImpl(const Info& I_Info) override;

    void SendSize(const std::uint64_t Size);

    std::uint64_t ReceiveSize();
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED
