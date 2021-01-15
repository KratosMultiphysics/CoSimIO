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

#ifndef CO_SIM_IO_COMMUNICATION_INCLUDED
#define CO_SIM_IO_COMMUNICATION_INCLUDED

// System includes
#include <unordered_map>

// Project includes
#include "../info.hpp"
#include "../data_container.hpp"
#include "../model_part.hpp"
#include "../filesystem_inc.hpp"
#include "../utilities.hpp"

namespace CoSimIO {
namespace Internals {


class Communication
{
public:
    explicit Communication(const Info& I_Settings)
    {
        mMyName = I_Settings.Get<std::string>("my_name");
        mConnectTo = I_Settings.Get<std::string>("connect_to");
        if (I_Settings.Has("is_primary_connection")) {
            mIsPrimaryConnection = I_Settings.Get<bool>("is_primary_connection");
        } else {
            // automatically determine the primary connection in case the user didn't specify it
            mIsPrimaryConnection = mMyName < mConnectTo;
        }
        mConnectionName = CreateConnectionName(mMyName, mConnectTo);

        mWorkingDirectory = I_Settings.Get<std::string>("working_directory", fs::current_path());

        mEchoLevel = I_Settings.Get<int>("echo_level", 0);
        mPrintTiming = I_Settings.Get<bool>("print_timing", false);
    }

    virtual ~Communication() = default; // impl of disconnect has to be in derived class due to order of class destruction

    Info Connect(const Info& I_Info)
    {
        CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>0)
            << "Establishing connection for \"" << mConnectionName
            << "\"\n    from: \"" << mMyName
            << "\"\n    to:   \"" << mConnectTo
            << "\"\n    as " << (mIsPrimaryConnection ? "PRIMARY" : "SECONDARY")
            << " connection; working directory: " << mWorkingDirectory << " ..." << std::endl;

        CO_SIM_IO_ERROR_IF(mIsConnected) << "A connection was already established!" << std::endl;

        Info connect_detail_info = ConnectDetail(I_Info);
        mIsConnected = connect_detail_info.Get<bool>("is_connected");
        connect_detail_info.Set<int>("connection_status", ConnectionStatus::Connected);
        connect_detail_info.Set<std::string>("working_directory", mWorkingDirectory);

        CO_SIM_IO_ERROR_IF_NOT(mIsConnected) << "Connection was not successful!" << std::endl;

        CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>0) << "Connection established" << std::endl;

        return connect_detail_info;
    }

    Info Disconnect(const Info& I_Info)
    {
        CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>0) << "Disconnecting \"" << mConnectionName << "\" ..." << std::endl;

        if (mIsConnected) {
            Info disconnect_detail_info = DisconnectDetail(I_Info);
            mIsConnected = disconnect_detail_info.Get<bool>("is_connected");

            if (mIsConnected) {
                CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not successful!" << std::endl;
                disconnect_detail_info.Set<int>("connection_status", ConnectionStatus::DisconnectionError);
            } else {
                CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>0) << "Disconnecting successful" << std::endl;
                disconnect_detail_info.Set<int>("connection_status", ConnectionStatus::Disconnected);
            }
            return disconnect_detail_info;

        } else {
            CO_SIM_IO_INFO("CoSimIO") << "Warning: Calling Disconnect but there was no active connection!" << std::endl;
            Info disconnect_info;
            disconnect_info.Set<bool>("is_connected", false);
            disconnect_info.Set<int>("connection_status", ConnectionStatus::DisconnectionError);
            return disconnect_info;
        }
    }

    /*[[deprecated]]*/ bool Connect()
    {
        Info temp_info;
        this->Connect(temp_info);
        return mIsConnected;
    }

    /*[[deprecated]]*/ bool Disconnect()
    {
        Info temp_info;
        this->Disconnect(temp_info);
        return mIsConnected;
    }

    template<class... Args>
    Info ExportInfo(Args&&... args)
    {
        CheckConnection(); return ExportInfoImpl(std::forward<Args>(args)...);
    }

    template<class... Args>
    Info ImportInfo(Args&&... args)
    {
        CheckConnection(); return ImportInfoImpl(std::forward<Args>(args)...);
    }

    template<class... Args>
    Info ImportData(Args&&... args)
    {
        CheckConnection(); return ImportDataImpl(std::forward<Args>(args)...);
    }

    template<class... Args>
    Info ExportData(Args&&... args)
    {
        CheckConnection(); return ExportDataImpl(std::forward<Args>(args)...);
    }

    template<class... Args>
    Info ImportMesh_1X2(Args&&... args) // TODO fix name once other interface is removed
    {
        CheckConnection(); return ImportMeshImpl(std::forward<Args>(args)...);
    }

    template<class... Args>
    Info ExportMesh_1X2(Args&&... args) // TODO fix name once other interface is removed
    {
        CheckConnection(); return ExportMeshImpl(std::forward<Args>(args)...);
    }


    // old interface, functions need to be removed before release
    /*[[deprecated]]*/ void SendControlSignal(const std::string& rIdentifier, const CoSimIO::ControlSignal Signal)
    {
        CheckConnection(); SendControlSignalDetail(rIdentifier, Signal);
    }
    /*[[deprecated]]*/ CoSimIO::ControlSignal RecvControlSignal(std::string& rIdentifier)
    {
        CheckConnection(); return RecvControlSignalDetail(rIdentifier);
    }

    template<class... Args>
    /*[[deprecated]]*/ Info ImportMesh(Args&&... args)
    {
        CheckConnection(); ImportMeshImpl(std::forward<Args>(args)...);
        return Info();
    }

    template<class... Args>
    /*[[deprecated]]*/ Info ExportMesh(Args&&... args)
    {
        CheckConnection(); ExportMeshImpl(std::forward<Args>(args)...);
        return Info();
    }

