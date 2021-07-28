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

#ifndef CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED
#define CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED

// System includes
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Project includes
#include "communication.hpp"
#include "../filesystem_inc.hpp"

namespace CoSimIO {
namespace Internals {

namespace { // helpers namespace



} // helpers namespace


class CO_SIM_IO_API PipeCommunication : public Communication
{
public:
    PipeCommunication(
        const Info& I_Settings,
        std::shared_ptr<DataCommunicator> I_DataComm);

    ~PipeCommunication() override
    {
        if (GetIsConnected()) {
            CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
            Info tmp;
            Disconnect(tmp);
        }
    }

private:

    fs::path mCommFolder;
    fs::path mPipeName;
    bool mCommInFolder = true;
    int mPipe;

    Info ConnectDetail(const Info& I_Info) override
    {
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

        if (GetIsPrimaryConnection()) {
            fs::remove(mPipeName);
            std::cout << "PIPE exists: " << fs::exists(mPipeName) << std::endl;
            CO_SIM_IO_ERROR_IF(mkfifo(mPipeName.c_str(), 0666) != 0) << "Pipe " << mPipeName << " could not be created!" << std::endl;
        }

        if (GetIsPrimaryConnection()) {
            CO_SIM_IO_ERROR_IF((mPipe = open(mPipeName.c_str(), O_WRONLY)) < 0) << "Pipe " << mPipeName << " could not be opened!" << std::endl;
        } else {
            CO_SIM_IO_ERROR_IF((mPipe = open(mPipeName.c_str(), O_RDONLY)) < 0) << "Pipe " << mPipeName << " could not be opened!" << std::endl;
        }

        // ExchangeSyncFileWithPartner();

        Info info;
        info.Set("is_connected", true);
        return info;
    }

    Info DisconnectDetail(const Info& I_Info) override
    {
        // ExchangeSyncFileWithPartner();

        close(mPipe);

        if (GetIsPrimaryConnection()) {
            // fs::remove(mPipeName);
        }

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
    }

    Info ImportInfoImpl(const Info& I_Info) override
    {
        // const std::string identifier = I_Info.Get<std::string>("identifier");
        // CheckEntry(identifier, "identifier");

        // CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Attempting to import Info ..." << std::endl;


        // std::cout << "Receiving size ..." << std::endl;
        // std::size_t received_size = ReceiveSize();
        // std::cout << "received size: " << received_size << std::endl;
        // std::size_t received_size_2 = ReceiveSize();
        // std::cout << "received size 2: " << received_size_2 << std::endl;


        // std::vector<char> buffer(received_size);
        // // char buffer[25600];
        // std::cout << "BEFORE read (size: " << received_size << ")" << std::endl;
        // read(mPipe, buffer.data(), received_size);
        // std::cout << "Afterread" << std::endl;

        // std::stringstream conv_stream(std::string(buffer.begin(), buffer.end()));
        // std::cout << "Received info: " << conv_stream.str() << std::endl;


        // open pipe in readonly mode
        // int pipe;
        // CO_SIM_IO_ERROR_IF((pipe = open(mPipeName.c_str(), O_RDONLY)) < 0) << "Pipe " << mPipeName << " could not be opened!" << std::endl;

        // std::cout << "before buffer creation" << std::endl;
        // std::vector<char> buffer(10*received_size);
        // char buffer2[25600];
        // std::cout << "before READING" << std::endl;
        // read(pipe, buffer2, received_size);
        // std::cout << "after READING" << std::endl;
        // close(pipe);


        // std::cout << "Received info: " << conv_stream.str() << std::endl;


        // std::cout <<"size buffer: " << sizeof(buffer) << std::endl;

        // std::string string_read(buffer);
        // std::cout << "READ from pipe: " << buffer << std::endl;
        // std::cout << "READ from pipe(String): " << string_read << "  length: " << string_read.length() << std::endl;
        // std::size_t imp_size_u;
        // std::stringstream conv_stream(string_read);
        // conv_stream >> imp_size_u;
        // std::cout << "READ from pipe(size_t): " << imp_size_u << std::endl;
        // int imp_size = std::stoi(string_read);
        // std::cout << "READ from pipe(int): " << imp_size << std::endl;

        // WaitForPath(file_name);

        // std::ifstream input_file(file_name);
        // CheckStream(input_file, file_name);

        Info imported_info;
        // imported_info.Load(conv_stream);

        // std::cout << "The importted INFO: " << imported_info << std::endl;

        // // input_file.close(); // TODO check return value?
        // // RemovePath(file_name);

        // CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Finished importing Info" << std::endl;

        return imported_info;
    }

    Info ExportInfoImpl(const Info& I_Info) override
    {
        // const std::string identifier = I_Info.Get<std::string>("identifier");
        // CheckEntry(identifier, "identifier");

        // CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Attempting to export Info ..." << std::endl;

        // std::stringstream info_stream;
        // I_Info.Save(info_stream);

        // std::size_t sending_size = info_stream.str().length();

        // std::cout << "sending size of: " << sending_size << std::endl;
        // SendSize(sending_size);
        // std::cout << "sending size of: " << 1234 << std::endl;
        // SendSize(1234);


        // std::cout << "BEFORE write (size: " << sending_size << ")" << std::endl;
        // write(mPipe, info_stream.str().c_str(), sending_size);
        // std::cout << "Afterwrite" << std::endl;

        // // open pipe in write mode
        // // int pipe;
        // // CO_SIM_IO_ERROR_IF((pipe = open(mPipeName.c_str(), O_WRONLY)) < 0) << "Pipe " << mPipeName << " could not be opened!" << std::endl;

        // // write(pipe, info_stream.str().c_str(), sending_size);
        // // close(pipe);

        // CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Finished exporting Info " << std::endl;

        return Info(); // TODO use
    }

    void SendSize(const std::uint64_t Size)
    {
        std::cout << "preparing to send size: " << Size << " (sizeof: " << sizeof(Size) << ")" << std::endl;

        write(mPipe, &Size, sizeof(Size));
    }

    std::uint64_t ReceiveSize()
    {
        std::uint64_t imp_size_u;
        std::cout << "preparing to receive size (sizeof: " << sizeof(imp_size_u) << ")" << std::endl;

        read(mPipe, &imp_size_u, sizeof(imp_size_u));

        return imp_size_u;
    }

};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED
