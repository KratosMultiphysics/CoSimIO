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

#ifndef CO_SIM_IO_FILE_COMMUNICATION_INCLUDED
#define CO_SIM_IO_FILE_COMMUNICATION_INCLUDED

// System includes

// Project includes
#include "communication.hpp"
#include "../filesystem_inc.hpp"

namespace CoSimIO {
namespace Internals {

namespace { // helpers namespace

template <typename T>
static void CheckStream(const T& rStream, const fs::path& rPath)
{
    CO_SIM_IO_ERROR_IF_NOT(rStream.is_open()) << rPath << " could not be opened!" << std::endl;
}

} // helpers namespace


class CO_SIM_IO_API FileCommunication : public Communication
{
public:
    explicit FileCommunication(const Info& I_Settings);

    ~FileCommunication() override;

private:

    fs::path mCommFolder;
    bool mCommInFolder = true;
    mutable int mFileIndex = 0;

    Info ConnectDetail(const Info& I_Info) override;

    Info DisconnectDetail(const Info& I_Info) override;

    void ExchangeSyncFileWithPartner(const std::string& rIdentifier) const;

    Info ImportInfoImpl(const Info& I_Info) override;

    Info ExportInfoImpl(const Info& I_Info) override;

    Info ImportDataImpl(
        const Info& I_Info,
        Internals::DataContainer<double>& rData) override;

    Info ExportDataImpl(
        const Info& I_Info,
        const Internals::DataContainer<double>& rData) override;

    Info ImportMeshImpl(
        const Info& I_Info,
        ModelPart& O_ModelPart) override;

    Info ExportMeshImpl(
        const Info& I_Info,
        const ModelPart& I_ModelPart) override;

    fs::path GetTempFileName(const fs::path& rPath) const;

    fs::path GetFileName(const fs::path& rPath, const std::string& rExtension) const;

    void WaitForPath(const fs::path& rPath) const;

    void WaitUntilFileIsRemoved(const fs::path& rPath) const;

    void MakeFileVisible(const fs::path& rPath) const;

    void RemovePath(const fs::path& rPath) const;

};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_FILE_COMMUNICATION_INCLUDED