protected:
    std::string GetConnectionName() const {return mConnectionName;}
    fs::path GetWorkingDirectory() const  {return mWorkingDirectory;}
    int GetEchoLevel() const              {return mEchoLevel;}
    bool GetIsConnectionMaster() const    {return mIsConnectionMaster;} // TODO remove
    bool GetIsPrimaryConnection() const   {return mIsPrimaryConnection;}
    bool GetPrintTiming() const           {return mPrintTiming;}
    bool GetIsConnected() const           {return mIsConnected;}

private:
    std::string mConnectionName;
    std::string mMyName;
    std::string mConnectTo;
    fs::path mWorkingDirectory;
    int mEchoLevel = 1;
    bool mIsConnectionMaster = false; // TODO remove
    bool mIsPrimaryConnection;
    bool mPrintTiming = false;
    bool mIsConnected = false;

    void CheckConnection()
    {
        CO_SIM_IO_ERROR_IF_NOT(mIsConnected) << "No active connection exists!" << std::endl;;
    }

    // new interface, functions return Info
    virtual Info ConnectDetail(const Info& I_Info) = 0;
    virtual Info DisconnectDetail(const Info& I_Info) = 0;

    virtual Info ImportInfoImpl(const Info& I_Info)
    {
        CO_SIM_IO_ERROR << "ImportInfo not implemented for this comm-type" << std::endl;
        return Info();
    }

    virtual Info ExportInfoImpl(const Info& I_Info)
    {
        CO_SIM_IO_ERROR << "ExportInfo not implemented for this comm-type" << std::endl;
        return Info();
    }

    virtual Info ImportDataImpl(
        const Info& I_Info,
        Internals::DataContainer<double>& rData)
    {
        CO_SIM_IO_ERROR << "ImportDataImpl not implemented for this comm-type!" << std::endl;
        return Info();
    }

    virtual Info ExportDataImpl(
        const Info& I_Info,
        const Internals::DataContainer<double>& rData)
    {
        CO_SIM_IO_ERROR << "ExportDataImpl not implemented for this comm-type!" << std::endl;
        return Info();
    }

    virtual Info ImportMeshImpl(
        const Info& I_Info,
        ModelPart& O_ModelPart)
    {
        CO_SIM_IO_ERROR << "ImportMeshImpl not implemented for this comm-type!" << std::endl;
        return Info();
    }

    virtual Info ExportMeshImpl(
        const Info& I_Info,
        const ModelPart& I_ModelPart)
    {
        CO_SIM_IO_ERROR << "ExportMeshImpl not implemented for this comm-type!" << std::endl;
        return Info();
    }

    // old interface, functions need to be removed before release
    virtual /*[[deprecated]]*/ void SendControlSignalDetail(const std::string& rIdentifier, CoSimIO::ControlSignal Signal)
    {
        CO_SIM_IO_ERROR << "SendControlSignalDetail not implemented for this comm-type" << std::endl;
    }
    virtual /*[[deprecated]]*/ CoSimIO::ControlSignal RecvControlSignalDetail(std::string& rIdentifier)
    {
        CO_SIM_IO_ERROR << "RecvControlSignalDetail not implemented for this comm-type" << std::endl;
        return CoSimIO::ControlSignal::Dummy;
    }

    virtual /*[[deprecated]]*/ void ImportMeshImpl(
        const std::string& rIdentifier,
        CoSimIO::Internals::DataContainer<double>& rNodalCoordinates,
        CoSimIO::Internals::DataContainer<int>& rElementConnectivities,
        CoSimIO::Internals::DataContainer<int>& rElementTypes)
    {
        CO_SIM_IO_ERROR << "ImportDataImpl not implemented for this comm-type!" << std::endl;
    }

    virtual /*[[deprecated]]*/ void ExportMeshImpl(
        const std::string& rIdentifier,
        const CoSimIO::Internals::DataContainer<double>& rNodalCoordinates,
        const CoSimIO::Internals::DataContainer<int>& rElementConnectivities,
        const CoSimIO::Internals::DataContainer<int>& rElementTypes)
    {
        CO_SIM_IO_ERROR << "ImportDataImpl not implemented for this comm-type!" << std::endl;
    }
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_COMMUNICATION_INCLUDED
