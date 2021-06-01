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

#include "ext_lib.hpp"

std::string ConnectToCoSimIO(CoSimIO::Info& rSettings)
{
    auto info = CoSimIO::Connect(rSettings);
    return info.Get<std::string>("connection_name");
}

void DisconnectFromCoSimIO(const std::string& rConnectionName)
{
    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", rConnectionName);
    CoSimIO::Disconnect(disconnect_settings);
}