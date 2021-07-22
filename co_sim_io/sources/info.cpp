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
#include "includes/info.hpp"

namespace CoSimIO {

// namespace {
//     std::once_flag flag_once;
// }

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
