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

/*
This is a dummy solver implementation that shows how the integration of a pure C++ solver
into the CoSimulation framework works
*/

// System includes
#include <iostream>
#include <vector>
#include <array>
#include <stdexcept>

// CoSimulation includes
#include "co_sim_io.hpp"

typedef std::vector<std::vector<std::array<double, 2>>> MeshType;
typedef std::vector<double> DataFieldType;

namespace { // helpers namespace

static int sEchoLevel = 1;
static const double sDeltaTime = 0.1;
static const double sEndTime = 0.5;

static std::vector<double> sNodalCoords;
static std::vector<int> sElementConnectivities;
static std::vector<int> sElementTypes;

static std::vector<double> sFieldPressure;
static std::vector<double> sFieldVelocity;

#define solver_print(level) if(sEchoLevel>=level) std::cout << "Solver [C++]: "

void Initialize()
{
    // Defining the mesh

    /*    -- Mesh --
        0      2      3
        x------x------x
         \     |     /|\
          \  1 |  2 / | \
           \   |   /  |  \
            \  |  /   |   \
             \ | /  3 |  4 \
              \|/     |     \
               x------x-----x
               1      4     5
    */

    sNodalCoords = {
        0.0, 2.5, 1.0, // 0
        2.0, 0.0, 1.5, // 1
        2.0, 2.5, 1.5, // 2
        4.0, 2.5, 1.7, // 3
        4.0, 0.0, 1.7, // 4
        6.0, 0.0, 1.8 //  5
    };

    sElementConnectivities = {
        0, 1, 2, // 1
        1, 3, 2, // 2
        1, 4, 3, // 3
        3, 4, 5, // 4
    };

    sElementTypes = {
        5,5,5,5 // VTK_TRIANGLE
    };

    // Initializing the fields
    // sFieldPressure is coming from outside, we do nothing with it

    sFieldVelocity.resize(sNodalCoords.size()); // a value for each component on each node
    for (std::size_t i=0; i<sNodalCoords.size(); ++i) {
        sFieldVelocity[i] = i*0.125;
    }
}


double AdvanceInTime(double pCurrentTime)
{
    pCurrentTime += sDeltaTime;
    return pCurrentTime;
    solver_print(2) << "AdvanceInTime; new time: " << pCurrentTime << std::endl;
}

void InitializeSolutionStep()
{
    solver_print(2) << "InitializeSolutionStep" << std::endl;
}

void SolveSolutionStep()
{
    solver_print(2) << "SolveSolutionStep" << std::endl;
}

void FinalizeSolutionStep()
{
    solver_print(2) << "FinalizeSolutionStep" << std::endl;
}


CoSimIO::ReturnInfo AdvanceInTime_CoSimIO(const CoSimIO::Info& I_Info)
{
    const double current_time = I_Info.Get<double>("current_time");
    const double new_time = AdvanceInTime(current_time);

    CoSimIO::ReturnInfo ret_info;
    ret_info.Set<double>("new_time", new_time);

    return ret_info;
}

CoSimIO::ReturnInfo InitializeSolutionStep_CoSimIO(const CoSimIO::Info& I_Info)
{
    InitializeSolutionStep();
    return CoSimIO::ReturnInfo();
}

CoSimIO::ReturnInfo SolveSolutionStep_CoSimIO(const CoSimIO::Info& I_Info)
{
    SolveSolutionStep();
    return CoSimIO::ReturnInfo();
}

CoSimIO::ReturnInfo FinalizeSolutionStep_CoSimIO(const CoSimIO::Info& I_Info)
{
    FinalizeSolutionStep();
    return CoSimIO::ReturnInfo();
}


CoSimIO::ReturnInfo ImportMesh_CoSimIO(const CoSimIO::Info& I_Info)
{
    // importing the mesh, but doing nothing with it to not overcomplicate this example
    std::vector<double> incoming_nodal_coords;
    std::vector<int> incoming_elem_connectivities;
    std::vector<int> incoming_elem_types;

    solver_print(3) << "\tBefore Importing Mesh from CoSim" << std::endl;
    CoSimIO::ImportMesh(I_Info, incoming_nodal_coords, incoming_elem_connectivities, incoming_elem_types);
    solver_print(3) << "\tAfter Importing Mesh from CoSim" << std::endl;

    // Now copy the imported mesh to the solver-internal data structure to use it ...
    // "rIdentifier" is used to identify which mesh was imported
    return CoSimIO::ReturnInfo();
}

CoSimIO::ReturnInfo ExportMesh_CoSimIO(const CoSimIO::Info& I_Info)
{
    // usually use "rIdentifier" to select which mesh to export
    // (and then copy from solver-specific data structure to buffer which is passed to the IO)
    // in this example this is not necessary since we only export one mesh
    solver_print(3) << "\tBefore Exporting Mesh from CoSim" << std::endl;
    CoSimIO::ExportMesh(I_Info, sNodalCoords, sElementConnectivities, sElementTypes);
    solver_print(3) << "\tAfter Exporting Mesh from CoSim" << std::endl;
    return CoSimIO::ReturnInfo();
}

CoSimIO::ReturnInfo ImportData_CoSimIO(const CoSimIO::Info& I_Info)
{
    // usually use "rIdentifier" to select which data to import
    // (and then copy from solver-specific data structure to buffer which is passed to the IO)
    // in this example this is not necessary since we always import "field_pressure"

    solver_print(3) << "\tBefore Importing Data from CoSim" << std::endl;
    CoSimIO::ImportData(I_Info, sFieldPressure);
    solver_print(3) << "\tAfter Importing Data from CoSim" << std::endl;
    return CoSimIO::ReturnInfo();
}

CoSimIO::ReturnInfo ExportData_CoSimIO(const CoSimIO::Info& I_Info)
{
    // usually use "rIdentifier" to select which data to export
    // (and then copy from solver-specific data structure to buffer which is passed to the IO)
    // in this example this is not necessary since we always export "field_velocity"

    solver_print(3) << "\tBefore Importing Data from CoSim" << std::endl;
    CoSimIO::ExportData(I_Info, sFieldVelocity);
    solver_print(3) << "\tAfter Importing Data from CoSim" << std::endl;
    return CoSimIO::ReturnInfo();
}

void ImportMesh(const std::string& rCommName, const std::string& rIdentifier)
{
    CoSimIO::Info info;
    info.Set("identifier", rIdentifier);
    info.Set("connection_name", rCommName);
    ImportMesh_CoSimIO(info);
}

void ExportMesh(const std::string& rCommName, const std::string& rIdentifier)
{
    CoSimIO::Info info;
    info.Set("identifier", rIdentifier);
    info.Set("connection_name", rCommName);
    ExportMesh_CoSimIO(info);
}

void ImportData(const std::string& rCommName, const std::string& rIdentifier)
{
    CoSimIO::Info info;
    info.Set("identifier", rIdentifier);
    info.Set("connection_name", rCommName);
    ImportData_CoSimIO(info);
}

void ExportData(const std::string& rCommName, const std::string& rIdentifier)
{
    CoSimIO::Info info;
    info.Set("identifier", rIdentifier);
    info.Set("connection_name", rCommName);
    ExportData_CoSimIO(info);
}


void RunSolutionLoop()
{
    double current_time = 0.0;
    while (current_time < sEndTime) {
        current_time = AdvanceInTime(current_time);
        InitializeSolutionStep();
        SolveSolutionStep();
        FinalizeSolutionStep();
        std::cout << std::endl;
    }
}

void RunSolutionLoopWithWeakCoupling()
{
    // Note the following only works with one coupling interface, requires more effort to make it work with multiple coupling interfaces.

    const std::string conn_name("external_simple_solver");

    CoSimIO::ConnectionSettings conn_settings;
    conn_settings.Set<std::string>("connection_name", conn_name);

    CoSimIO::Connect(conn_settings);

    // ImportMesh(conn_name, "interface_mesh_quads");
    ExportMesh(conn_name, "interface_mesh_tri");

    double current_time = 0.0;
    while (current_time < sEndTime) {
        current_time = AdvanceInTime(current_time);
        InitializeSolutionStep();

        ImportData(conn_name, "field_pressure");
        SolveSolutionStep();
        ExportData(conn_name, "field_velocity");

        FinalizeSolutionStep();
        std::cout << std::endl;
    }

    CoSimIO::Info info;
    info.Set<std::string>("connection_name", conn_name);
    CoSimIO::Disconnect(info);
}

void RunSolutionLoopWithStrongCoupling()
{
    const std::string conn_name("external_simple_solver");

    CoSimIO::Info info;
    info.Set<std::string>("connection_name", conn_name);
    CoSimIO::ConnectionSettings conn_settings;
    conn_settings.Set<std::string>("connection_name", conn_name);

    CoSimIO::Connect(conn_settings);

    // ImportMesh(conn_name, "interface_mesh_quads");
    ExportMesh(conn_name, "interface_mesh_tri");

    double current_time = 0.0;
    while (current_time < sEndTime) {
        current_time = AdvanceInTime(current_time);
        InitializeSolutionStep();

        while(true) {
            ImportData(conn_name, "field_pressure");
            SolveSolutionStep();
            ExportData(conn_name, "field_velocity");

            auto ret_info = CoSimIO::IsConverged(info);
            if (ret_info.Get<bool>("is_converged")) {break;}
        }

        FinalizeSolutionStep();
        std::cout << std::endl;
    }

    CoSimIO::Disconnect(info);
}

void RunSolutionCoSimulationOrchestrated()
{
    const std::string conn_name("external_simple_solver");

    CoSimIO::ConnectionSettings conn_settings;
    conn_settings.Set<std::string>("connection_name", conn_name);

    CoSimIO::Connect(conn_settings);

    CoSimIO::Info info;
    info.Set<std::string>("connection_name", conn_name);

    info.Set<std::string>("function_name", "AdvanceInTime");
    CoSimIO::Register(info, &AdvanceInTime_CoSimIO);

    info.Set<std::string>("function_name", "InitializeSolutionStep");
    CoSimIO::Register(info, &InitializeSolutionStep_CoSimIO);

    info.Set<std::string>("function_name", "SolveSolutionStep");
    CoSimIO::Register(info, &SolveSolutionStep_CoSimIO);

    info.Set<std::string>("function_name", "FinalizeSolutionStep");
    CoSimIO::Register(info, &FinalizeSolutionStep_CoSimIO);

    info.Set<std::string>("function_name", "ImportData");
    CoSimIO::Register(info, &ImportData_CoSimIO);

    info.Set<std::string>("function_name", "ExportData");
    CoSimIO::Register(info, &ExportData_CoSimIO);

    info.Set<std::string>("function_name", "ImportMesh");
    CoSimIO::Register(info, &ImportMesh_CoSimIO);

    info.Set<std::string>("function_name", "ExportMesh");
    CoSimIO::Register(info, &ExportMesh_CoSimIO);

    CoSimIO::Run(info);

    CoSimIO::Disconnect(info);
}

} // helpers namespace

