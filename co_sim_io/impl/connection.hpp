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

#ifndef CO_SIM_IO_CONNECTION_INCLUDED
#define CO_SIM_IO_CONNECTION_INCLUDED

// Optional includes
#ifdef CO_SIM_IO_USING_SOCKETS
#include "communication/sockets_communication.hpp"
#endif // CO_SIM_IO_USING_SOCKETS


#ifdef CO_SIM_IO_USING_MPI
#include "communication/mpi_communication.hpp"
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
#include "communication/file_communication.hpp"

namespace CoSimIO {
namespace Internals {

class Connection
{

public:

    using FunctionPointerType = std::function<Info(const Info&)>;

    explicit Connection(const Info& I_Settings)
    {
        Initialize(I_Settings);
    }

    Info Connect(const Info& I_Info)
    {
        Info info = mpComm->Connect(I_Info);
        info.Set<int>("connection_status", ConnectionStatus::Connected);
        return info;
    }

    Info Disconnect(const Info& I_Info)
    {
        Info info = mpComm->Disconnect(I_Info);
        info.Set<int>("connection_status", ConnectionStatus::Disconnected);
        return info;
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

    Info Run(const Info& I_Info)
    {
        CO_SIM_IO_ERROR << "This function is currently not working!!!" << std::endl;
        CO_SIM_IO_ERROR_IF(mIsConnectionMaster) << "This function can only be called as the Connection-Slave!" << std::endl;

        // CoSimIO::ControlSignal control_signal;
        // std::string identifier;
        // while(true) {
        //     // TODO replace with ImportInfo
        //     // control_signal = RecvControlSignal(identifier);
        //     //TODO check if received signal is valid
        //     if (control_signal == CoSimIO::ControlSignal::BreakSolutionLoop) {
        //         break; // coupled simulation is done
        //     } else {
        //         const std::string function_name(ControlSignalName(control_signal));
        //         CO_SIM_IO_ERROR_IF_NOT((mRegisteredFunctions.count(function_name)>0)) << "No function was registered for \"" << function_name << "\"!" << std::endl;
        //         Info info;
        //         mRegisteredFunctions.at(function_name)(info);
        //     }
        // }
        return Info(); // TODO use this
    }


    template<class... Args>
    Info ImportInfo(Args&&... args)
    {
        return mpComm->ImportInfo(std::forward<Args>(args)...);
    }

    template<class... Args>
    Info ExportInfo(Args&&... args)
    {
        return mpComm->ExportInfo(std::forward<Args>(args)...);
    }
    template<class... Args>
    Info ImportData(Args&&... args)
    {
        return mpComm->ImportData(std::forward<Args>(args)...);
    }

    template<class... Args>
    Info ExportData(Args&&... args)
    {
        return mpComm->ExportData(std::forward<Args>(args)...);
    }

    template<class... Args>
    Info ImportMesh(Args&&... args)
    {
        return mpComm->ImportMesh(std::forward<Args>(args)...);
    }

    template<class... Args>
    Info ExportMesh(Args&&... args)
    {
        return mpComm->ExportMesh(std::forward<Args>(args)...);
    }

private:
    std::unique_ptr<Communication> mpComm; // handles communication (File, Sockets, MPI, ...)

    bool mIsConnectionMaster = false;

    std::unordered_map<std::string, FunctionPointerType> mRegisteredFunctions;

    void Initialize(const Info& I_Settings)
    {
        const std::string comm_format = I_Settings.Get<std::string>("communication_format", "file"); // default is file-communication

        CO_SIM_IO_INFO("CoSimIO") << "CoSimIO from \"" << I_Settings.Get<std::string>("my_name") << "\" to \"" << I_Settings.Get<std::string>("connect_to") << "\" uses communication format: " << comm_format << std::endl;

        if (comm_format == "file") {
            mpComm = CoSimIO::make_unique<FileCommunication>(I_Settings);
        } else if (comm_format == "sockets") {
            #ifdef CO_SIM_IO_USING_SOCKETS
            mpComm = CoSimIO::make_unique<SocketsCommunication>(I_Settings);
            #else
            CO_SIM_IO_ERROR << "Support for Sockets was not compiled!" << std::endl;
            #endif // CO_SIM_IO_USING_SOCKETS
        } else if (comm_format == "mpi") {
            #ifdef CO_SIM_IO_USING_MPI
            mpComm = CoSimIO::make_unique<MPICommunication>(I_Settings);
            #else
            CO_SIM_IO_ERROR << "Support for MPI was not compiled!" << std::endl;
            #endif // CO_SIM_IO_USING_MPI
        } else {
            CO_SIM_IO_ERROR << "Unsupported communication format: " << comm_format << std::endl;
        }
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
            case ControlSignal::ImportMesh:             return "ImportMesh";
            case ControlSignal::ExportMesh:             return "ExportMesh";
            case ControlSignal::ImportData:             return "ImportData";
            case ControlSignal::ExportData:             return "ExportData";
            default: CO_SIM_IO_ERROR << "Signal is unknown: " << static_cast<int>(Signal); return "";
        }
    }

}; // class Connection

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_CONNECTION_INCLUDED
