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
#include <mutex>
#include <type_traits>

// Project includes
#include "../impl/info.hpp"

namespace CoSimIO {

namespace {
    std::once_flag flag_once;
}

void Info::Save(std::ostream& O_OutStream) const
{
    static std::map<std::string, std::string> s_registered_object_names {
        {typeid(Internals::InfoData<int>).name(),         "InfoData_int"},
        {typeid(Internals::InfoData<double>).name(),      "InfoData_double"},
        {typeid(Internals::InfoData<bool>).name(),        "InfoData_bool"},
        {typeid(Internals::InfoData<std::string>).name(), "InfoData_string"}
    };

    O_OutStream << Size() << "\n";
    for (const auto& r_pair: mOptions) {
        const auto& r_val = *(r_pair.second);
        auto it_obj = s_registered_object_names.find(typeid(r_val).name());
        CO_SIM_IO_ERROR_IF(it_obj == s_registered_object_names.end()) << "No name registered" << std::endl;
        O_OutStream << r_pair.first << "\n";
        O_OutStream << it_obj->second << "\n";
        r_pair.second->Save(O_OutStream);
        O_OutStream << "\n";
    }
}
void Info::Load(std::istream& I_InStream)
{
    static std::map<std::string, std::shared_ptr<Internals::InfoDataBase>> s_registered_object_prototypes {
        {"InfoData_int"    , std::make_shared<Internals::InfoData<int>>(1)},
        {"InfoData_double" , std::make_shared<Internals::InfoData<double>>(1)},
        {"InfoData_bool"   , std::make_shared<Internals::InfoData<bool>>(1)},
        {"InfoData_string" , std::make_shared<Internals::InfoData<std::string>>("")}
    };

    std::string key, registered_name;

    int size;
    I_InStream >> size;

    for (int i=0; i<size; ++i) {
        I_InStream >> key;
        I_InStream >> registered_name;
        auto it_prototype = s_registered_object_prototypes.find(registered_name);
        CO_SIM_IO_ERROR_IF(it_prototype == s_registered_object_prototypes.end()) << "No prototype registered for " << registered_name << std::endl;

        auto p_clone = it_prototype->second->Clone();
        p_clone->Load(I_InStream);
        mOptions[key] = p_clone;
    }
}

void Info::Print(std::ostream& rOStream) const
{
    rOStream << "CoSimIO-Info; containing " << Size() << " entries\n";

    for (const auto& r_pair: mOptions) {
        rOStream << "  name: " << r_pair.first << " | " << *(r_pair.second) << std::endl;
    }
}

void Info::save(CoSimIO::Internals::Serializer& rSerializer) const
{
    RegisterTypesInSerializer();

    rSerializer.save("mOptions", mOptions);
}
void Info::load(CoSimIO::Internals::Serializer& rSerializer)
{
    RegisterTypesInSerializer();
    rSerializer.load("mOptions", mOptions);
}

void Info::RegisterTypesInSerializer()
{
    if (!mpSerializerTypesRegistered) {
        // std::call_once(flag_once, [](){
            static bool types_are_registered; // no need to initialize, all that matters is that mpSerializerTypesRegistered is no longer a null_ptr
            mpSerializerTypesRegistered = &types_are_registered;

            static Internals::InfoData<int> info_data_int(1);
            static Internals::InfoData<double> info_data_double(1);
            static Internals::InfoData<bool> info_data_bool(1);
            static Internals::InfoData<std::string> info_data_string("");

            CoSimIO::Internals::Serializer::Register("info_data_int", info_data_int);
            CoSimIO::Internals::Serializer::Register("msInfoDataDouble", info_data_double);
            CoSimIO::Internals::Serializer::Register("msInfoDataBool", info_data_bool);
            CoSimIO::Internals::Serializer::Register("msInfoDataString", info_data_string);
        // });
    }
}

bool* Info::mpSerializerTypesRegistered = nullptr;

} // namespace CoSimIO