int main(int argc, char **argv)
{
    // parsing input
    if (argc != 3) {
        throw std::runtime_error("Two input arguments required (level of coupling & echo-level)!");
    }

    const int level_of_coupling = std::atoi(argv[1]);
    sEchoLevel = std::atoi(argv[2]);

    Initialize();

    if (level_of_coupling == 0) {
        solver_print(1) << "Doing STANDALONE simulation" << std::endl;
        RunSolutionLoop();
    } else if (level_of_coupling == 1) {
        solver_print(1) << "Doing COUPLED simulation (weakly coupled)" << std::endl;
        RunSolutionLoopWithWeakCoupling();
    } else if (level_of_coupling == 2) {
        solver_print(1) << "Doing COUPLED simulation (strongly coupled)" << std::endl;
        RunSolutionLoopWithStrongCoupling();
    } else if (level_of_coupling == 3) {
        solver_print(1) << "Doing COUPLED simulation (orchestrated by CoSimulation)" << std::endl;
        RunSolutionCoSimulationOrchestrated();
    } else {
        throw std::runtime_error("ERROR, WRONG LEVEL OF COUPLING; CAN ONLY BE 0, 1, 2, 3, STOPPING");
    }

    solver_print(1) << "Exiting" << std::endl;

    return (0);
}

