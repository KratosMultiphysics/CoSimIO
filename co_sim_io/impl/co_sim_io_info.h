// Kratos   ______     _____ _           ________
//         / ____/___ / ___/(_)___ ___  /  _/ __ |
//        / /   / __ \\__ \/ / __ `__ \ / // / / /
//       / /___/ /_/ /__/ / / / / / / // // /_/ /
//       \____/\____/____/_/_/ /_/ /_/___/\____/
//      Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Philipp Bucher (https://github.com/philbucher)
//

#ifndef KRATOS_CO_SIM_IO_INFO_H_INCLUDED
#define KRATOS_CO_SIM_IO_INFO_H_INCLUDED

// System includes
#include <string>
#include <unordered_map>
#include <typeinfo>
#include <utility>

#ifdef CO_SIM_IO_USING_MPI
#include "mpi.h"
#endif // CO_SIM_IO_USING_MPI

namespace CoSimIO {

class Info
{
public:
    virtual ~Info() = default;

    template<typename TDataType>
    TDataType GetValue(const std::string& rKey) // const ?
    {
        const bool key_exists = HasValue<TDataType>(rKey);
        // KRATOS_ERROR_IF_NOT(mOptions[rKey].first() == typeid(TDataType)) << "Wrong DataType!";
        if (key_exists) {
            return TDataType();
        } else {
            return static_cast<TDataType>(mOptions[rKey]);
        }
    }


    template<typename TDataType>
    bool HasValue(const std::string& rKey) const
    {
        const bool key_exists = mOptions.count(rKey)>0;
        if (key_exists) {
            KRATOS_CO_SIM_ERROR_IF_NOT(mOptions.at(rKey).first(), typeid(TDataType)) << "Wrong DataType!";
        }

        return key_exists;
    }

    template<typename TDataType>
    void SetValue(const std::string& rKey, const TDataType Value)
    {
        static_assert(
            std::is_same<TDataType, double>::value ||
            std::is_same<TDataType, int>::value    ||
            std::is_same<TDataType, bool>::value   ||
            // std::is_same<TDataType, Info>::value   || // makes it recursive
            std::is_same<TDataType, std::string>::value,
                "Only allowed types are double, int, bool, string");

        mOptions[rKey] = {typeid(TDataType), Value};
    }

private:
    std::unordered_map<std::string, std::pair<std::type_info, void*>> mOptions;
};


class ConnectionSettings : Info
{
public:
    // TODO set defaults in constructor

    // int GetEchoLevel() const {return mEchoLevel;}
    // void SetEchoLevel(const int EchoLevel) {mEchoLevel = EchoLevel;}

    // int GetNumberOfProcessors() const {return mNumberOfProcessors;}
    // void SetNumberOfProcessors(const int NumberOfProcessors) {mNumberOfProcessors = NumberOfProcessors;}

    #ifdef CO_SIM_IO_USING_MPI
    MPI_Comm GetMpiComm() const {return mMpiComm;}
    void SetMpiComm(const MPI_Comm MpiComm) {mMpiComm = MpiComm;}
    #endif // CO_SIM_IO_USING_MPI

private:
    // int mEchoLevel = 0;
    // int mNumberOfProcessors = 1;

    #ifdef CO_SIM_IO_USING_MPI
    MPI_Comm mMpiComm = nullptr;
    #endif // CO_SIM_IO_USING_MPI
};


// struct TransferInfoC
// {
//     TransferInfoC(const char* rIdentifier) : Identifier(rIdentifier) {}

//     std::string Identifier;
// };

struct TransferInfo
{
    // TransferInfo(const std::string& rIdentifier) : Identifier(rIdentifier) {}

    // std::string Identifier;
};

// struct MeshInfo
// {

// };



class SolutionInfo
{
public:
    SolutionInfo() {}

    double CurrentTime;
    double NewTime;
    bool IsConverged=true;

    std::unordered_map<std::string, int> MeshStates; // 0=Unchanged // 1=NodesMoved // 2=FullRemesh

};


struct ReturnInfo
{
    int ReturnCode;
};

} // namespace CoSimIO

#endif // KRATOS_CO_SIM_IO_INFO_H_INCLUDED