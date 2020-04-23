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

#ifndef CO_SIM_IO_CONNECTION_H_INCLUDED
#define CO_SIM_IO_CONNECTION_H_INCLUDED

// Optional includes
#ifdef CO_SIM_IO_USING_SOCKETS
#include "co_sim_sockets_communication.h"
#endif // CO_SIM_IO_USING_SOCKETS


#ifdef CO_SIM_IO_USING_MPI
#include "co_sim_mpi_communication.h"
#endif // CO_SIM_IO_USING_MPI

// System includes
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <functional>

// Project includes
#include "co_sim_file_communication.h"

namespace CoSimIO {
namespace Internals {

class CoSimConnection
{

public:

    using FunctionPointerType = std::function<void(CoSimConnection&, const std::string&)>;

    explicit CoSimConnection(const std::string& rName, const std::string& rSettingsFileName)
    : CoSimConnection(rName, Internals::ReadSettingsFile(rSettingsFileName)) { } // forwarding constructor call

    explicit CoSimConnection(const std::string& rName, SettingsType Settings) : mConnectionName(rName)
    {
        Initialize(Settings);
    }

    Info Connect()
    {
        mpComm->Connect();
        return Info(); // TODO use this
    }

    Info Disconnect()
    {
        mpComm->Disconnect();
        return Info(); // TODO use this
    }

    Info SendControlSignal(const std::string& rIdentifier, const CoSimIO::ControlSignal Signal)
    {
        CO_SIM_IO_ERROR_IF_NOT(mIsConnectionMaster) << "This function can only be called as the Connection-Master!" << std::endl;
        mpComm->SendControlSignal(rIdentifier, Signal);
        return Info(); // TODO use this
    }

    Info Register(
        const std::string& rFunctionName,
        FunctionPointerType FunctionPointer)
    {
        CO_SIM_IO_INFO("CoSimIO") << "Registering function for: " << rFunctionName << std::endl;

        CO_SIM_IO_ERROR_IF(mIsConnectionMaster) << "This function can only be called as the Connection-Slave!" << std::endl;

        CheckIfFunctionNameIsValid(rFunctionName);

        CO_SIM_IO_ERROR_IF((mRegisteredFunctions.count(rFunctionName)>0)) << "A function was already registered for " << rFunctionName << "!" << std::endl;

        mRegisteredFunctions[rFunctionName] = FunctionPointer;
        return Info(); // TODO use this
    }

    Info Run()
    {
        CO_SIM_IO_ERROR_IF(mIsConnectionMaster) << "This function can only be called as the Connection-Slave!" << std::endl;

        CoSimIO::ControlSignal control_signal;
        std::string identifier;
        while(true) {
            control_signal = RecvControlSignal(identifier);
            //TODO check if received signal is valid
            if (control_signal == CoSimIO::ControlSignal::BreakSolutionLoop) {
                break; // coupled simulation is done
            } else {
                const std::string function_name(ControlSignalName(control_signal));
                CO_SIM_IO_ERROR_IF_NOT((mRegisteredFunctions.count(function_name)>0)) << "No function was registered for \"" << function_name << "\"!" << std::endl;
                mRegisteredFunctions.at(function_name)(*this, identifier);
            }
        }
        return Info(); // TODO use this
    }

    bool IsConverged()
    {
        std::string dummy("");
        return (RecvControlSignal(dummy) == CoSimIO::ControlSignal::ConvergenceAchieved);
    }


    template<class... Args>
    Info ImportData(Args&&... args)
    {
        mpComm->ImportData(std::forward<Args>(args)...);

        return Info(); // TODO use this
    }

    template<class... Args>
    Info ExportData(Args&&... args)
    {
        mpComm->ExportData(std::forward<Args>(args)...);

        return Info(); // TODO use this
    }

    template<class... Args>
    Info ImportMesh(Args&&... args)
    {
        mpComm->ImportMesh(std::forward<Args>(args)...);

        return Info(); // TODO use this
    }

    template<class... Args>
    Info ExportMesh(Args&&... args)
    {
        mpComm->ExportMesh(std::forward<Args>(args)...);

        return Info(); // TODO use this
    }

    template<class... Args>
    Info ImportGeometry(Args&&... args)
    {
        CO_SIM_IO_ERROR << "Importing of Geometry is not yet implemented!" << std::endl;
        mpComm->ImportGeometry(std::forward<Args>(args)...);

        return Info(); // TODO use this
    }

    template<class... Args>
    Info ExportGeometry(Args&&... args)
    {
        CO_SIM_IO_ERROR << "Exporting of Geometry is not yet implemented!" << std::endl;
        mpComm->ExportGeometry(std::forward<Args>(args)...);

        return Info(); // TODO use this
    }

private:
    std::unique_ptr<CoSimCommunication> mpComm; // handles communication (File, Sockets, MPI, ...)

