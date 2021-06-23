#include "../co_sim_io.hpp"


namespace CoSimIO {

Info Hello()
{
    std::cout << "Hello, this is the CoSimIO\n";
    std::cout << "The detached interface for coupled simulations together with the\n";
    std::cout << "CoSimulationApplication of KratosMultiphysics\n\"https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication\"\n";
    std::cout << "Version:\n";
    std::cout << "    Major: " << GetMajorVersion() << "\n";
    std::cout << "    Minor: " << GetMinorVersion() << "\n";
    std::cout << "    Patch: " << GetPatchVersion() << "\n";
    std::cout << "For more information please visit \"https://github.com/KratosMultiphysics/CoSimIO\"" << std::endl;

    Info info;
    info.Set("major_version", GetMajorVersion());
    info.Set("minor_version", GetMinorVersion());
    info.Set("patch_version", GetPatchVersion());
    // TODO maybe add more things too here

    return info;
}

}