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
#include <utility>
#include <memory>
#include <type_traits>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef CO_SIM_IO_USING_MPI
#include "mpi.h"
#endif // CO_SIM_IO_USING_MPI

// Project includes
#include "macros.hpp"

namespace CoSimIO {

namespace Internals {
    std::string Name(int dummy)         {return "int";}
    std::string Name(double dummy)      {return "double";}
    std::string Name(bool dummy)        {return "bool";}
    std::string Name(std::string dummy) {return "string";}

class InfoDataBase
{
public:
    virtual ~InfoDataBase() = default;
    virtual const void* GetData() const = 0;
    virtual std::string GetDataTypeName() const = 0;
    virtual std::shared_ptr<InfoDataBase> Clone() const = 0;
    // virtual void Print(const void* pSource, std::ostream& rOStream) const;
    virtual void Save(std::ostream& out) const = 0;
    virtual void Load(std::istream& in) = 0;
};

template<class TDataType>
class InfoData : public InfoDataBase
{
public:
    explicit InfoData(const TDataType Source) : mData(Source) {}

    std::string GetDataTypeName() const override {return Internals::Name(TDataType());}

    const void* GetData() const override
    {
        return &mData;
    }

    // void Print(const void* pSource, std::ostream& rOStream) const override
    // {
    //     rOStream << Name() << " : " << *static_cast<const TDataType* >(pSource) ;
    // }

    std::shared_ptr<InfoDataBase> Clone() const override
    {
        return std::make_shared<InfoData<TDataType>>(TDataType());
    }

    void Save(std::ostream& out) const override
    {
        out << mData;
    }
    void Load(std::istream& in) override
    {
        in >> mData;

        std::cout << "Data after loading: " << mData << std::endl;
    }

private:
    TDataType mData;
};

} // namespace Internals


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
            CO_SIM_IO_ERROR_IF(r_val->GetDataTypeName() != Internals::Name(TDataType())) << "Wrong DataType! Trying to get \"" << I_Key << "\" which is of type \"" << r_val->GetDataTypeName() << "\" with \"" << Internals::Name(TDataType()) << "\"!" << std::endl;
            return *static_cast<const TDataType*>(r_val->GetData());
        } else {
            return TDataType();
        }
    }

    bool Has(const std::string& I_Key) const
    {
        return mOptions.count(I_Key)>0;
    }

    template<typename TDataType>
    void Set(const std::string& I_Key, TDataType I_Value)
    {
        static_assert(
            std::is_same<TDataType, double>::value ||
            std::is_same<TDataType, int>::value    ||
            std::is_same<TDataType, bool>::value   ||
            // std::is_same<TDataType, Info>::value   || // makes it recursive
            std::is_same<TDataType, std::string>::value,
                "Only allowed types are double, int, bool, string");

        mOptions[I_Key] = std::make_shared<Internals::InfoData<TDataType>>(I_Value);
    }

    void Erase(const std::string& I_Key)
    {
        mOptions.erase(I_Key);
    }

    void Clear()
    {
        mOptions.clear();
    }

    std::size_t Size() const
    {
        return mOptions.size();
    }

    void Save(std::ostream& out) const
    {
        static std::unordered_map<std::string, std::string> s_registerd_objects_name {
            {typeid(Internals::InfoData<int>).name(),         "InfoData_int"},
            {typeid(Internals::InfoData<double>).name(),      "InfoData_double"},
            {typeid(Internals::InfoData<bool>).name(),        "InfoData_bool"},
            {typeid(Internals::InfoData<std::string>).name(), "InfoData_string"}
        };
        // """
        // {
        //     'size' : 3,
        //     'data' : [
        //         {'name' : 'InfoData_int',    'value':55},
        //         {'name' : 'InfoData_bool',   'value':true},
        //         {'name' : 'InfoData_string', 'value':'maybe'}
        //     ]
        // }
        // """

        out << Size() << "\n";
        for (const auto& r_pair: mOptions) {
            auto it = s_registerd_objects_name.find(typeid(*(r_pair.second)).name());
            out << r_pair.first << "\n";
            out << it->second << "\n";
            r_pair.second->Save(out);
            out << "\n";
        }
    }
    void Load(std::istream& in)
    {
        static std::unordered_map<std::string, std::shared_ptr<Internals::InfoDataBase>> s_registerd_objects_name {
            {"InfoData_int"    , std::make_shared<Internals::InfoData<int>>(1)},
            {"InfoData_double" , std::make_shared<Internals::InfoData<double>>(1)},
            {"InfoData_bool"   , std::make_shared<Internals::InfoData<bool>>(1)},
            {"InfoData_string" , std::make_shared<Internals::InfoData<std::string>>("")}
        };

        std::string key, registered_name;

        int size;
        in >> size;
        std::cout << "Size read: " << size << std::endl;

        in.ignore(10, '\n'); // skipping to end of line

        for (int i=0; i<size; ++i) {
            std::getline(in, key);
            std::cout << "Current line: " << i << " | key: " << key << std::endl;
            std::getline(in, registered_name);
            std::cout << "Current line: " << i << " | registered_name: " << registered_name << std::endl;
            auto it_prototype = s_registerd_objects_name.find(registered_name);

            auto p = it_prototype->second->Clone();

            p->Load(in);

            in.ignore(10, '\n'); // skipping to end of line
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Internals::InfoDataBase>> mOptions;
};

// TODO add ofstream operator


class ConnectionSettings : public Info
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


struct ReturnInfo : public Info
{
    int ReturnCode() const { return Get<int>("return_code"); }
};

} // namespace CoSimIO

#endif // CO_SIM_IO_INFO_H_INCLUDED