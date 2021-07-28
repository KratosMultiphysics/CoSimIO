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

// External includes

// Project includes
#include "includes/communication/pipe_communication.hpp"

namespace CoSimIO {
namespace Internals {

PipeCommunication::PipeCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
    mCommFolder = GetWorkingDirectory();
    mCommFolder /= ".CoSimIOFileComm_" + GetConnectionName();
    mPipeName = mCommInFolder ? mCommFolder / GetConnectionName() : fs::path(GetConnectionName());
    mCommInFolder = I_Settings.Get<bool>("use_folder_for_communication", true);
}

// Info PipeCommunication::ConnectDetail(const Info& I_Info)
// {
//     return Info();
// }

// Info PipeCommunication::DisconnectDetail(const Info& I_Info)
// {
//     return Info();
// }

} // namespace Internals
} // namespace CoSimIO
