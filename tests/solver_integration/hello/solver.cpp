// CoSimulation includes
#include "co_sim_io.hpp"

int main(){
    auto info = CoSimIO::Hello();

    std::cout << info << std::endl;

    int major_version = info.Get<int>("major_version");
    int minor_version = info.Get<int>("minor_version");
    std::string patch_version = info.Get<std::string>("patch_version");

    
    
    return 0;
}