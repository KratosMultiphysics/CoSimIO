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

#define COSIMIO_CHECK_TRUE(a)                               \
    if (!(a)) {                                             \
        std::cout << "in line " << __LINE__ << " : " << #a  \
                  << " is false!" << std::endl;             \
        return 1;                                           \
    }

std::string s_connection_name;

bool ControlOtherCode(const std::string& I_FunctionName)
{
    // we tell the other code what to do
    CoSimIO::Info ctrl_info;
    ctrl_info.Set("connection_name", s_connection_name);
    ctrl_info.Set("identifier", "run_control");

    ctrl_info.Set("control_signal", I_FunctionName);

    std::cout << ">>> I_FunctionName: " << I_FunctionName << std::endl;

    CoSimIO::ExportInfo(ctrl_info); // here we tell the other code which function to call

    // this is for testing to make sure the function
    // we wanted to call was actually called
    CoSimIO::Info import_info;
    import_info.Set("connection_name", s_connection_name);
    import_info.Set("identifier", "info_for_test");
    auto check_info = CoSimIO::ImportInfo(import_info);

    std::cout << "GOT NAME: " << std::endl;
    std::cout << check_info << std::endl<< std::endl<< std::endl;

    return check_info.Get<std::string>("name_for_check") == I_FunctionName;
}

int main()
{
    CoSimIO::Info settings;
    settings.Set("my_name", "cpp_runner");
    settings.Set("connect_to", "cpp_run");
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    auto info = CoSimIO::Connect(settings);
    COSIMIO_CHECK_TRUE(info.Get<int>("connection_status") == CoSimIO::ConnectionStatus::Connected);
    s_connection_name = info.Get<std::string>("connection_name");

    // we tell the other code what to do

    // initial mesh exchange
    COSIMIO_CHECK_TRUE(ControlOtherCode("ExportMesh"));
    COSIMIO_CHECK_TRUE(ControlOtherCode("ImportMesh"));
    COSIMIO_CHECK_TRUE(ControlOtherCode("ExportMesh"));
    COSIMIO_CHECK_TRUE(ControlOtherCode("ImportMesh"));

    // "solution loop"
    const std::size_t num_time_steps = 5;
    const std::size_t inner_iterations = 3;
    for (std::size_t i=0; i<num_time_steps; ++i) {
        COSIMIO_CHECK_TRUE(ControlOtherCode("AdvanceInTime"));
        COSIMIO_CHECK_TRUE(ControlOtherCode("InitializeSolutionStep"));
        COSIMIO_CHECK_TRUE(ControlOtherCode("Predict"));
        for (std::size_t j=0; j<inner_iterations; ++j) {
            // importing twice, but exporting only once
            COSIMIO_CHECK_TRUE(ControlOtherCode("ImportData"));
            COSIMIO_CHECK_TRUE(ControlOtherCode("ImportData"));
            COSIMIO_CHECK_TRUE(ControlOtherCode("SolveSolutionStep"));
            COSIMIO_CHECK_TRUE(ControlOtherCode("ExportData"));
        }
        COSIMIO_CHECK_TRUE(ControlOtherCode("FinalizeSolutionStep"));
        COSIMIO_CHECK_TRUE(ControlOtherCode("OutputSolutionStep"));
    }

    CoSimIO::Info ctrl_info;
    ctrl_info.Set("connection_name", s_connection_name);
    ctrl_info.Set("identifier", "run_control");

    ctrl_info.Set("control_signal", "end"); // TODO improve
    CoSimIO::ExportInfo(ctrl_info); // here we tell the other code which function to call

    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", s_connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_TRUE(info.Get<int>("connection_status") == CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}
