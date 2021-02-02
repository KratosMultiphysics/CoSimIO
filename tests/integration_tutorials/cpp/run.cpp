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

// CoSimulation includes
#include "co_sim_io.hpp"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a        \
                  << " is not equal to " << b << std::endl;      \
        return 1;                                                \
    }

std::string s_connection_name;

CoSimIO::Info AdvanceInTime(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "AdvanceInTime");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}

CoSimIO::Info InitializeSolutionStep(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "InitializeSolutionStep");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}

CoSimIO::Info Predict(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "Predict");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}

CoSimIO::Info SolveSolutionStep(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "SolveSolutionStep");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}

CoSimIO::Info FinalizeSolutionStep(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "FinalizeSolutionStep");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}

CoSimIO::Info OutputSolutionStep(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "OutputSolutionStep");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}

CoSimIO::Info ImportData(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "ImportData");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}

CoSimIO::Info ExportData(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "ExportData");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}

CoSimIO::Info ImportMesh(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "ImportMesh");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}

CoSimIO::Info ExportMesh(const CoSimIO::Info& I_Info)
{
    CoSimIO::Info info;
    info.Set("connection_name", s_connection_name);
    info.Set("identifier", "info_for_test");
    info.Set("name_for_check", "ExportMesh");
    CoSimIO::ExportInfo(info);
    return CoSimIO::Info();
}


int main()
{
    CoSimIO::Info settings;
    settings.Set("my_name", "cpp_run");
    settings.Set("connect_to", "cpp_runner");
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    auto info = CoSimIO::Connect(settings);
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);
    s_connection_name = info.Get<std::string>("connection_name");

    // registering the functions
    CoSimIO::Info fct_info;
    fct_info.Set("connection_name", s_connection_name);

    fct_info.Set("function_name", "AdvanceInTime");
    CoSimIO::Register(fct_info,   &AdvanceInTime);

    fct_info.Set("function_name", "InitializeSolutionStep");
    CoSimIO::Register(fct_info,   &InitializeSolutionStep);

    fct_info.Set("function_name", "Predict");
    CoSimIO::Register(fct_info,   &Predict);

    fct_info.Set("function_name", "SolveSolutionStep");
    CoSimIO::Register(fct_info,   &SolveSolutionStep);

    fct_info.Set("function_name", "FinalizeSolutionStep");
    CoSimIO::Register(fct_info,   &FinalizeSolutionStep);

    fct_info.Set("function_name", "OutputSolutionStep");
    CoSimIO::Register(fct_info,   &OutputSolutionStep);

    fct_info.Set("function_name", "ImportData");
    CoSimIO::Register(fct_info,   &ImportMesh);

    fct_info.Set("function_name", "ExportData");
    CoSimIO::Register(fct_info,   &ExportMesh);

    fct_info.Set("function_name", "ImportMesh");
    CoSimIO::Register(fct_info,   &ImportMesh);

    fct_info.Set("function_name", "ExportMesh");
    CoSimIO::Register(fct_info,   &ExportMesh);

    CoSimIO::Info run_info;
    run_info.Set("connection_name", s_connection_name);
    CoSimIO::Run(run_info);

    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", s_connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}
