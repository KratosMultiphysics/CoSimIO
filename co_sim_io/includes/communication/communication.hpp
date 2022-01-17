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
#include <utility>
#include <tuple>

// Project includes
#include "includes/info.hpp"
#include "includes/data_container.hpp"
#include "includes/model_part.hpp"
#include "includes/data_communicator.hpp"
#include "includes/filesystem_inc.hpp"
#include "includes/utilities.hpp"

namespace CoSimIO {
namespace Internals {


class CO_SIM_IO_API Communication
{
public:
    Communication(
        const Info& I_Settings,
        std::shared_ptr<DataCommunicator> I_DataComm);

    // might throw when trying to remove files!
    virtual ~Communication() noexcept(false) {}; // impl of disconnect has to be in derived class due to order of class destruction

    Info Connect(const Info& I_Info);

    Info Disconnect(const Info& I_Info);

    template<class... Args>
    Info ExportInfo(Args&&... args)
    {
        const Info i_info = std::get<0>(std::forward_as_tuple(args...));

        CheckConnection(i_info);

        Info o_info = ExportInfoImpl(std::forward<Args>(args)...);

        PrintElapsedTime(i_info, o_info, "Export info");

        return o_info;
    }

    template<class... Args>
    Info ImportInfo(Args&&... args)
    {
        const Info i_info = std::get<0>(std::forward_as_tuple(args...));

        CheckConnection(i_info);

        Info o_info = ImportInfoImpl(std::forward<Args>(args)...);

        PrintElapsedTime(i_info, o_info, "Import info");

        return o_info;
    }

    template<class... Args>
    Info ImportData(Args&&... args)
    {
        const Info i_info = std::get<0>(std::forward_as_tuple(args...));

        CheckConnection(i_info);

        Info o_info = ImportDataImpl(std::forward<Args>(args)...);

        PrintElapsedTime(i_info, o_info, "Import data");

        return o_info;
    }

    template<class... Args>
    Info ExportData(Args&&... args)
    {
        const Info i_info = std::get<0>(std::forward_as_tuple(args...));

        CheckConnection(i_info);

        Info o_info = ExportDataImpl(std::forward<Args>(args)...);

        PrintElapsedTime(i_info, o_info, "Export data");

        return o_info;
    }

    template<class... Args>
    Info ImportMesh(Args&&... args)
    {
        const Info i_info = std::get<0>(std::forward_as_tuple(args...));

        CheckConnection(i_info);

        Info o_info = ImportMeshImpl(std::forward<Args>(args)...);

        PrintElapsedTime(i_info, o_info, "Import mesh");

        return o_info;
    }

    template<class... Args>
    Info ExportMesh(Args&&... args)
    {
        const Info i_info = std::get<0>(std::forward_as_tuple(args...));

        CheckConnection(i_info);

        Info o_info = ExportMeshImpl(std::forward<Args>(args)...);

        PrintElapsedTime(i_info, o_info, "Export mesh");

        return o_info;
    }

protected:
    std::string GetConnectionName() const      {return mConnectionName;}
    fs::path GetWorkingDirectory() const       {return mWorkingDirectory;}
    fs::path GetCommunicationDirectory() const {return mCommFolder;}
    int GetEchoLevel() const                   {return mEchoLevel;}
    bool GetIsPrimaryConnection() const        {return mIsPrimaryConnection;}
    bool GetPrintTiming() const                {return mPrintTiming;}
    bool GetIsConnected() const                {return mIsConnected;}
    const DataCommunicator& GetDataCommunicator() const                {return *mpDataComm;}

    Info GetMyInfo() const;
    Info GetPartnerInfo() const {return mPartnerInfo;};

    fs::path GetTempFileName(const fs::path& rPath) const;

    fs::path GetFileName(const fs::path& rPath, const std::string& rExtension) const;

    fs::path GetFileName(const fs::path& rPath, const int Rank, const std::string& rExtension) const;

    void WaitForPath(const fs::path& rPath) const;

    void WaitUntilFileIsRemoved(const fs::path& rPath) const;

    void MakeFileVisible(const fs::path& rPath) const;

    void RemovePath(const fs::path& rPath) const;

    void SynchronizeAll(const std::string& rTag) const;

    template<class TObjectType>
    double SendObjectWithStreamSerializer(
        const Info& I_Info,
        const TObjectType& rObject)
    {
        CO_SIM_IO_TRY

        const auto start_time(std::chrono::steady_clock::now());
        StreamSerializer serializer;
        serializer.save("object", rObject);
        const double elapsed_time_save = Utilities::ElapsedSeconds(start_time);

        const double elapsed_time_write = SendString(I_Info, serializer.GetStringRepresentation());
        return elapsed_time_save + elapsed_time_write;

        CO_SIM_IO_CATCH
    }

    template<class TObjectType>
    double ReceiveObjectWithStreamSerializer(
        const Info& I_Info,
        TObjectType& rObject)
    {
        CO_SIM_IO_TRY

        std::string buffer;
        const double elapsed_time_read = ReceiveString(I_Info, buffer);

        const auto start_time(std::chrono::steady_clock::now());
        StreamSerializer serializer(buffer);
        serializer.load("object", rObject);
        const double elapsed_time_load = Utilities::ElapsedSeconds(start_time);

        return elapsed_time_read+elapsed_time_load;

        CO_SIM_IO_CATCH
    }

    virtual double SendString(
        const Info& I_Info,
        const std::string& rData) = 0;

    virtual double ReceiveString(
        const Info& I_Info,
        std::string& rData) = 0;

    virtual double SendDataContainer(
        const Info& I_Info,
        const Internals::DataContainer<double>& rData) = 0;

    virtual double ReceiveDataContainer(
        const Info& I_Info,
        Internals::DataContainer<double>& rData) = 0;

private:
    std::shared_ptr<DataCommunicator> mpDataComm;

    std::string mConnectionName;
    std::string mMyName;
    std::string mConnectTo;

    Info mPartnerInfo;

    fs::path mCommFolder;
    bool mCommInFolder = true;
    bool mUseAuxFileForFileAvailability = false;

    fs::path mWorkingDirectory;
    int mEchoLevel = 1;
    bool mIsPrimaryConnection;
    bool mPrimaryWasExplicitlySpecified;
    bool mPrintTiming = false;
    bool mIsConnected = false;

    void CheckConnection(const Info& I_Info);
    virtual std::string GetCommunicationName() const = 0;
    virtual Info GetCommunicationSettings() const {return Info();}

    virtual void BaseConnectDetail(const Info& I_Info);
    virtual void BaseDisconnectDetail(const Info& I_Info);
    virtual void PrepareConnection(const Info& I_Info){}
    virtual Info ConnectDetail(const Info& I_Info){return Info();}
    virtual Info DisconnectDetail(const Info& I_Info){return Info();}

    virtual Info ImportInfoImpl(const Info& I_Info);

    virtual Info ExportInfoImpl(const Info& I_Info);

    virtual Info ImportDataImpl(
        const Info& I_Info,
        Internals::DataContainer<double>& rData);

    virtual Info ExportDataImpl(
        const Info& I_Info,
        const Internals::DataContainer<double>& rData);

    virtual Info ImportMeshImpl(
        const Info& I_Info,
        ModelPart& O_ModelPart);

    virtual Info ExportMeshImpl(
        const Info& I_Info,
        const ModelPart& I_ModelPart);

    void HandShake(const Info& I_Info);

    virtual void DerivedHandShake() const {};

    void PrintElapsedTime(
        const Info& I_Info,
        const Info& O_Info,
        const std::string& rLabel);
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_COMMUNICATION_INCLUDED
