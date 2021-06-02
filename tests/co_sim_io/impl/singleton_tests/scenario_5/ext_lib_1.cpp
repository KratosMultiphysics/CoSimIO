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

// Project includes
#include "co_sim_io.hpp"
#include "ext_lib_1.hpp"

std::string ConnectToCoSimIO()
{
    CoSimIO::Info settings;
    settings.Set("my_name",    "singleton_tester");
    settings.Set("connect_to", "singleton_tester_partner");
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    auto info = CoSimIO::Connect(settings);
    return info.Get<std::string>("connection_name");
}

void DisconnectFromCoSimIO(const std::string& rConnectionName)
{
    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", rConnectionName);
    CoSimIO::Disconnect(disconnect_settings);
}