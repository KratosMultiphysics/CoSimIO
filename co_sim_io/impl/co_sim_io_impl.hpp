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

#ifndef CO_SIM_IO_IMPL_INCLUDED
#define CO_SIM_IO_IMPL_INCLUDED

/*
This file contains the implementation of the functions defined in "co_sim_io.hpp"
*/

// System includes
#include <string>
#include <memory>

// Project includes
#include "connection.hpp"
#include "utilities.hpp"
#include "version.hpp"

namespace CoSimIO {

namespace Internals {
// TODO make sure this is unique even across compilation units (test somehow)
static std::unordered_map<std::string, std::unique_ptr<Connection>> s_co_sim_connections;

static bool HasIO(const std::string& rConnectionName)
{
    return s_co_sim_connections.find(rConnectionName) != s_co_sim_connections.end();
}

static Connection& GetConnection(const std::string& rConnectionName)
{
    CO_SIM_IO_ERROR_IF_NOT(HasIO(rConnectionName)) << "Trying to use connection \"" << rConnectionName << "\" which does not exist!" << std::endl;
    return *s_co_sim_connections.at(rConnectionName);
}

} // namespace Internals

inline Info Hello()
{
    std::cout << "Hello, this is the CoSimIO\n";
    std::cout << "The detached interface for coupled simulations together with the\n";
    std::cout << "CoSimulationApplication of KratosMultiphysics\n\"https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication\"\n";
    std::cout << "Version:\n";
    std::cout << "    Major: " << GetMajorVersion() << "\n";
    std::cout << "    Minor: " << GetMinorVersion() << "\n";
    std::cout << "    Patch: " << GetPatchVersion() << "\n";
    std::cout << "For more information please visit \"https://github.com/KratosMultiphysics/CoSimIO\"" << std::endl;

    Info info;
    info.Set("major_version", GetMajorVersion());
    info.Set("minor_version", GetMinorVersion());
    info.Set("patch_version", GetPatchVersion());
    // TODO maybe add more things too here

    return info;
}


inline Info Connect(const Info& I_Settings)
{
    using namespace Internals;
    const std::string my_name = I_Settings.Get<std::string>("my_name");
    const std::string connect_to = I_Settings.Get<std::string>("connect_to");
    CO_SIM_IO_ERROR_IF(my_name == connect_to) << "Connecting to self is not allowed!" << std::endl;

    const std::string connection_name = CreateConnectionName(my_name, connect_to);

    CO_SIM_IO_ERROR_IF(HasIO(connection_name)) << "A connection from \"" << my_name << "\" to \"" << connect_to << "\"already exists!" << std::endl;

    s_co_sim_connections[connection_name] = std::unique_ptr<Connection>(new Connection(I_Settings));

    auto info = GetConnection(connection_name).Connect();
    info.Set<std::string>("connection_name", connection_name);

    return info;
}

inline Info Disconnect(const Info& I_Info)
{
    using namespace Internals;
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    CO_SIM_IO_ERROR_IF_NOT(HasIO(connection_name)) << "Trying to disconnect connection \"" << connection_name << "\" which does not exist!" << std::endl;

    auto info = GetConnection(connection_name).Disconnect();
    s_co_sim_connections.erase(connection_name);

    return info;
}

// Version for C++, there this input is a std::vector, which we have to wrap before passing it on
template<>
inline Info ImportData(
    const Info& I_Info,
    std::vector<double>& rData)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerStdVector<double>(rData));
    return GetConnection(connection_name).ImportData(I_Info, *p_container);
}

// Version for C and fortran, there we already get a container
template<>
inline Info ImportData(
    const Info& I_Info,
    CoSimIO::Internals::DataContainer<double>& rData)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    return Internals::GetConnection(connection_name).ImportData(I_Info, rData);
}

// Version for C++, there this input is a std::vector, which we have to wrap before passing it on
template<>
inline Info ExportData(
    const Info& I_Info,
    const std::vector<double>& rData)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerStdVectorReadOnly<double>(rData));
    return GetConnection(connection_name).ExportData(I_Info, *p_container);
}

// Version for C and fortran, there we already get a container
template<>
inline Info ExportData(
    const Info& I_Info,
    const CoSimIO::Internals::DataContainer<double>& rData)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    return Internals::GetConnection(connection_name).ExportData(I_Info, rData);
}

