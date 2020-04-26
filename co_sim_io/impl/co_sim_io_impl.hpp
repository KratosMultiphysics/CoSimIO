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

#ifndef CO_SIM_IO_IMPL_H_INCLUDED
#define CO_SIM_IO_IMPL_H_INCLUDED

/*
This file contains the implementation of the functions defined in "co_sim_io.hpp"
*/

// System includes
#include <string>
#include <memory>

// Project includes
#include "connection.hpp"

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

inline void SendControlSignal(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    const CoSimIO::ControlSignal Signal)
{
    Internals::GetConnection(rConnectionName).SendControlSignal(rIdentifier, Signal);
}

} // namespace Internals

inline Info Connect(const std::string& rConnectionName, CoSimIO::SettingsType Settings)
{
    using namespace Internals;
    CO_SIM_IO_ERROR_IF(HasIO(rConnectionName)) << "A connection for \"" << rConnectionName << "\" already exists!" << std::endl;

    s_co_sim_connections[rConnectionName] = std::unique_ptr<Connection>(new Connection(rConnectionName, Settings));
    return GetConnection(rConnectionName).Connect();
}

inline Info Connect(const std::string& rConnectionName, const std::string& rSettingsFileName)
{
    Connect(rConnectionName, Internals::ReadSettingsFile(rSettingsFileName)); // forward call to funciton above

    return Info(); // TODO use this
}

inline Info Disconnect(const std::string& rConnectionName)
{
    using namespace Internals;
    CO_SIM_IO_ERROR_IF_NOT(HasIO(rConnectionName)) << "Trying to disconnect connection \"" << rConnectionName << "\" which does not exist!" << std::endl;

    auto info = GetConnection(rConnectionName).Disconnect();
    s_co_sim_connections.erase(rConnectionName);

    return Info(); // TODO use this
}

// Version for C++, there this input is a std::vector, which we have to wrap before passing it on
template<>
inline Info ImportData(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    std::vector<double>& rData)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerStdVector<double>(rData));
    return GetConnection(rConnectionName).ImportData(rIdentifier, *p_container);
}

// Version for C and fortran, there we already get a container
template<>
inline Info ImportData(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    CoSimIO::Internals::DataContainer<double>& rData)
{
    return Internals::GetConnection(rConnectionName).ImportData(rIdentifier, rData);
}

// Version for C++, there this input is a std::vector, which we have to wrap before passing it on
template<>
inline Info ExportData(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    const std::vector<double>& rData)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerStdVectorReadOnly<double>(rData));
    return GetConnection(rConnectionName).ExportData(rIdentifier, *p_container);
}

// Version for C and fortran, there we already get a container
template<>
inline Info ExportData(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    const CoSimIO::Internals::DataContainer<double>& rData)
{
    return Internals::GetConnection(rConnectionName).ExportData(rIdentifier, rData);
}

template<>
inline Info ImportMesh(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    std::vector<double>& rNodalCoordinates,
    std::vector<int>& rElementConnectivities,
    std::vector<int>& rElementTypes)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerStdVector<double>(rNodalCoordinates));
    std::unique_ptr<DataContainer<int>> p_container_conn(new DataContainerStdVector<int>(rElementConnectivities));
    std::unique_ptr<DataContainer<int>> p_container_types(new DataContainerStdVector<int>(rElementTypes));
    return Internals::GetConnection(rConnectionName).ImportMesh(rIdentifier, *p_container_coords, *p_container_conn, *p_container_types);
}

template<>
inline Info ImportMesh(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    CoSimIO::Internals::DataContainer<double>& rNodalCoordinates,
    CoSimIO::Internals::DataContainer<int>& rElementConnectivities,
    CoSimIO::Internals::DataContainer<int>& rElementTypes)
{
    return Internals::GetConnection(rConnectionName).ImportMesh(rIdentifier, rNodalCoordinates, rElementConnectivities, rElementTypes);
}

template<>
inline Info ExportMesh(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    const std::vector<double>& rNodalCoordinates,
    const std::vector<int>& rElementConnectivities,
    const std::vector<int>& rElementTypes)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerStdVectorReadOnly<double>(rNodalCoordinates));
    std::unique_ptr<DataContainer<int>> p_container_conn(new DataContainerStdVectorReadOnly<int>(rElementConnectivities));
    std::unique_ptr<DataContainer<int>> p_container_types(new DataContainerStdVectorReadOnly<int>(rElementTypes));
    return Internals::GetConnection(rConnectionName).ExportMesh(rIdentifier, *p_container_coords, *p_container_conn, *p_container_types);
}

template<>
inline Info ExportMesh(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    const CoSimIO::Internals::DataContainer<double>& rNodalCoordinates,
    const CoSimIO::Internals::DataContainer<int>& rElementConnectivities,
    const CoSimIO::Internals::DataContainer<int>& rElementTypes)
{
    return Internals::GetConnection(rConnectionName).ExportMesh(rIdentifier, rNodalCoordinates, rElementConnectivities, rElementTypes);
}

inline Info ImportInfo(
    const std::string& rConnectionName,
    Info& rInfo)
{
    // Internals::GetConnection(rConnectionName).ImportInfo(rInfo);
    return Info(); // TODO use this
}

inline Info ExportInfo(
    const std::string& rConnectionName,
    const Info& rInfo)
{
    // Internals::GetConnection(rConnectionName).ExportInfo(rInfo);
    return Info(); // TODO use this
}

inline int IsConverged(const std::string& rConnectionName)
{
    return Internals::GetConnection(rConnectionName).IsConverged();
}

inline Info Run(const std::string& rConnectionName)
{
    return Internals::GetConnection(rConnectionName).Run();
}


template<>
inline Info Register(
    const std::string& rConnectionName,
    const std::string& rFunctionName,
    std::function<void(Info&)> FunctionPointer)
{
    using namespace CoSimIO::Internals;

    auto fct_callback = [FunctionPointer](Info& IO_Info)
    {
        FunctionPointer(IO_Info);
    };

    // Internals::GetConnection(rConnectionName).Register(rFunctionName, fct_callback);

    return Info(); // TODO use this
}

template<>
inline Info Register(
    const std::string& rConnectionName,
    const std::string& rFunctionName,
    void (*pFunctionPointer)(Info&))
{
    using namespace CoSimIO::Internals;

    auto fct_callback = [pFunctionPointer](Info& IO_Info)
    {
        pFunctionPointer(IO_Info);
    };

    // Internals::GetConnection(rConnectionName).Register(rFunctionName, fct_callback);

    return Info(); // TODO use this
}

} // namespace CoSimIO

#endif // CO_SIM_IO_IMPL_H_INCLUDED
