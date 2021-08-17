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
#include <string>
#include <map>
#include <chrono>
#include <thread>

// Project includes
#include "includes/utilities.hpp"

namespace CoSimIO {
namespace Internals {

// Create the name for the connection
// In a function bcs maybe in the future this will
// need to be more elaborate
std::string CreateConnectionName(
    const std::string& rName1,
    const std::string& rName2)
{
    if (rName1 < rName2) {
        return rName1 + "_" + rName2;
    } else {
        return rName2 + "_" + rName1;
    }
}

void CheckEntry(const std::string& rEntry, const std::string& rKey)
{
    // the entries are used e.g. for the folder creation, file names and other things
    // hence they are a little bit restricted to avoid unfortunate failures in rare cases

    const std::size_t max_allowed_size = 1000;
    CO_SIM_IO_ERROR_IF(rEntry.empty()) << "Using an empty entry for \"" << rKey << "\" is not allowed!" << std::endl;
    CO_SIM_IO_ERROR_IF(rEntry.length() > max_allowed_size) << "Entry for \"" << rKey << "\" is too long! Maximum allowed length: " << max_allowed_size << " characters!" << std::endl;

    const char disallowed_chars[] = {'.', ',', ':', ';', '>', '<', '/', '\'', '|', '*', '!', '"', ' '};
    for (const auto ch : disallowed_chars) {
        CO_SIM_IO_ERROR_IF_NOT(rEntry.find(ch) == std::string::npos) << "Entry for \"" << rKey << "\" contains a character that is not allowed: \"" << std::string(1,ch) << "\"!" << std::endl;
    }
}

double ElapsedSeconds(const std::chrono::steady_clock::time_point& rStartTime)
{
    using namespace std::chrono;
    return duration_cast<duration<double>>(steady_clock::now() - rStartTime).count();
}

int GetNumberOfNodesForElementType(ElementType Type)
{
    const std::map<ElementType, int> type_num_nodes_map {
        {ElementType::Hexahedra3D20,        20},
        {ElementType::Hexahedra3D27,        27},
        {ElementType::Hexahedra3D8,         8},
        {ElementType::Prism3D15,            15},
        {ElementType::Prism3D6,             6},
        {ElementType::Quadrilateral2D4,     4},
        {ElementType::Quadrilateral2D8,     8},
        {ElementType::Quadrilateral2D9,     9},
        {ElementType::Quadrilateral3D4,     4},
        {ElementType::Quadrilateral3D8,     8},
        {ElementType::Quadrilateral3D9,     9},
        {ElementType::Tetrahedra3D10,       10},
        {ElementType::Tetrahedra3D4,        4},
        {ElementType::Triangle2D3,          3},
        {ElementType::Triangle2D6,          6},
        {ElementType::Triangle3D3,          3},
        {ElementType::Triangle3D6,          6},
        {ElementType::Line2D2,              2},
        {ElementType::Line2D3,              3},
        {ElementType::Line3D2,              2},
        {ElementType::Line3D3,              3},
        {ElementType::Point2D,              1},
        {ElementType::Point3D,              1}
    };

    auto type_iter = type_num_nodes_map.find(Type);
    CO_SIM_IO_ERROR_IF(type_iter == type_num_nodes_map.end()) << "Unsupported element type: " << static_cast<int>(Type) << std::endl;
    return type_iter->second;
}

void WaitUntilPathExists(const fs::path& rPath)
{
    while(!fs::exists(rPath)) {std::this_thread::sleep_for(std::chrono::milliseconds(5));} // wait 0.005s before next check
}

std::unordered_set<std::size_t> ComputeNeighborRanks(
    const std::size_t MyRank,
    const std::size_t MySize,
    const std::size_t PartnerSize)
{
    // assert(MySize>0)
    // assert(MyRank<MySize)
    // assert(PartnerSize>0)

    if (MyRank == 0 && MySize == 1 && PartnerSize == 1) { // serial case
        return {0};
    } else if (MyRank == 0 && MySize == 1 && PartnerSize > 1) { // partner is distributed
        std::unordered_set<std::size_t> partner_ranks;
        for (std::size_t i=0; i<PartnerSize; ++i) {partner_ranks.insert(i);}
        return partner_ranks;
    } else if (MyRank < MySize && MyRank > PartnerSize) {
        return {};
    }
    return {};
}

} // namespace Internals
} // namespace CoSimIO