template<>
inline Info ImportMesh(
    const Info& I_Info,
    std::vector<double>& rNodalCoordinates,
    std::vector<int>& rElementConnectivities,
    std::vector<int>& rElementTypes)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    const std::string identifier = I_Info.Get<std::string>("identifier");
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerStdVector<double>(rNodalCoordinates));
    std::unique_ptr<DataContainer<int>> p_container_conn(new DataContainerStdVector<int>(rElementConnectivities));
    std::unique_ptr<DataContainer<int>> p_container_types(new DataContainerStdVector<int>(rElementTypes));
    // TODO maybe pass the Info into the function (only changes under the hood necessary)
    return Internals::GetConnection(connection_name).ImportMesh(identifier, *p_container_coords, *p_container_conn, *p_container_types);
}

template<>
inline Info ImportMesh(
    const Info& I_Info,
    CoSimIO::Internals::DataContainer<double>& rNodalCoordinates,
    CoSimIO::Internals::DataContainer<int>& rElementConnectivities,
    CoSimIO::Internals::DataContainer<int>& rElementTypes)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    const std::string identifier = I_Info.Get<std::string>("identifier");
    // TODO maybe pass the Info into the function (only changes under the hood necessary)
    return Internals::GetConnection(connection_name).ImportMesh(identifier, rNodalCoordinates, rElementConnectivities, rElementTypes);
}

template<>
inline Info ExportMesh(
    const Info& I_Info,
    const std::vector<double>& rNodalCoordinates,
    const std::vector<int>& rElementConnectivities,
    const std::vector<int>& rElementTypes)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    const std::string identifier = I_Info.Get<std::string>("identifier");
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerStdVectorReadOnly<double>(rNodalCoordinates));
    std::unique_ptr<DataContainer<int>> p_container_conn(new DataContainerStdVectorReadOnly<int>(rElementConnectivities));
    std::unique_ptr<DataContainer<int>> p_container_types(new DataContainerStdVectorReadOnly<int>(rElementTypes));
    // TODO maybe pass the Info into the function (only changes under the hood necessary)
    return Internals::GetConnection(connection_name).ExportMesh(identifier, *p_container_coords, *p_container_conn, *p_container_types);
}

template<>
inline Info ExportMesh(
    const Info& I_Info,
    const CoSimIO::Internals::DataContainer<double>& rNodalCoordinates,
    const CoSimIO::Internals::DataContainer<int>& rElementConnectivities,
    const CoSimIO::Internals::DataContainer<int>& rElementTypes)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    const std::string identifier = I_Info.Get<std::string>("identifier");
    // TODO maybe pass the Info into the function (only changes under the hood necessary)
    return Internals::GetConnection(connection_name).ExportMesh(identifier, rNodalCoordinates, rElementConnectivities, rElementTypes);
}

inline Info ImportInfo(
    const Info& I_Info)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    return Internals::GetConnection(connection_name).ImportInfo(I_Info);
}

inline Info ExportInfo(
    const Info& I_Info)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    return Internals::GetConnection(connection_name).ExportInfo(I_Info);
}

inline Info Run(const Info& I_Info)
{
    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    return Internals::GetConnection(connection_name).Run();
}


template<>
inline Info Register(
    const Info& I_Info,
    std::function<Info(const Info&)> I_FunctionPointer)
{
    using namespace CoSimIO::Internals;

    auto fct_callback = [I_FunctionPointer](const Info& I_Info)
    {
        Info info = I_FunctionPointer(I_Info);
        return info;
    };

    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    const std::string function_name = I_Info.Get<std::string>("function_name");
    Internals::GetConnection(connection_name).Register(function_name, fct_callback);

    return Info(); // TODO use this
}

template<>
inline Info Register(
    const Info& I_Info,
    Info (*I_FunctionPointer)(const Info&))
{
    using namespace CoSimIO::Internals;

    auto fct_callback = [I_FunctionPointer](const Info& I_Info)
    {
        Info info = I_FunctionPointer(I_Info);
        return info;
    };

    const std::string connection_name = I_Info.Get<std::string>("connection_name");
    const std::string function_name = I_Info.Get<std::string>("function_name");
    Internals::GetConnection(connection_name).Register(function_name, fct_callback);

    return Info(); // TODO use this
}

} // namespace CoSimIO

#endif // CO_SIM_IO_IMPL_INCLUDED