    std::string mConnectionName;

    bool mIsConnectionMaster = false;

    std::unordered_map<std::string, FunctionPointerType> mRegisteredFunctions;

    void Initialize(SettingsType& rSettings)
    {
        std::string comm_format("file"); // default is file-communication
        if (rSettings.count("communication_format") != 0) { // communication format has been specified
            comm_format = rSettings.at("communication_format");
        }

        if (rSettings.count("is_connection_master") != 0) { // is_connection_master has been specified
            mIsConnectionMaster = (rSettings.at("is_connection_master") == "1");
        }

        CO_SIM_IO_INFO("CoSimIO") << "CoSimIO for \"" << mConnectionName << "\" uses communication format: " << comm_format << std::endl;

        if (comm_format == "file") {
            mpComm = std::unique_ptr<CoSimCommunication>(new CoSimFileCommunication(mConnectionName, rSettings, mIsConnectionMaster));
        } else if (comm_format == "sockets") {
            #ifdef CO_SIM_IO_USING_SOCKETS
            mpComm = std::unique_ptr<CoSimCommunication>(new CoSimSocketsCommunication(mConnectionName, rSettings, mIsConnectionMaster));
            #else
            CO_SIM_IO_ERROR << "Support for Sockets was not compiled!" << std::endl;
            #endif // CO_SIM_IO_USING_SOCKETS
        } else if (comm_format == "mpi") {
            #ifdef CO_SIM_IO_USING_MPI
            mpComm = std::unique_ptr<CoSimCommunication>(new CoSimMPICommunication(mConnectionName, rSettings, mIsConnectionMaster));
            #else
            CO_SIM_IO_ERROR << "Support for MPI was not compiled!" << std::endl;
            #endif // CO_SIM_IO_USING_MPI
        } else {
            CO_SIM_IO_ERROR << "Unsupported communication format: " << comm_format << std::endl;
        }
    }

    CoSimIO::ControlSignal RecvControlSignal(std::string& rIdentifier)
    {
        CO_SIM_IO_ERROR_IF(mIsConnectionMaster) << "This function can only be called as the Connection-Slave!" << std::endl;
        return mpComm->RecvControlSignal(rIdentifier);
    }

    void CheckIfFunctionNameIsValid(const std::string rFunctionName) const
    {
        // could use set but that would require another include just for this
        const static std::vector<std::string> allowed_function_names {
            "AdvanceInTime",
            "InitializeSolutionStep",
            "Predict",
            "SolveSolutionStep",
            "FinalizeSolutionStep",
            "OutputSolutionStep",
            // "ImportGeometry", // not yet implemented
            // "ExportGeometry", // not yet implemented
            "ImportMesh",
            "ExportMesh",
            "ImportData",
            "ExportData"
        };

        CO_SIM_IO_ERROR_IF(std::find(allowed_function_names.begin(), allowed_function_names.end(), rFunctionName) == allowed_function_names.end()) << "The function name \"" << rFunctionName << "\" is not allowed!\nOnly the following names are allowed:\n"; // TODO print the names
    }

    std::string ControlSignalName(const ControlSignal Signal) const
    {
        switch (Signal) {
            // first two should not be needed here, this is intended to be used in "Run"
            // case ControlSignal::Dummy:                  return "Dummy";
            // case ControlSignal::ConvergenceAchieved:    return "ConvergenceAchieved";
            case ControlSignal::BreakSolutionLoop:      return "BreakSolutionLoop";
            case ControlSignal::AdvanceInTime:          return "AdvanceInTime";
            case ControlSignal::InitializeSolutionStep: return "InitializeSolutionStep";
            case ControlSignal::Predict:                return "Predict";
            case ControlSignal::SolveSolutionStep:      return "SolveSolutionStep";
            case ControlSignal::FinalizeSolutionStep:   return "FinalizeSolutionStep";
            case ControlSignal::OutputSolutionStep:     return "OutputSolutionStep";
            case ControlSignal::ImportGeometry:         return "ImportGeometry";
            case ControlSignal::ExportGeometry:         return "ExportGeometry";
            case ControlSignal::ImportMesh:             return "ImportMesh";
            case ControlSignal::ExportMesh:             return "ExportMesh";
            case ControlSignal::ImportData:             return "ImportData";
            case ControlSignal::ExportData:             return "ExportData";
            default: CO_SIM_IO_ERROR << "Signal is unknown: " << static_cast<int>(Signal); return "";
        }
    }

}; // class CoSimConnection

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_CONNECTION_H_INCLUDED
