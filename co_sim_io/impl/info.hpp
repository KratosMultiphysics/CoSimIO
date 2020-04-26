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

#ifndef CO_SIM_IO_INFO_H_INCLUDED
#define CO_SIM_IO_INFO_H_INCLUDED

// System includes
#include <string>
#include <unordered_map>
// #include <typeinfo>
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
    TDataType Get(const std::string& I_Key) const
    {
        const bool key_exists = Has(I_Key);
        if (key_exists) {
            const auto& r_val = mOptions.at(I_Key);
            KRATOS_ERROR_IF_NOT(r_val.first == Name(TDataType())) << "Wrong DataType!";
            return static_cast<TDataType>(r_val.second);
        } else {
            return TDataType();
        }
    }

    bool Has(const std::string& I_Key) const
    {
        return mOptions.count(I_Key)>0;
    }

    template<typename TDataType>
    void SetValue(const std::string& I_Key, const TDataType I_Value)
    {
        static_assert(
            std::is_same<TDataType, double>::value ||
            std::is_same<TDataType, int>::value    ||
            std::is_same<TDataType, bool>::value   ||
            // std::is_same<TDataType, Info>::value   || // makes it recursive
            std::is_same<TDataType, std::string>::value,
                "Only allowed types are double, int, bool, string");

        mOptions[I_Key] = {Name(TDataType()), I_Value};
    }

    // GetType?

    // TODO do we need "erase", "clear", "size" ?

private:
    // TODO change type_info to std::string, otherwise serialization is not really possible
    std::unordered_map<std::string, std::pair<std::string, void*>> mOptions;

    std::string Name(int dummy)         {return "int";}
    std::string Name(double dummy)      {return "double";}
    std::string Name(bool dummy)        {return "bool";}
    std::string Name(std::string dummy) {return "string";}
};

// TODO add ofstream operator


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

// struct TransferInfo
// {
//     // TransferInfo(const std::string& rIdentifier) : Identifier(rIdentifier) {}

//     // std::string Identifier;
// };

// // struct MeshInfo
// // {

// // };



// class SolutionInfo
// {
// public:
//     SolutionInfo() {}

//     double CurrentTime;
//     double NewTime;
//     bool IsConverged=true;

//     std::unordered_map<std::string, int> MeshStates; // 0=Unchanged // 1=NodesMoved // 2=FullRemesh

// };


// struct ReturnInfo
// {
//     int ReturnCode;
// };

} // namespace CoSimIO

#endif // CO_SIM_IO_INFO_H_INCLUDED