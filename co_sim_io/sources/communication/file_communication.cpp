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

// System includes
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>
#include <limits>
#include <system_error>
#include <unordered_map>

// Project includes
#include "includes/communication/file_communication.hpp"
#include "includes/utilities.hpp"
#include "includes/vtk_utilities.hpp"
#include "includes/file_serializer.hpp"

namespace CoSimIO {
namespace Internals {

namespace {

template<class TObject>
void SerializeToFile(const fs::path& rPath, const std::string& rTag, const TObject& rObject, const Serializer::TraceType SerializerTrace)
{
    CO_SIM_IO_TRY

    FileSerializer serializer(rPath.string(), SerializerTrace);
    serializer.save(rTag, rObject);

    CO_SIM_IO_CATCH
}
template<class TObject>
void SerializeFromFile(const fs::path& rPath, const std::string& rTag, TObject& rObject, const Serializer::TraceType SerializerTrace)
{
    CO_SIM_IO_TRY

    FileSerializer serializer(rPath.string(), SerializerTrace);
    serializer.load(rTag, rObject);

    CO_SIM_IO_CATCH
}

}

FileCommunication::FileCommunication(const Info& I_Settings) : Communication(I_Settings)
{
    CO_SIM_IO_TRY

    mCommFolder = GetWorkingDirectory();
    mCommFolder /= ".CoSimIOFileComm_" + GetConnectionName();
    mCommInFolder = I_Settings.Get<bool>("use_folder_for_communication", true);

    CO_SIM_IO_CATCH
}

FileCommunication::~FileCommunication()
{
    CO_SIM_IO_TRY

    if (GetIsConnected()) {
        CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
        Info tmp;
        Disconnect(tmp);
    }

    CO_SIM_IO_CATCH
}

Info FileCommunication::ConnectDetail(const Info& I_Info)
{
    CO_SIM_IO_TRY

    if (mCommInFolder) {
        if (GetIsPrimaryConnection()) {
            // delete and recreate directory to remove potential leftovers
            std::error_code ec;
            fs::remove_all(mCommFolder, ec);
            if (ec) {
                CO_SIM_IO_INFO("CoSimIO") << "Warning, communication directory (" << mCommFolder << ")could not be deleted!\nError code: " << ec.message() << std::endl;
            }
            if (!fs::exists(mCommFolder)) {
                fs::create_directory(mCommFolder);
            }
        }
    }

    ExchangeSyncFileWithPartner("connect");

    Info info;
    info.Set("is_connected", true);
    return info;

    CO_SIM_IO_CATCH
}

Info FileCommunication::DisconnectDetail(const Info& I_Info)
{
    CO_SIM_IO_TRY

    ExchangeSyncFileWithPartner("disconnect");

    if (mCommInFolder && GetIsPrimaryConnection()) {
        // delete directory to remove potential leftovers
        std::error_code ec;
        fs::remove_all(mCommFolder, ec);
        if (ec) {
            CO_SIM_IO_INFO("CoSimIO") << "Warning, communication directory (" << mCommFolder << ")could not be deleted!\nError code: " << ec.message() << std::endl;
        }
    }

    Info info;
    info.Set("is_connected", false);
    return info;

    CO_SIM_IO_CATCH
}

void FileCommunication::ExchangeSyncFileWithPartner(const std::string& rIdentifier) const
{
    CO_SIM_IO_TRY

    const fs::path file_name_primary(GetFileName("CoSimIO_primary_" + rIdentifier + "_" + GetConnectionName(), "sync"));
    const fs::path file_name_secondary(GetFileName("CoSimIO_secondary_" + rIdentifier + "_" + GetConnectionName(), "sync"));

    if (GetIsPrimaryConnection()) {
        std::ofstream sync_file;
        sync_file.open(GetTempFileName(file_name_primary));
        sync_file.close();
        CO_SIM_IO_ERROR_IF_NOT(fs::exists(GetTempFileName(file_name_primary))) << "Primary sync file " << file_name_primary << " could not be created!" << std::endl;
        MakeFileVisible(file_name_primary);

        WaitForPath(file_name_secondary);
        RemovePath(file_name_secondary);

        WaitUntilFileIsRemoved(file_name_primary);
    } else {
        WaitForPath(file_name_primary);
        RemovePath(file_name_primary);

        std::ofstream sync_file;
        sync_file.open(GetTempFileName(file_name_secondary));
        sync_file.close();
        CO_SIM_IO_ERROR_IF_NOT(fs::exists(GetTempFileName(file_name_secondary))) << "Secondary sync file " << file_name_secondary << " could not be created!" << std::endl;
        MakeFileVisible(file_name_secondary);

        WaitUntilFileIsRemoved(file_name_secondary);
    }

    CO_SIM_IO_CATCH
}

Info FileCommunication::ImportInfoImpl(const Info& I_Info)
{
    CO_SIM_IO_TRY

    const std::string identifier = I_Info.Get<std::string>("identifier");
    CheckEntry(identifier, "identifier");

    const fs::path file_name(GetFileName("CoSimIO_info_" + GetConnectionName() + "_" + identifier, "dat"));

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Attempting to import Info in file " << file_name << " ..." << std::endl;

    WaitForPath(file_name);

    Info info;
    SerializeFromFile(file_name, "info", info, Serializer::TraceType::SERIALIZER_NO_TRACE);

    RemovePath(file_name);

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Finished importing Info" << std::endl;

    return info;

    CO_SIM_IO_CATCH
}

Info FileCommunication::ExportInfoImpl(const Info& I_Info)
{
    CO_SIM_IO_TRY

    const std::string identifier = I_Info.Get<std::string>("identifier");
    CheckEntry(identifier, "identifier");

    const fs::path file_name(GetFileName("CoSimIO_info_" + GetConnectionName() + "_" + identifier, "dat"));

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Attempting to export Info in file " << file_name << " ..." << std::endl;

    WaitUntilFileIsRemoved(file_name); // TODO maybe this can be queued somehow ... => then it would not block the sender

    SerializeToFile(GetTempFileName(file_name), "info", I_Info, Serializer::TraceType::SERIALIZER_NO_TRACE);

    MakeFileVisible(file_name);

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Finished exporting Info" << std::endl;

    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info FileCommunication::ImportDataImpl(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    CO_SIM_IO_TRY

    const std::string identifier = I_Info.Get<std::string>("identifier");
    CheckEntry(identifier, "identifier");

    const fs::path file_name(GetFileName("CoSimIO_data_" + GetConnectionName() + "_" + identifier, "dat"));

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Attempting to import array \"" << identifier << "\" in file " << file_name << " ..." << std::endl;

    WaitForPath(file_name);

    const auto start_time(std::chrono::steady_clock::now());

    SerializeFromFile(file_name, "data", rData, Serializer::TraceType::SERIALIZER_NO_TRACE);

    RemovePath(file_name);

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Finished importing array with size: " << rData.size() << std::endl;

    CO_SIM_IO_INFO_IF("CoSimIO", GetPrintTiming()) << "Importing Array \"" << identifier << "\" took: " << ElapsedSeconds(start_time) << " [sec]" << std::endl;

    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info FileCommunication::ExportDataImpl(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    CO_SIM_IO_TRY

    const std::string identifier = I_Info.Get<std::string>("identifier");
    CheckEntry(identifier, "identifier");

    const fs::path file_name(GetFileName("CoSimIO_data_" + GetConnectionName() + "_" + identifier, "dat"));

    WaitUntilFileIsRemoved(file_name); // TODO maybe this can be queued somehow ... => then it would not block the sender

    const std::size_t size = rData.size();
    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Attempting to export array \"" << identifier << "\" with size: " << size << " in file " << file_name << " ..." << std::endl;

    const auto start_time(std::chrono::steady_clock::now());

    SerializeToFile(GetTempFileName(file_name), "data", rData, Serializer::TraceType::SERIALIZER_NO_TRACE);

    MakeFileVisible(file_name);

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Finished exporting array" << std::endl;

    CO_SIM_IO_INFO_IF("CoSimIO", GetPrintTiming()) << "Exporting Array \"" << identifier << "\" took: " << ElapsedSeconds(start_time) << " [sec]" << std::endl;

    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info FileCommunication::ImportMeshImpl(
    const Info& I_Info,
    ModelPart& O_ModelPart)
{
    CO_SIM_IO_TRY

    const std::string identifier = I_Info.Get<std::string>("identifier");
    CheckEntry(identifier, "identifier");

    const fs::path file_name(GetFileName("CoSimIO_mesh_" + GetConnectionName() + "_" + identifier, "vtk"));

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Attempting to import mesh \"" << identifier << "\" in file " << file_name << " ..." << std::endl;

    WaitForPath(file_name);

    const auto start_time(std::chrono::steady_clock::now());

    SerializeFromFile(file_name, "model_part", O_ModelPart, Serializer::TraceType::SERIALIZER_NO_TRACE);

    RemovePath(file_name);

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Finished importing mesh" << std::endl;

    CO_SIM_IO_INFO_IF("CoSimIO", GetPrintTiming()) << "Importing Mesh \"" << identifier << "\" took: " << ElapsedSeconds(start_time) << " [sec]" << std::endl;

    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info FileCommunication::ExportMeshImpl(
    const Info& I_Info,
    const ModelPart& I_ModelPart)
{
    CO_SIM_IO_TRY

    const std::string identifier = I_Info.Get<std::string>("identifier");
    CheckEntry(identifier, "identifier");

    const fs::path file_name(GetFileName("CoSimIO_mesh_" + GetConnectionName() + "_" + identifier, "vtk"));

    WaitUntilFileIsRemoved(file_name); // TODO maybe this can be queued somehow ... => then it would not block the sender

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Attempting to export mesh \"" << identifier << "\" with " << I_ModelPart.NumberOfNodes() << " Nodes | " << I_ModelPart.NumberOfElements() << " Elements in file " << file_name << " ..." << std::endl;

    const auto start_time(std::chrono::steady_clock::now());

    SerializeToFile(GetTempFileName(file_name), "model_part", I_ModelPart, Serializer::TraceType::SERIALIZER_NO_TRACE);

    MakeFileVisible(file_name);

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Finished exporting mesh" << std::endl;

    CO_SIM_IO_INFO_IF("CoSimIO", GetPrintTiming()) << "Exporting Mesh \"" << identifier << "\" took: " << ElapsedSeconds(start_time) << " [sec]" << std::endl;

    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

fs::path FileCommunication::GetTempFileName(const fs::path& rPath) const
{
    CO_SIM_IO_TRY

    if (mCommInFolder) {
        return rPath.string().insert(mCommFolder.string().length()+1, ".");
    } else {
        return "." + rPath.string();
    }

    CO_SIM_IO_CATCH
}

fs::path FileCommunication::GetFileName(const fs::path& rPath, const std::string& rExtension) const
{
    CO_SIM_IO_TRY

    fs::path local_copy(rPath);
    local_copy += "_" + std::to_string((mFileIndex++)%100) + "." + rExtension;

    if (mCommInFolder) {
        return mCommFolder / local_copy;
    } else {
        return local_copy;
    }

    CO_SIM_IO_CATCH
}

void FileCommunication::WaitForPath(const fs::path& rPath) const
{
    CO_SIM_IO_TRY

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>0) << "Waiting for: " << rPath << std::endl;
    while(!fs::exists(rPath)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); // wait 0.001s before next check
    }
    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>0) << "Found: " << rPath << std::endl;

    CO_SIM_IO_CATCH
}

void FileCommunication::WaitUntilFileIsRemoved(const fs::path& rPath) const
{
    CO_SIM_IO_TRY

    if (fs::exists(rPath)) { // only issue the wating message if the file exists initially
        CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>0) << "Waiting for: " << rPath << " to be removed" << std::endl;
        while(fs::exists(rPath)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // wait 0.001s before next check
        }
        CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>0) << rPath << " was removed" << std::endl;
    }

    CO_SIM_IO_CATCH
}

void FileCommunication::MakeFileVisible(const fs::path& rPath) const
{
    CO_SIM_IO_TRY

    std::error_code ec;
    fs::rename(GetTempFileName(rPath), rPath, ec);
    CO_SIM_IO_ERROR_IF(ec) << rPath << " could not be made visible!\nError code: " << ec.message() << std::endl;

    CO_SIM_IO_ERROR << "Error deep down" << std::endl;

    CO_SIM_IO_CATCH
}

void FileCommunication::RemovePath(const fs::path& rPath) const
{
    CO_SIM_IO_TRY

    // In windows the file cannot be removed if another file handle is using it
    // this can be the case here if the partner checks if the file (still) exists
    // hence we try multiple times to delete it
    std::error_code ec;
    for (std::size_t i=0; i<5; ++i) {
        if (fs::remove(rPath, ec)) {
            return; // if file could be removed succesfully then return
        }
    }
    CO_SIM_IO_ERROR << rPath << " could not be deleted!\nError code: " << ec.message() << std::endl;

    CO_SIM_IO_CATCH
}

} // namespace Internals
} // namespace CoSimIO
